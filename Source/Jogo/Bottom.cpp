// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Bottom.h"
#include "Laser.h"


// Sets default values
ABottom::ABottom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>AlavancaMesh(TEXT("StaticMesh'/Game/cidade/painel.painel'"));
	if (AlavancaMesh.Succeeded()) {
		MeshComp->SetStaticMesh(AlavancaMesh.Object);
	}

	if(BottomNum==1){
		ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/Materials/Cyborg.Cyborg'"));
		if (Material.Succeeded()) {
			MeshComp->SetMaterial(0, Material.Object);
		}
	}else if(BottomNum==2){
		ConstructorHelpers::FObjectFinder<UMaterial>Mater(TEXT("Material'/Game/Materials/Doctor.Doctor'"));
		if (Mater.Succeeded()) {
			MeshComp->SetMaterial(0, Mater.Object);
		}
	}


	MeshComp->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	RootComponent = MeshComp;

	bReplicates = true;
	bReplicateMovement = true;

}

// Called when the game starts or when spawned
void ABottom::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABottom::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABottom::OnPressed() {
	TArray<AActor*>Lasers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALaser::StaticClass(), Lasers);
	UMaterial*Material;

	if (Lasers.Num() > 0) {
		ALaser*Laser = Cast<ALaser>(Lasers[0]);

		if (Laser->IsOpen()) {
			Laser->SetOpen(false);
			Material = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/Materials/Acess.Acess'")));
		}
		else {
			Laser->SetOpen(true);
			Laser->Destroy();
			if(BottomNum==1 && Laser->GetLaserNum()== BottomNum){
					Material = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/Materials/Cyborg.Cyborg'")));
			}else if(BottomNum==2 && Laser->GetLaserNum() == BottomNum){
					Material = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/Materials/Doctor.Doctor'")));
			}
		}
	}
}
