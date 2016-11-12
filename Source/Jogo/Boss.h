// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Boss.generated.h"

UCLASS()
class JOGO_API ABoss : public AActor
{
	GENERATED_BODY()





public:
	// Sets default values for this actor's properties
	ABoss();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	TSubclassOf<class AInimigoBot> MyAiBlueprint;
	TSubclassOf<class AInimigoMedium>MyAi;


	void SetLife(int NewLife);
	int GetLife();

	void SetTorres(int NewTorres);
	int GetTorres();


	void OnDeath();
	void SpawnEnemies();
	void SpawnEnemiesMed();

private:


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent*Mesh;
	int Life=1000;
	int Torres=4;

	FTimerHandle CountdownTimerHandle;
	float CountdownTime;
	void TimerManager();




};
