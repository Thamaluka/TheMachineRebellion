// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "QuimicaPower.h"


// Sets default values
AQuimicaPower::AQuimicaPower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	Root->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	RootComponent = Root;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName("NoCollision");

	

	static ConstructorHelpers::FObjectFinder<UMaterial>
		Material(TEXT("Material'/Game/InfinityBladeEffects/Effects/FX_Materials/Mobile/M_FogSheet_Trans.M_FogSheet_Trans'"));
	
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
		ParticleSystem(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_Aura/P_Aura_Poison_Shatter_01.P_Aura_Poison_Shatter_01'"));

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
void AQuimicaPower::BeginPlay()
{
	Super::BeginPlay();
	DefaultZ = GetActorLocation().Z;
}

// Called every frame
void AQuimicaPower::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	FVector Location = GetActorLocation();
	Location.Z = DefaultZ;
	SetActorLocation(Location);
}

