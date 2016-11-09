// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Doctor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "ProjectileActor.h"
#include "InimigoBot.h"
#include "Bottom.h"
#include "Cyborg.h"
#include "InimigoMedium.h"
#include "Towers.h"
#include "Boss.h"


// Sets default values
ADoctor::ADoctor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false;


	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;


	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Models/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}


	static ConstructorHelpers::FObjectFinder<USkeletalMesh>Doctor(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (Doctor.Succeeded()) {
		GetMesh()->SetSkeletalMesh(Doctor.Object);
	}
	GetMesh()->SetWorldRotation(FRotator(0.0f,-90.0f,0.0f));
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ADoctor::OnOverlapBegin);


	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());


	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Life = 5000;
	Power = 0;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollectCollision"));
	CollisionComp->InitSphereRadius(200.0f);
	CollisionComp->AttachTo(RootComponent);

	NitrogenioPart = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("NitrogenioPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleSystem(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_Aura/P_Aura_Ice_Shatter_01.P_Aura_Ice_Shatter_01'"));
	if (ParticleSystem.Succeeded()) {
		NitrogenioPart->SetTemplate(ParticleSystem.Object);
	}
	NitrogenioPart->SetupAttachment(CollisionComp);
	NitrogenioPart->bAutoActivate = false;


	QuimicoPart = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("QuimicoPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleSys(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_Aura/P_Aura_Poison_Shatter_01.P_Aura_Poison_Shatter_01'"));
	if (ParticleSys.Succeeded()) {
		QuimicoPart->SetTemplate(ParticleSys.Object);
	}
	QuimicoPart->SetupAttachment(CollisionComp);
	QuimicoPart->bAutoActivate = false;

	CuraPart = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CuraPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>Particle(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Archive/P_HealthOrb_Pickup.P_HealthOrb_Pickup'"));
	if (Particle.Succeeded()) {
		CuraPart->SetTemplate(Particle.Object);
	}
	CuraPart->SetupAttachment(CollisionComp);
	CuraPart->bAutoActivate = false;

	bReplicates = true;

	ConstructorHelpers::FObjectFinder<USoundCue>SoundCue(TEXT("SoundCue'/Game/Sounds/HurtCharacters_Cue.HurtCharacters_Cue'"));
	if (SoundCue.Succeeded()) {
		HitSound = SoundCue.Object;
	}

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->bAutoActivate = false;
	AudioComp->AttachTo(GetMesh());


	//AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ADoctor::BeginPlay()
{
	Super::BeginPlay();
	StartPlayer = FVector(2244.0f,6175.0f,110.0f);
}

// Called every frame
void ADoctor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	Tiro = false;

	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

// Called to bind functionality to input
void ADoctor::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("PowerQ", IE_Pressed, this, &ADoctor::Cura);
	InputComponent->BindAction("PowerW", IE_Pressed, this, &ADoctor::Bomba);
	InputComponent->BindAction("PowerE", IE_Pressed, this, &ADoctor::Quimico);
	InputComponent->BindAction("SuperPowerR", IE_Pressed, this, &ADoctor::Nitrogenio);
	InputComponent->BindAction("MouseLeft", IE_Pressed, this, &ADoctor::OnBottom);

}

void ADoctor::SetLife(int NewLife) {
	Life = NewLife;
}


int ADoctor::GetLife() {
	return Life;
}

void ADoctor::OnDeath() {
	AudioComp->SetSound(HitSound);
	AudioComp->Play();
	if (Life <= 0) {
		Life = 3000;
		SetActorLocation(StartPlayer);
	}
}


//Energia
void ADoctor::SetPower(int NewPower) {
	if (NewPower > 0 && Power+NewPower<3000) {
		NewPower = NewPower + Power;
	}
}

int ADoctor::GetPower() {
	if (Power > 3000) {
		SuperPower = true;
	}
	return Power;
}

//Poderes pelas teclas Q,W,E e R
void ADoctor::Cura() {
	CuraPart->ToggleActive();
	TArray<AActor*> Curas;
	CollisionComp->GetOverlappingActors(Curas);

	for (int i = 0; i < Curas.Num(); i++) {
		if (Curas[i]->IsA(ACyborg::StaticClass())) {
			ACyborg* Cyborg = Cast<ACyborg>(Curas[i]);
			if(Cyborg->GetLife()+100 <5000){
				Cyborg->SetLife(Cyborg->GetLife()+100);
			}
		}
	}
if (Life + 120 < 3000){
	Life = Life + 120;
}
}

