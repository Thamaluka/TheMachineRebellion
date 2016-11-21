// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Boss.h"
#include "Towers.h"
#include "InimigoBot.h"
#include "InimigoMedium.h"
#include "LaserBoss.h"

#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"



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

	static ConstructorHelpers::FObjectFinder<UBlueprint>BotBlueprint(TEXT("Blueprint'/Game/Blueprints/EnemyForlder/InimigoBotAnim/AIInimigoBot.AIInimigoBot'"));
	if (BotBlueprint.Object) {
		MyAiBlueprint = (UClass*)BotBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint>MedBlueprint(TEXT("Blueprint'/Game/Blueprints/EnemyForlder/AIInimigoMedium.AIInimigoMedium'"));
	if (MedBlueprint.Object) {
		MyAi = (UClass*)MedBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint>LaserBlueprint(TEXT("Blueprint'/Game/Blueprints/EnemyForlder/MyLaserBoss.MyLaserBoss'"));
	if (LaserBlueprint.Object) {
		MyBossLaser = (UClass*)LaserBlueprint.Object->GeneratedClass;
	}

	ConstructorHelpers::FClassFinder<UUserWidget>Widget(TEXT("WidgetBlueprint'/Game/Blueprints/Decisao.Decisao_C'"));
	if (Widget.Succeeded()) {
		UserWidget = Widget.Class;
	}

	bReplicates = true;
	bReplicateMovement = true;

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
		TheEnd();
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
		SpawnEnemies();
		SpawnEnemiesMed();
		SpawnLaser();
	}
 }


 void ABoss::SpawnEnemies() {
	 int CountBot=3;
	 for (int i =0 ; i < CountBot; i++) {
	 	if(i <=CountBot){
			FActorSpawnParameters SpawnParameters;
			UWorld* const World = GetWorld();
			if (World != nullptr) {
				FRotator Rotation = RootComponent->GetComponentRotation();
				FVector Location = FVector(-580.0f,-240.0f,20.0f);
				FVector MedLocation = FVector(-600.0f,-200.0f,20.0f);
				AInimigoBot* Bot = World->SpawnActor<AInimigoBot>(MyAiBlueprint, Location, Rotation, SpawnParameters);

			}
		}
	 }
 }


 void ABoss::SpawnEnemiesMed() {
	 int CountBot=2;
	 for (int i =0 ; i < CountBot; i++) {
	 	if(i <=CountBot){
			FActorSpawnParameters SpawnParameters;
			UWorld* const World = GetWorld();
			if (World != nullptr) {
				FRotator Rotation = RootComponent->GetComponentRotation();
				FVector MedLocation = FVector(200.0f,-200.0f,20.0f);

				AInimigoMedium* Med = World->SpawnActor<AInimigoMedium>(MyAi,MedLocation,Rotation,SpawnParameters);
			}
		}
	 }
 }


 void ABoss::TheEnd(){
	 UWorld* World = GetWorld();
	 if (World != nullptr) {
		 APlayerController* PlayerController =
			 UGameplayStatics::GetPlayerController(World, 0);
		 if (PlayerController != nullptr && UserWidget != NULL) {
			 PlayerController->SetPause(true);
			 UUserWidget* UserW = UWidgetBlueprintLibrary::Create(World, UserWidget, PlayerController);
			 if (UserW != nullptr) {
				 UserW->AddToViewport();
				 PlayerController->bShowMouseCursor = true;
			 }
		 }
	 }
 }

 void ABoss::SpawnLaser(){
	 FActorSpawnParameters SpawnParameters;
	 UWorld* const World = GetWorld();
	 if (World != nullptr) {
		 FRotator Rotation = FRotator(0.0f,0.0f,-90.0f);
		 FVector LaserLocation = FVector(-800.0f,260.0f,20.0f);
		 ALaserBoss* Laser = World->SpawnActor<ALaserBoss>(MyBossLaser,LaserLocation,Rotation,SpawnParameters);
	 }
 }
