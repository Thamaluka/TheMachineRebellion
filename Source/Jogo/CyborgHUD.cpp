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

	static ConstructorHelpers::FObjectFinder<UTexture2D>TextureHud(TEXT("Texture2D'/Game/Materials/HUDTest2.HUDTest2'"));
	if (TextureHud.Succeeded()) {
		HudCyborg = TextureHud.Object;
	}


}

void ACyborgHUD::DrawHUD() {
	Super::DrawHUD();

	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX,
		Canvas->SizeY);

	ACyborg* Cyborg = Cast<ACyborg>(UGameplayStatics::GetPlayerPawn(this, 0));

	FString LifeString = FString::Printf(TEXT("Life: %d"),Cyborg->GetLife());
	DrawText(LifeString, FColor::Red, 30, 50, HUDFont);

	FString PowerString = FString::Printf(TEXT("Power: %d"), Cyborg->GetPower());
	DrawText(PowerString, FColor::Black, 40, 50, HUDFont);

DrawTextureSimple(HudCyborg, 50, 50, 1.0f, false);

	DrawTexture(MyTexture, 150, 109, Cyborg->GetLife()/33,
	9, 0, 0, Cyborg->GetLife()/33,
	5, FLinearColor::White,
	EBlendMode::BLEND_Translucent, 1.0f, false, 0.0f,
	FVector2D::ZeroVector);

//														x,y
	DrawTexture(PowerTexture, 150, 91, Cyborg->GetPower() / 40,
	9, 0, 0, Cyborg->GetPower() / 40,
	9, FLinearColor::White,
	EBlendMode::BLEND_Translucent, 1.0f, false, 0.0f,
	FVector2D::ZeroVector);



}
