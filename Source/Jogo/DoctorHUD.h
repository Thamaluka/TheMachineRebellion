// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "DoctorHUD.generated.h"

/**
 *
 */
UCLASS()
class JOGO_API ADoctorHUD : public AHUD
{
	GENERATED_BODY()

public:
	ADoctorHUD();


	UPROPERTY(EditAnywhere)
		UFont*HUDFont;
	UPROPERTY(EditAnywhere)
		UTexture2D* MyTexture;
	UPROPERTY(EditAnywhere)
		UTexture2D* PowerTexture;

	virtual void DrawHUD() override;



};
