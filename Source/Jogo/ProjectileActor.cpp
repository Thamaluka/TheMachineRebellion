// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "ProjectileActor.h"
#include "Doctor.h"
#include "Cyborg.h"



// Sets default values
AProjectileActor::AProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	Root->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	RootComponent = Root;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName("NoCollision");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		Mesh(TEXT("StaticMesh'/Game/InfinityBladeEffects/Effects/FX_Meshes/Enemy/SM_RoboSpikeBall.SM_RoboSpikeBall'"));

	if (Mesh.Succeeded()) {
		MeshComp->SetStaticMesh(Mesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterial>
		Material(TEXT("Material'/Game/InfinityBladeEffects/Effects/FX_Materials/Mobile/M_JaggedRockFire_MeshEmit_Master.M_JaggedRockFire_MeshEmit_Master'"));
	if (Material.Succeeded()) {
		MeshComp->SetMaterial(0,Material.Object);
	}
	MeshComp->SetWorldLocation(FVector(0.0f, 0.0f, -30.0f));
	MeshComp->SetWorldScale3D(FVector(0.025f, 0.025f, 0.025f));
	MeshComp->AttachTo(RootComponent);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>
		(TEXT("Particle"));
	Particle->bAutoActivate = false;
	/*
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ParticleSystem(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Spider/Fire/P_Spider_Proj_Fire_SM.P_Spider_Proj_Fire_SM'"));
	if (ParticleSystem.Succeeded()) {
		Particle->SetTemplate(ParticleSystem.Object);
	}
	*/
	Particle->SetupAttachment(RootComponent);

	//Particle->ToggleActive();

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = Root;
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 5000.0f;

	InitialLifeSpan = 15.0f;

	bReplicates = true;
	bReplicateMovement = true;

}

// Called when the game starts or when spawned
void AProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	DefaultZ = GetActorLocation().Z;
}

// Called every frame
void AProjectileActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	Super::Tick(DeltaTime);

	FVector Location = GetActorLocation();
	Location.Z = DefaultZ;
	SetActorLocation(Location);
}

void AProjectileActor::DestroyProjectile() {
	Destroy();
}

void AProjectileActor::Particulas(int Tipo) {
	//UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/Materials/DamageMaterial.DamageMaterial'")));
	if (Tipo==1) {
		UParticleSystem* Part = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), NULL, TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Spider/Fire/P_Spider_Proj_Fire_SM.P_Spider_Proj_Fire_SM'")));

		if (Part!=nullptr && Particle!= nullptr) {
			Particle->SetTemplate(Part);
			Particle->ToggleActive();
		}
	}
	else if (Tipo==2) {
		UParticleSystem* Part = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), NULL, TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/P_Beam_Laser_FireAttack.P_Beam_Laser_FireAttack'")));

		if (Part != nullptr && Particle != nullptr) {
			Particle->SetTemplate(Part);
			Particle->ToggleActive();
		}
	}
}
<<<<<<< HEAD

=======
>>>>>>> dev
