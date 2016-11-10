// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Boss.h"
#include "Towers.h"
#include "InimigoBot.h"
#include "InimigoMedium.h"



// Sets default values
ABoss::ABoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshBoss(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (MeshBoss.Succeeded()) {
		Mesh->SetStaticMesh(MeshBoss.Object);
	}
	Mesh->SetWorldScale3D(FVector(2.0f,4.5f,2.0f));

	CountdownTime = 100.0f;

}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this,&ABoss::TimerManager, 1.0f, true);

}

// Called every frame
void ABoss::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	ABoss::TimerManager();
	if (CountdownTime <= 0) {
		CountdownTime = 100.0f;
	}

	}



void ABoss::SetLife(int NewLife) {
	Life = NewLife;
}


int ABoss::GetLife() {
	return Life;
}

void ABoss::OnDeath() {
	if (Life <= 0 && Torres<=0) {
		Destroy();
	}
}


void ABoss::SetTorres(int NewTorres) {
	Torres = NewTorres;
}


int ABoss::GetTorres() {
	return Torres;
}

 void ABoss::TimerManager(){
	 CountdownTime -= 1.0f;
	if (CountdownTime <= 0.0f) {
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		ABoss::SpawnEnemies();
	}
 }

 void ABoss::SpawnEnemies() {
	 FActorSpawnParameters SpawnParameters;
	 UWorld* const World = GetWorld();
	 if (World != nullptr) {
		 FRotator Rotation = RootComponent->GetComponentRotation();
		 AInimigoBot* Bot = World->SpawnActor<AInimigoBot>(GetActorLocation(), Rotation, SpawnParameters);
		 if (Bot != nullptr) {
			 UE_LOG(LogTemp, Warning, TEXT("Spawn OK!"));
		 }
	 }
 }
