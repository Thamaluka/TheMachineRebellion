// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Cyborg.generated.h"

UCLASS()
class JOGO_API ACyborg : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACyborg();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	void SetLife(int NewLife);
	int GetLife();

	void OnDeath();

	void SetPower(int NewPower);
	int GetPower();

	void Escudo();
	void Energy();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

		FVector StartPlayer;

		UPROPERTY(EditAnywhere)
		UParticleSystemComponent* EscudoPart;
		UPROPERTY(EditAnywhere)
		UParticleSystemComponent* EnergyPart;

		UPROPERTY(EditAnywhere)
		USphereComponent* CollisionComp;


		int Life;
		int Power;
		bool SuperPower;

};
