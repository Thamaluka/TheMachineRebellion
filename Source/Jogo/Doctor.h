// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Doctor.generated.h"

UCLASS()
class JOGO_API ADoctor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADoctor();

	
	virtual void BeginPlay() override;
	
	
	virtual void Tick( float DeltaSeconds ) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;
};
