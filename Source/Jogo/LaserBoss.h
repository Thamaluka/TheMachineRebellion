// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "LaserBoss.generated.h"

UCLASS()
class JOGO_API ALaserBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALaserBoss();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

void DestroyLaserBoss();



private:
	UPROPERTY(EditAnywhere)
		UShapeComponent* Root;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* LaserPart;

	FTimerHandle CountdownTimerHandle;
	float CountdownTime;
	void TimerManager();

};
