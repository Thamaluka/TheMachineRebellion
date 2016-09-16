// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JOGO_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

private:


	uint32 bMoveToMouseCursor : 1;

	
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	void MoveToMouseCursor();
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	void SetNewMoveDestination(const FVector DestLocation);
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	
	
	
};
