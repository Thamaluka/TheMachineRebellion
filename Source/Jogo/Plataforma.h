// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Plataforma.generated.h"

UCLASS()
class JOGO_API APlataforma : public AActor
{
	GENERATED_BODY()

		FORCEINLINE int GetPlataformaNum() const { return PlataformaNum; }

public:
	// Sets default values for this actor's properties
	APlataforma();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent*MeshComp;

	UPROPERTY(EditAnywhere)
	int PlataformaNum;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
