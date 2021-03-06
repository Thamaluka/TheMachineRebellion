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

	FORCEINLINE int GetId() const { return Id; }
	FORCEINLINE void ADoctor::SetId(int NewId) { Id = NewId; }

	void SetLife(int NewLife);
	int GetLife();

	void OnDeath();

	void SetPower(int NewPower);
	int GetPower();

	void Cura();
	void Bomba();
	void Quimico();
	void Nitrogenio();
	void OnBottom();

	void Pause();

	TSubclassOf<class UUserWidget> UserWidget;

	TSubclassOf<class UUserWidget> GameOver;

	USoundCue* HitSound;
	UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere)
		UAnimSequence* AnimFire;


	UPROPERTY(EditAnywhere)
		UAnimSequence* AnimArea;


	UPROPERTY(EditAnywhere)
		UAnimSequence* AnimCure;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

	UPROPERTY(EditAnywhere)
		UArrowComponent* ArrowComp;

	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionComp;

	FVector StartPlayer;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* Particle;
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* NitrogenioPart;
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* QuimicoPart;
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* CuraPart;

	int Life;
	int Power;
	bool SuperPower;
	int Id=2;
	bool Tiro;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		UFUNCTION()
		        void OnHit(UPrimitiveComponent* HitComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
