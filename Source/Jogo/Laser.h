// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Laser.generated.h"

UCLASS()
class JOGO_API ALaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaser();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	FORCEINLINE UStaticMeshComponent*GetDoor() const { return Door; }
	FORCEINLINE bool IsOpen()const { return Open; }
	FORCEINLINE void ALaser::SetOpen(bool NewOpen) { Open = NewOpen; }
	
private:
	UStaticMeshComponent*Door;
	float StartYaw;
	bool Open;

	
	
};
