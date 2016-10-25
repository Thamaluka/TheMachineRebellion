// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bottom.generated.h"

UCLASS()
class JOGO_API ABottom : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABottom();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	FORCEINLINE int GetBottomNum() const { return BottomNum; }
 

	void OnPressed();

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent*MeshComp;
	UPROPERTY(EditAnywhere)
	int BottomNum;

};
