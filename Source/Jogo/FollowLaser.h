// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FollowLaser.generated.h"

UCLASS()
class JOGO_API AFollowLaser : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFollowLaser();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent*FollowLaser;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* LaserPart;

};
