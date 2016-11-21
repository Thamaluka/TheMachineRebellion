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

	static ConstructorHelpers::FObjectFinder<UTexture2D>TextureHud(TEXT("Texture2D'/Game/Blueprints/Menu/main_button_1.main_button_1'"));
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

	DrawTextureSimple(HUDTexture, 50, 50, 0.5f, false);

	DrawTexture(MyTexture, 147, 88, Doctor->GetLife()/25.8f,
	7, 0, 0, Doctor->GetLife()/25.8f,
	5, FLinearColor::White,
	EBlendMode::BLEND_Translucent, 1.0f, false, 0.0f,
	FVector2D::ZeroVector);

	DrawTexture(PowerTexture, 147, 105, Doctor->GetPower() / 15.3f,
	7, 0, 0, Doctor->GetPower() / 15.3f,
	5, FLinearColor::White,
	EBlendMode::BLEND_Translucent, 1.0f, false, 0.0f,
	FVector2D::ZeroVector);
	

	
}
