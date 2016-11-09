// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Towers.h"


// Sets default values
ATowers::ATowers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATowers::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATowers::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

