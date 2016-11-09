// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Towers.generated.h"

UCLASS()
class JOGO_API ATowers : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATowers();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SetLife(int NewLife);
	int GetLife();
	void OnDeath();

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent*Tower;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* EletricPart;

	int Life = 500;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
