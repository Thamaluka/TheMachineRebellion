// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "InimigoMedium.generated.h"

UCLASS()
class JOGO_API AInimigoMedium : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInimigoMedium();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	
};
