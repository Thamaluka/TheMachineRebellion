// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "InimigoMedium.h"


// Sets default values
AInimigoMedium::AInimigoMedium()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInimigoMedium::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInimigoMedium::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AInimigoMedium::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

