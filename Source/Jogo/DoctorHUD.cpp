// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "DoctorHUD.h"
#include "Engine/Font.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "Doctor.h"

ADoctorHUD::ADoctorHUD(){

	static ConstructorHelpers::FObjectFinder<UFont>Font(TEXT("Font'/Engine/EngineFonts/TinyFont.TinyFont'"));

	if (Font.Succeeded()) {
		HUDFont = Font.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D>Texture(TEXT("Texture2D'/Game/Materials/Green_Texture.Green_Texture'"));
	if (Texture.Succeeded()) {
		MyTexture = Texture.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D>TextureP(TEXT("Texture2D'/Game/Materials/AzulY.AzulY'"));
	if (TextureP.Succeeded()) {
		PowerTexture = TextureP.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D>TextureHud(TEXT("Texture2D'/Game/Materials/HUDTest2.HUDTest2'"));
	if (TextureHud.Succeeded()) {
		HUDTexture = TextureHud.Object;
	}


}

void ADoctorHUD::DrawHUD() {
	Super::DrawHUD();

	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX,
		Canvas->SizeY);

	ADoctor* Doctor = Cast<ADoctor>(UGameplayStatics::GetPlayerPawn(this, 0));

//	FString LifeString = FString::Printf(TEXT("Life: %d"),Doctor->GetLife());
//	DrawText(LifeString, FColor::Red, 50, 50, HUDFont);

	DrawTextureSimple(HUDTexture, 50, 50, 1.0f, false);

	DrawTexture(MyTexture, 150, 109, Doctor->GetLife()/33,
	9, 0, 0, Doctor->GetLife()/20,
	5, FLinearColor::White,
	EBlendMode::BLEND_Translucent, 1.0f, false, 0.0f,
	FVector2D::ZeroVector);

	DrawTexture(PowerTexture, 150, 91, Doctor->GetPower() / 40,
	9, 0, 0, Doctor->GetPower() / 10,
	9, FLinearColor::White,
	EBlendMode::BLEND_Translucent, 1.0f, false, 0.0f,
	FVector2D::ZeroVector);
}
