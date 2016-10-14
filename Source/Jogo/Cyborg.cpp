// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Cyborg.h"
#include "InimigoBot.h"
#include "Bottom.h"


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


	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollectCollision"));
	CollisionComp->InitSphereRadius(200.0f);
	CollisionComp->AttachTo(RootComponent);

	EscudoPart = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EscudoPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleSystem(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_Aura/P_Aura_Ice_Shatter_01.P_Aura_Ice_Shatter_01'"));
	if (ParticleSystem.Succeeded()) {
		EscudoPart->SetTemplate(ParticleSystem.Object);
	}
	EscudoPart->SetupAttachment(CollisionComp);
	EscudoPart->bAutoActivate = false;


	EnergyPart = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EnergyPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleSys(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_Aura/P_Aura_Ice_Shatter_01.P_Aura_Ice_Shatter_01'"));
	if (ParticleSystem.Succeeded()) {
			EnergyPart->SetTemplate(ParticleSys.Object);
		}
	EnergyPart->SetupAttachment(CollisionComp);
	EnergyPart->bAutoActivate = false;


	Life = 5000;

	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());


	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

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

}

void ACyborg::SetLife(int NewLife) {
	Life = NewLife;
}


int ACyborg::GetLife() {
	return Life;
}

void ACyborg::OnDeath() {
	if (Life <= 0) {
		Life = 5000;
		SetActorLocation(StartPlayer);
	}
}


//Energia
void ACyborg::SetPower(int NewPower) {
	if (NewPower > 0) {
		NewPower = NewPower + Power;
	}
	else {
		Power = NewPower;
	}
}

int ACyborg::GetPower() {
	if (Power > 3000) {
		SuperPower = true;
	}
	return Power;
}

void ACyborg::Escudo(){
	EscudoPart->ToggleActive();
}

void ACyborg::Energy(){
	TArray<AActor*> Colidido;
	CollisionComp->GetOverlappingActors(Colidido);
	EnergyPart->ToggleActive();
	for (int i = 0; i < Colidido.Num(); i++) {
		if (Colidido[i]->IsA(ACyborg::StaticClass())) {
			AInimigoBot* InimigoPequeno = Cast<AInimigoBot>(Colidido[i]);
			InimigoPequeno->SetInimigoPeqLife(InimigoPequeno->GetInimigoPeqLife()-200);
			InimigoPequeno->InimigoPeqDeath();
			//UE_LOG(LogTemp, Warning, TEXT("%d"), Inventory.Num());
		}
	}
}
