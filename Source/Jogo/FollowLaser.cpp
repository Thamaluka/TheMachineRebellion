// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "FollowLaser.h"


// Sets default values
AFollowLaser::AFollowLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FollowLaser = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Laser"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshTower(TEXT("Mesh"));
	if (MeshTower.Succeeded()) {
		FollowLaser->SetStaticMesh(MeshTower.Object);
	}
	FollowLaser->SetWorldScale3D(FVector(1.0f,15.0f,2.0f));
	FollowLaser->SetWorldRotation(FRotator(0.0f,0.0f,-90.0f));
	FollowLaser->SetCollisionProfileName("BlockAll");

	LaserPart = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleSys(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Elemental/Fire/P_Beam_Laser_Fire.P_Beam_Laser_Fire'"));
	if (ParticleSys.Succeeded()) {
			LaserPart->SetTemplate(ParticleSys.Object);
		}
	LaserPart->SetWorldScale3D(FVector(1.0f,15.0f,2.0f));
	LaserPart->SetWorldRotation(FRotator(0.0f,-90.0f,0.0f));
	LaserPart->SetupAttachment(FollowLaser);
	LaserPart->bAutoActivate = true;


}

// Called when the game starts or when spawned
void AFollowLaser::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFollowLaser::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
