// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Laser.generated.h"

UCLASS()
class JOGO_API ALaser : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALaser();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	FORCEINLINE UStaticMeshComponent*GetLaser() const { return Laser; }
	FORCEINLINE bool IsOpen()const { return Open; }
	FORCEINLINE void ALaser::SetOpen(bool NewOpen) { Open = NewOpen; }
	FORCEINLINE int GetLaserNum() const { return LaserNum; }

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent*Laser;
	bool Open;
	
	UPROPERTY(EditAnywhere)
	int LaserNum;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
