// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "InimigoBot.generated.h"

UCLASS()
class JOGO_API AInimigoBot : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInimigoBot();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	FORCEINLINE UStaticMeshComponent* GetMeshComp()const { return MeshComp; }

	void SetInimigoPeqLife(int NewLife);
	int GetInimigoPeqLife();
	void InimigoPeqDeath();

private:
	UPROPERTY(EditAnywhere)
		UShapeComponent* Root;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere)
		float DamageAmount = 250;
	UPROPERTY(EditAnywhere)
		int InimigoPeqLife = 500;


	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* Dead;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* Death;


	USoundCue* DestroySound;
	UAudioComponent* AudioComp;



	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
