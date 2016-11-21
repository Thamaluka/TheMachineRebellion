// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "LaserBoss.h"


// Sets default values
ALaserBoss::ALaserBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LaserPart = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleSys(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Elemental/Fire/P_Beam_Laser_Fire.P_Beam_Laser_Fire'"));
	if (ParticleSys.Succeeded()) {
		LaserPart->SetTemplate(ParticleSys.Object);
	}
	LaserPart->SetupAttachment(Root);

	LaserPart->bAutoActivate = true;

	bReplicates = true;
	bReplicateMovement = true;

}

// Called when the game starts or when spawned
void ALaserBoss::BeginPlay()
{
	Super::BeginPlay();
	Count = 500;
}

// Called every frame
void ALaserBoss::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if(Count >0){
		Count = Count -1;
	}

}

// Called to bind functionality to input
void ALaserBoss::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ALaserBoss::DestroyLaserBoss(){

	Destroy();
}
