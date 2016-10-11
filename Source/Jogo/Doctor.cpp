// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Doctor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "ProjectileActor.h"
#include "InimigoBot.h"


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

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/StarterContent/Materials/NewMaterial.NewMaterial'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}

	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());


	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Life = 3000;
	Power = 0;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollectCollision"));
	CollisionComp->InitSphereRadius(200.0f);
	CollisionComp->AttachTo(RootComponent);

	NitrogenioPart = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("NitrogenioPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleSystem(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_FrostGiant/ICE/P_ArmBuildUp01.P_ArmBuildUp01'"));
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
	static ConstructorHelpers::FObjectFinder<UParticleSystem>Particle(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_Aura/P_AuraCircle_Ice_Vortex_01.P_AuraCircle_Ice_Vortex_01'"));
	if (Particle.Succeeded()) {
		CuraPart->SetTemplate(Particle.Object);
	}
	CuraPart->SetupAttachment(CollisionComp);
	CuraPart->bAutoActivate = false;
	CuraPart->bAutoDestroy = true;



	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ADoctor::BeginPlay()
{
	Super::BeginPlay();
	StartPlayer = FVector(-1580.0f,-40.0f,108.0f);
}

// Called every frame
void ADoctor::Tick( float DeltaTime )
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
void ADoctor::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Cura", IE_Pressed, this, &ADoctor::Cura);
	InputComponent->BindAction("Bomba", IE_Pressed, this, &ADoctor::Bomba);
	InputComponent->BindAction("Quimico", IE_Pressed, this, &ADoctor::Quimico);
	InputComponent->BindAction("Nitrogenio", IE_Pressed, this, &ADoctor::Nitrogenio);

}

void ADoctor::SetLife(int NewLife) {
	Life = NewLife;
}


int ADoctor::GetLife() {
	return Life;
}

void ADoctor::OnDeath() {
	if (Life <= 0) {
		Life = 3000;
		SetActorLocation(StartPlayer);
	}
}


//Energia
void ADoctor::SetPower(int NewPower) {
	if (NewPower > 0) {
		NewPower = NewPower + Power;
	}
	else {
		Power = NewPower;
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
		if (Life + 120 < 3000){
						Life = Life + 120;
		}

		UE_LOG(LogTemp, Warning, TEXT("Cura"));
}

void ADoctor::Bomba() {
	FActorSpawnParameters SpawnParameters;
	UWorld* World = GetWorld();
	if (World != nullptr) {
		FRotator Rotation = RootComponent->GetComponentRotation();
		AProjectileActor* Proj = World->SpawnActor<AProjectileActor>
			(GetActorLocation(), Rotation,
				SpawnParameters);
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
		}
	}
}


void ADoctor::Nitrogenio() {

	TArray<AActor*> Colidido;
	CollisionComp->GetOverlappingActors(Colidido);
	NitrogenioPart->ToggleActive();
	for (int i = 0; i < Colidido.Num(); i++) {
		if (Colidido[i]->IsA(AInimigoBot::StaticClass())) {
			AInimigoBot* InimigoPequeno = Cast<AInimigoBot>(Colidido[i]);
			InimigoPequeno->SetInimigoPeqLife(InimigoPequeno->GetInimigoPeqLife()-200);
			InimigoPequeno->InimigoPeqDeath();
			//UE_LOG(LogTemp, Warning, TEXT("%d"), Inventory.Num());
		}
	}
}
