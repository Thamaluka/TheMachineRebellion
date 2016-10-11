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
	ConstructorHelpers::FObjectFinder<UStaticMesh>AlavancaMesh(TEXT("StaticMesh'/Game/StarterContent/Props/SM_MatPreviewMesh_02.SM_MatPreviewMesh_02'"));
	if (AlavancaMesh.Succeeded()) {
		MeshComp->SetStaticMesh(AlavancaMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/StarterContent/Materials/NewMaterial.NewMaterial'"));
	if (Material.Succeeded()) {
		MeshComp->SetMaterial(0, Material.Object);
	}

	MeshComp->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));
	RootComponent = MeshComp;

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
			Material = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss'")));
		}
		else {
			Laser->SetOpen(true);
			Material = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/StarterContent/Materials/NewMaterial.NewMaterial'")));
		}
		if (Material != nullptr) {
			MeshComp->SetMaterial(0, Material);
		}
	}
}

