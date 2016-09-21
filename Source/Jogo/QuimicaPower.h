// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "QuimicaPower.generated.h"

UCLASS()
class JOGO_API AQuimicaPower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuimicaPower();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:
	USphereComponent* Root;
	UStaticMeshComponent* MeshComp;
	UParticleSystemComponent* Particle;
	UProjectileMovementComponent* ProjectileMovement;

	float DefaultZ;
	
};
