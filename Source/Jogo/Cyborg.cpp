// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Cyborg.h"

#include "InimigoBot.h"
#include "Bottom.h"
#include "Escudo.h"
#include "InimigoMedium.h"
#include "Towers.h"
#include "Boss.h"



// Sets default values
ACyborg::ACyborg()
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

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Models/M_Cursor_Cyborg.M_Cursor_Cyborg'"));
	if (DecalMaterialAsset.Succeeded())
	{
	CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>Cyborg(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (Cyborg.Succeeded()) {
		GetMesh()->SetSkeletalMesh(Cyborg.Object);
	}


	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollectCollision"));
	CollisionComp->InitSphereRadius(200.0f);
	CollisionComp->AttachTo(RootComponent);


	EnergyPart = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EnergyPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleSys(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_Aura/P_Aura_Ice_Shatter_01.P_Aura_Ice_Shatter_01'"));
	if (ParticleSys.Succeeded()) {
			EnergyPart->SetTemplate(ParticleSys.Object);
		}
	EnergyPart->SetupAttachment(CollisionComp);
	EnergyPart->bAutoActivate = false;


	Life = 5000;
	Power = 3000;

	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	GetMesh()->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
//	AutoPossessPlayer = EAutoReceiveInput::Player0;


ConstructorHelpers::FObjectFinder<USoundCue>SoundCue(TEXT("SoundCue'/Game/Sounds/HurtCharacters_Cue.HurtCharacters_Cue'"));
if (SoundCue.Succeeded()) {
	HitSound = SoundCue.Object;
}

AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
AudioComp->bAutoActivate = false;
AudioComp->AttachTo(GetMesh());


}

// Called when the game starts or when spawned
void ACyborg::BeginPlay()
{
	Super::BeginPlay();
		StartPlayer = FVector(2244.0f,6175.0f,110.0f);
}

// Called every frame
void ACyborg::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
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
void ACyborg::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("PowerQ", IE_Pressed, this, &ACyborg::Escudo);
	InputComponent->BindAction("SuperPowerR", IE_Pressed, this, &ACyborg::Energy);
	InputComponent->BindAction("MouseLeft", IE_Pressed, this, &ACyborg::OnBottom);

//	InputComponent->BindAction("Pause", IE_Pressed, this, &ACyborg::Pause);


}

void ACyborg::SetLife(int NewLife) {
	Life = NewLife;
}


int ACyborg::GetLife() {
	return Life;
}

void ACyborg::OnDeath() {
	AudioComp->SetSound(HitSound);
	AudioComp->Play();
	if (Life <= 0) {
		Life = 5000;
		SetActorLocation(StartPlayer);
	}
}


//Energia
void ACyborg::SetPower(int NewPower) {
	if (NewPower > 0 && Power+NewPower<5000) {
		NewPower = NewPower + Power;
	}
}

int ACyborg::GetPower() {
	if (Power > 5000) {
		SuperPower = true;
	}
	return Power;
}

void ACyborg::Escudo(){
	FActorSpawnParameters SpawnParameters;
	UWorld* World = GetWorld();
	if (World != nullptr) {
		FRotator Rotation = RootComponent->GetComponentRotation();
		AEscudo* Escudo = World->SpawnActor<AEscudo>(GetActorLocation(), Rotation, SpawnParameters);
	}

}

void ACyborg::Energy(){
//	if(SuperPower){
		TArray<AActor*> Colidido;
		CollisionComp->GetOverlappingActors(Colidido);
		EnergyPart->ToggleActive();
		for (int i = 0; i < Colidido.Num(); i++) {
			if (Colidido[i]->IsA(AInimigoBot::StaticClass())) {
				AInimigoBot* InimigoPequeno = Cast<AInimigoBot>(Colidido[i]);
				InimigoPequeno->SetInimigoPeqLife(InimigoPequeno->GetInimigoPeqLife()-200);
				InimigoPequeno->InimigoPeqDeath();
				Power = Power+100;
				//UE_LOG(LogTemp, Warning, TEXT("%d"), Inventory.Num());
			}else if(Colidido[i]->IsA(AInimigoMedium::StaticClass())){
				AInimigoMedium* InimigoMedio = Cast<AInimigoMedium>(Colidido[i]);
				InimigoMedio->SetInimigoMedLife(InimigoMedio->GetInimigoMedLife()-200);
				InimigoMedio->InimigoMedDeath();
				Power = Power+200;
			}else if(Colidido[i]->IsA(ATowers::StaticClass())){
				ATowers* Tower = Cast<ATowers>(Colidido[i]);
				Tower->SetLife(Tower->GetLife()-200);
				Tower->OnDeath();
				Power = Power+200;
			}else if(Colidido[i]->IsA(ABoss::StaticClass())){
				ABoss* Boss = Cast<ABoss>(Colidido[i]);
				if(Boss->GetTorres()<=0){
					Boss->SetLife(Boss->GetLife()-100);
					Boss->OnDeath();
					Power = Power+100;
				}
			}
		}
	//}
}

//Colisao com o botao para abertura do laser
void ACyborg::OnBottom() {
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
