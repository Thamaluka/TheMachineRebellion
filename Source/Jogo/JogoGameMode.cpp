// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "JogoGameMode.h"
#include "MyPlayerController.h"
#include "Doctor.h"
#include "Cyborg.h"
#include "CyborgHUD.h"
#include "DoctorHUD.h"

AJogoGameMode::AJogoGameMode() {
	PlayerControllerClass = AMyPlayerController::StaticClass();

	DefaultPawnClass = ACyborg::StaticClass();
	HUDClass = ACyborgHUD::StaticClass();

	//DefaultPawnClass = ADoctor::StaticClass();
//	HUDClass = ADoctorHUD::StaticClass();

}