void ADoctor::Bomba() {
	Tiro = true;
	FActorSpawnParameters SpawnParameters;
	UWorld* World = GetWorld();
	if (World != nullptr) {
		FRotator Rotation = RootComponent->GetComponentRotation();
		AProjectileActor* Proj = World->SpawnActor<AProjectileActor>
			(GetActorLocation(), Rotation,SpawnParameters);
		if (Proj != nullptr) {
		//	UE_LOG(LogTemp, Warning, TEXT("Spawn OK!"));
		}
	}

}

void ADoctor::Quimico() {
	TArray<AActor*> Colidido;
	CollisionComp->GetOverlappingActors(Colidido);
	QuimicoPart->ToggleActive();
	for (int i = 0; i < Colidido.Num(); i++) {
		if (Colidido[i]->IsA(AInimigoBot::StaticClass())) {
			AInimigoBot* InimigoPequeno = Cast<AInimigoBot>(Colidido[i]);
			InimigoPequeno->SetInimigoPeqLife(InimigoPequeno->GetInimigoPeqLife()-100);
			InimigoPequeno->InimigoPeqDeath();
				Power = Power+100;
		}else if(Colidido[i]->IsA(AInimigoMedium::StaticClass())){
			AInimigoMedium* InimigoMedio = Cast<AInimigoMedium>(Colidido[i]);
			InimigoMedio->SetInimigoMedLife(InimigoMedio->GetInimigoMedLife()-200);
			InimigoMedio->InimigoMedDeath();
				Power = Power+100;
		}else if(Colidido[i]->IsA(ATowers::StaticClass())){
			ATowers* Tower = Cast<ATowers>(Colidido[i]);
			Tower->SetLife(Tower->GetLife()-100);
			Tower->OnDeath();
			Power = Power+100;
		}else if(Colidido[i]->IsA(ABoss::StaticClass())){
			ABoss* Boss = Cast<ABoss>(Colidido[i]);
			if(Boss->GetTorres()<=0){
				Boss->SetLife(Boss->GetLife()-100);
				Boss->OnDeath();
				Power = Power+100;
			}
		}
	}
}


void ADoctor::Nitrogenio() {
	if(SuperPower){
		TArray<AActor*> Colidido;
		CollisionComp->GetOverlappingActors(Colidido);
		NitrogenioPart->ToggleActive();
		for (int i = 0; i < Colidido.Num(); i++) {
			if (Colidido[i]->IsA(AInimigoBot::StaticClass())) {
				AInimigoBot* InimigoPequeno = Cast<AInimigoBot>(Colidido[i]);
				InimigoPequeno->SetInimigoPeqLife(InimigoPequeno->GetInimigoPeqLife()-200);
				InimigoPequeno->InimigoPeqDeath();
				//UE_LOG(LogTemp, Warning, TEXT("%d"), Inventory.Num());
			}else if(Colidido[i]->IsA(AInimigoMedium::StaticClass())){
				AInimigoMedium* InimigoMedio = Cast<AInimigoMedium>(Colidido[i]);
				InimigoMedio->SetInimigoMedLife(InimigoMedio->GetInimigoMedLife()-200);
				InimigoMedio->InimigoMedDeath();
			}else if(Colidido[i]->IsA(ATowers::StaticClass())){
				ATowers* Tower = Cast<ATowers>(Colidido[i]);
				Tower->SetLife(Tower->GetLife()-100);
				Tower->OnDeath();
			}else if(Colidido[i]->IsA(ABoss::StaticClass())){
				ABoss* Boss = Cast<ABoss>(Colidido[i]);
				if(Boss->GetTorres()<=0){
					Boss->SetLife(Boss->GetLife()-100);
					Boss->OnDeath();
					Power = Power+100;
				}
			}
		}
	}
}

//Colisao com o botao para abertura do laser
void ADoctor::OnBottom() {
	TArray<AActor*>AtoresColetaveis;
	CollisionComp->GetOverlappingActors(AtoresColetaveis);

	for (int i = 0; i < AtoresColetaveis.Num(); i++) {
		 if (AtoresColetaveis[i]->IsA(ABottom::StaticClass())) {
			ABottom* Botao = Cast<ABottom>(AtoresColetaveis[i]);
			if(Botao->GetBottomNum()==Id){
					Botao->OnPressed();
			}
		}
	}

}

void ADoctor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) &&
		(OtherComp != nullptr) && (OtherActor->IsA(AProjectileActor::StaticClass()))) {
		if (!Tiro) {
			AProjectileActor* Projectile = Cast<AProjectileActor>(OtherActor);
			Life = Life - 100;

			UE_LOG(LogTemp, Warning, TEXT("TIRO"));
		}
	}
}
