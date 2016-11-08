// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "JogoGameMode.h"
#include "MyPlayerController.h"
#include "Doctor.h"
#include "Cyborg.h"
#include "CyborgHUD.h"

AJogoGameMode::AJogoGameMode() {
	PlayerControllerClass = AMyPlayerController::StaticClass();
	// DefaultPawnClass = ADoctor::StaticClass();
	DefaultPawnClass = ACyborg::StaticClass();

	HUDClass = ACyborgHUD::StaticClass();

}
