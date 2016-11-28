// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Cyborg.h"

#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

#include "ProjectileActor.h"
#include "InimigoBot.h"
#include "Bottom.h"
#include "Escudo.h"
#include "InimigoMedium.h"
#include "Towers.h"
#include "Boss.h"
#include "LaserBoss.h"



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
	CameraBoom->TargetArmLength = 1500.f;
	CameraBoom->RelativeRotation = FRotator(-20.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false;


	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;


	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);

	GetMesh()->OnComponentHit.AddDynamic(this, &ACyborg::OnHit);

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Models/M_Cursor_Cyborg.M_Cursor_Cyborg'"));
	if (DecalMaterialAsset.Succeeded())
	{
	CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>Cyborg(TEXT("SkeletalMesh'/Game/Models/Personagens/Cyborg/Cyborg.Cyborg'"));
	if (Cyborg.Succeeded()) {
		GetMesh()->SetSkeletalMesh(Cyborg.Object);
	}

	ConstructorHelpers::FObjectFinder<UAnimBlueprint>AnimWalk(TEXT("AnimBlueprint'/Game/Models/Personagens/Cyborg/Animacao/CyborgAnimBlueprint.CyborgAnimBlueprint'"));
	if (AnimWalk.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(AnimWalk.Object->GetAnimBlueprintGeneratedClass());
	}

	ConstructorHelpers::FObjectFinder<UAnimSequence>AnimAtak01Load(TEXT("AnimSequence'/Game/Models/Personagens/Cyborg/Animacao/Cyborg_Ataque01_Anim.Cyborg_Ataque01_Anim'"));
	if (AnimAtak01Load.Succeeded()) {
		AnimAtak01 = AnimAtak01Load.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimSequence>AnimAtak02Load(TEXT("AnimSequence'/Game/Models/Personagens/Cyborg/Animacao/Cyborg_Ataque02_Anim.Cyborg_Ataque02_Anim'"));
	if (AnimAtak02Load.Succeeded()) {
		AnimAtak02 = AnimAtak02Load.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimSequence>AnimEscudoLoad(TEXT("AnimSequence'/Game/Models/Personagens/Cyborg/Animacao/Cyborg_Escudo_Anim.Cyborg_Escudo_Anim'"));
	if (AnimEscudoLoad.Succeeded()) {
		AnimEscudo = AnimEscudoLoad.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimSequence>AnimPowerLoad(TEXT("AnimSequence'/Game/Models/Personagens/Cyborg/Animacao/Cyborg_Raio_Anim.Cyborg_Raio_Anim'"));
	if (AnimPowerLoad.Succeeded()) {
		AnimPower = AnimPowerLoad.Object;
	}




	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollectCollision"));
	CollisionComp->InitSphereRadius(200.0f);
	CollisionComp->AttachTo(RootComponent);


	EnergyPart = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EnergyPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleSys(TEXT("ParticleSystem'/Game/Particulas/P_Genno_Overhead_Imp_01.P_Genno_Overhead_Imp_01'"));
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
	GetMesh()->SetWorldLocation(FVector(0.0f,0.0f,-70.0f));
	GetMesh()->SetWorldScale3D(FVector(0.2f,0.2f,0.2f));

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

ConstructorHelpers::FClassFinder<UUserWidget>Widget(TEXT("WidgetBlueprint'/Game/Blueprints/Menu/Pause.Pause_C'"));
if (Widget.Succeeded()) {
	UserWidget = Widget.Class;
}

ConstructorHelpers::FClassFinder<UUserWidget>OverWidget(TEXT("WidgetBlueprint'/Game/Blueprints/GameOver.GameOver_C'"));
if (OverWidget.Succeeded()) {
	GameOver = OverWidget.Class;
}

bReplicates = true;
	bReplicateMovement = true;

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


	if (GetMesh()->GetAnimationMode() == EAnimationMode::AnimationSingleNode && !GetMesh()->IsPlaying()) {
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}
}

// Called to bind functionality to input
void ACyborg::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("PowerQ", IE_Pressed, this, &ACyborg::Escudo);
	InputComponent->BindAction("SuperPowerR", IE_Pressed, this, &ACyborg::Energy);
	InputComponent->BindAction("PowerW", IE_Pressed, this, &ACyborg::Attack);
	InputComponent->BindAction("PowerE", IE_Pressed, this, &ACyborg::StrongAttack);

	InputComponent->BindAction("MouseLeft", IE_Pressed, this, &ACyborg::OnBottom);

	InputComponent->BindAction("Pause", IE_Pressed, this, &ACyborg::Pause);



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
		UWorld* World = GetWorld();
		if (World != nullptr) {
			APlayerController* PlayerController =
				UGameplayStatics::GetPlayerController(World, 0);
			if (PlayerController != nullptr && UserWidget != NULL) {
				PlayerController->SetPause(true);
				UUserWidget* UserW = UWidgetBlueprintLibrary::Create(World, GameOver, PlayerController);
				if (UserW != nullptr) {
					UserW->AddToViewport();
					PlayerController->bShowMouseCursor = true;
				}
			}
		}
	}
}


//Energia
void ACyborg::SetPower(int NewPower) {
	if (NewPower > 0 && Power+NewPower<=3000) {
		Power = NewPower + Power;
	}
}

int ACyborg::GetPower() {
	if (Power >= 3000) {
		SuperPower = true;
	}else{
		SuperPower = false;
	}
	return Power;
}

void ACyborg::Escudo(){
	if (AnimEscudo != nullptr) {
		GetMesh()->PlayAnimation(AnimEscudo, false);
	}
	FActorSpawnParameters SpawnParameters;
	UWorld* World = GetWorld();
	if (World != nullptr) {
		FRotator Rotation = RootComponent->GetComponentRotation();
		AEscudo* Escudo = World->SpawnActor<AEscudo>(GetActorLocation(), Rotation, SpawnParameters);
	}
}

void ACyborg::Energy(){

	if(Power >= 3000){
		if (AnimPower != nullptr) {
			GetMesh()->PlayAnimation(AnimPower, false);
		}
		Power = Power-1500;
		TArray<AActor*> Colidido;
		CollisionComp->GetOverlappingActors(Colidido);
		EnergyPart->ToggleActive();
		for (int i = 0; i < Colidido.Num(); i++) {
			if (Colidido[i]->IsA(AInimigoBot::StaticClass())) {
				AInimigoBot* InimigoPequeno = Cast<AInimigoBot>(Colidido[i]);
				InimigoPequeno->SetInimigoPeqLife(InimigoPequeno->GetInimigoPeqLife()-200);
				InimigoPequeno->InimigoPeqDeath();
				SetPower(100);

				//UE_LOG(LogTemp, Warning, TEXT("%d"), Inventory.Num());
			}else if(Colidido[i]->IsA(AInimigoMedium::StaticClass())){
				AInimigoMedium* InimigoMedio = Cast<AInimigoMedium>(Colidido[i]);
				InimigoMedio->SetInimigoMedLife(InimigoMedio->GetInimigoMedLife()-200);
				InimigoMedio->InimigoMedDeath();
				SetPower(100);
			}else if(Colidido[i]->IsA(ATowers::StaticClass())){
				ATowers* Tower = Cast<ATowers>(Colidido[i]);
				Tower->SetLife(Tower->GetLife()-200);
				Tower->OnDeath();
				SetPower(100);
			}else if(Colidido[i]->IsA(ABoss::StaticClass())){
				ABoss* Boss = Cast<ABoss>(Colidido[i]);
				if(Boss->GetTorres()<=0){
					Boss->SetLife(Boss->GetLife()-200);
					Boss->OnDeath();
					SetPower(100);
				}
			}
		}
	}
}

void ACyborg::Attack(){

	TArray<AActor*> Colidido;
	CollisionComp->GetOverlappingActors(Colidido);

	//PlayAnimacaoAttack01
	if (AnimAtak01 != nullptr) {
		GetMesh()->PlayAnimation(AnimAtak01, false);
	}
	for (int i = 0; i < Colidido.Num(); i++) {
		if (Colidido[i]->IsA(AInimigoBot::StaticClass())) {
			AInimigoBot* InimigoPequeno = Cast<AInimigoBot>(Colidido[i]);
			InimigoPequeno->SetInimigoPeqLife(InimigoPequeno->GetInimigoPeqLife()-50);
			InimigoPequeno->InimigoPeqDeath();
			SetPower(100);
			//UE_LOG(LogTemp, Warning, TEXT("%d"), Inventory.Num());
		}else if(Colidido[i]->IsA(AInimigoMedium::StaticClass())){
			AInimigoMedium* InimigoMedio = Cast<AInimigoMedium>(Colidido[i]);
			InimigoMedio->SetInimigoMedLife(InimigoMedio->GetInimigoMedLife()-50);
			InimigoMedio->InimigoMedDeath();
			SetPower(100);
		}else if(Colidido[i]->IsA(ATowers::StaticClass())){
			ATowers* Tower = Cast<ATowers>(Colidido[i]);
			Tower->SetLife(Tower->GetLife()-50);
			Tower->OnDeath();
			SetPower(100);
		}else if(Colidido[i]->IsA(ABoss::StaticClass())){
			ABoss* Boss = Cast<ABoss>(Colidido[i]);
			if(Boss->GetTorres()<=0){
				Boss->SetLife(Boss->GetLife()-50);
				Boss->OnDeath();
				SetPower(100);
			}
		}
	}
}

void ACyborg::StrongAttack(){
	TArray<AActor*> Colidido;
	CollisionComp->GetOverlappingActors(Colidido);

	if (AnimAtak02 != nullptr) {
		GetMesh()->PlayAnimation(AnimAtak02, false);
	}
	//PlayAnimacaoAttack02

	for (int i = 0; i < Colidido.Num(); i++) {
		if (Colidido[i]->IsA(AInimigoBot::StaticClass())) {
			AInimigoBot* InimigoPequeno = Cast<AInimigoBot>(Colidido[i]);
			InimigoPequeno->SetInimigoPeqLife(InimigoPequeno->GetInimigoPeqLife()-100);
			InimigoPequeno->InimigoPeqDeath();
			Power = Power+100;
			//UE_LOG(LogTemp, Warning, TEXT("%d"), Inventory.Num());
		}else if(Colidido[i]->IsA(AInimigoMedium::StaticClass())){
			AInimigoMedium* InimigoMedio = Cast<AInimigoMedium>(Colidido[i]);
			InimigoMedio->SetInimigoMedLife(InimigoMedio->GetInimigoMedLife()-100);
			InimigoMedio->InimigoMedDeath();
			Power = Power+200;
		}else if(Colidido[i]->IsA(ATowers::StaticClass())){
			ATowers* Tower = Cast<ATowers>(Colidido[i]);
			Tower->SetLife(Tower->GetLife()-100);
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

void ACyborg::Pause(){
	UWorld* World = GetWorld();
	if (World != nullptr) {
		APlayerController* PlayerController =
			UGameplayStatics::GetPlayerController(World, 0);
		if (PlayerController != nullptr && UserWidget != NULL) {
			PlayerController->SetPause(true);
			UUserWidget* UserW = UWidgetBlueprintLibrary::Create(World, UserWidget, PlayerController);
			if (UserW != nullptr) {
				UserW->AddToViewport();
				PlayerController->bShowMouseCursor = true;
			}

		}
	}
}



void ACyborg::OnHit(UPrimitiveComponent* HitComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if ((OtherActor != nullptr) && (OtherActor != this) &&
		(OtherComp != nullptr) && (OtherActor->IsA(AProjectileActor::StaticClass()))) {
			AProjectileActor* Projectile = Cast<AProjectileActor>(OtherActor);
			Life = Life - 100;

			UE_LOG(LogTemp, Warning, TEXT("TIRO"));
		}else if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor->IsA(ALaserBoss::StaticClass()))) {
					ALaserBoss* Laser = Cast<ALaserBoss>(OtherActor);
					Life = Life -50;
				}
	}
