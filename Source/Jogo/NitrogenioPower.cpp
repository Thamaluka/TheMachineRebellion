// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "NitrogenioPower.h"


// Sets default values
ANitrogenioPower::ANitrogenioPower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	Root->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	RootComponent = Root;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName("NoCollision");

	static ConstructorHelpers::FObjectFinder<UMaterial>
		Material(TEXT("MaterialInstanceConstant'/Game/InfinityBladeEffects/Effects/FX_Materials/ICE/M_IceTrap_Shard_INST.M_IceTrap_Shard_INST'"));
	if (Material.Succeeded()) {
		MeshComp->SetMaterial(0, Material.Object);
	}
	MeshComp->SetWorldLocation(FVector(0.0f, 0.0f, -30.0f));
	MeshComp->SetWorldScale3D(FVector(100.0f, 100.0f, 100.0f));
	MeshComp->AttachTo(RootComponent);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>
		(TEXT("Particle"));
	Particle->bAutoActivate = true;
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ParticleSystem(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/ICE/combat/P_Moving_Spin_Ice_02.P_Moving_Spin_Ice_02'"));

	if (ParticleSystem.Succeeded()) {
		Particle->SetTemplate(ParticleSystem.Object);
	}
	Particle->SetupAttachment(RootComponent);
	//Particle->ToggleActive();

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = Root;
	ProjectileMovement->InitialSpeed = 30.0f;
	ProjectileMovement->MaxSpeed = 30.0f;

	InitialLifeSpan = 1.0f;
}

// Called when the game starts or when spawned
void ANitrogenioPower::BeginPlay()
{
	Super::BeginPlay();
	DefaultZ = GetActorLocation().Z;
}

// Called every frame
void ANitrogenioPower::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	FVector Location = GetActorLocation();
	Location.Z = DefaultZ;
	SetActorLocation(Location);
}

