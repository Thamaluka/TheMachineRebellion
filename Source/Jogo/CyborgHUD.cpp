// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "CyborgHUD.h"
#include "Engine/Font.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "Cyborg.h"

ACyborgHUD::ACyborgHUD(){

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


}

void ACyborgHUD::DrawHUD() {
	Super::DrawHUD();

	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX,
		Canvas->SizeY);

	ACyborg* Cyborg = Cast<ACyborg>(UGameplayStatics::GetPlayerPawn(this, 0));

	FString LifeString = FString::Printf(TEXT("Life: %d"),Cyborg->GetLife());
	DrawText(LifeString, FColor::Red, 50, 50, HUDFont);

	FString PowerString = FString::Printf(TEXT("Life: %d"), Cyborg->GetPower());
	DrawText(PowerString, FColor::Red, 50, 50, HUDFont);

	DrawTexture(MyTexture, 50, 50, Cyborg->GetLife()/20,
	20, 0, 0, Cyborg->GetLife()/20,
	20, FLinearColor::White,
	EBlendMode::BLEND_Translucent, 1.0f, false, 0.0f,
	FVector2D::ZeroVector);

	DrawTexture(PowerTexture, 80, 80, Cyborg->GetPower() / 10,
	20, 0, 0, Cyborg->GetPower() / 10,
	20, FLinearColor::White,
	EBlendMode::BLEND_Translucent, 1.0f, false, 0.0f,
	FVector2D::ZeroVector);
}
