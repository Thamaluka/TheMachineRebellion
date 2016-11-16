// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Escudo.h"


// Sets default values
AEscudo::AEscudo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>Shield(TEXT("StaticMesh'/Game/InfinityBladeEffects/Effects/FX_Meshes/Skills/SM_Shield_Rings_02.SM_Shield_Rings_02'"));
	if (Shield.Succeeded()) {
		MeshComp->SetStaticMesh(Shield.Object);
	}


	ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/InfinityBladeEffects/Effects/FX_Materials/Mobile/M_ShieldMesh_mobile.M_ShieldMesh_mobile'"));
	if (Material.Succeeded()) {
		MeshComp->SetMaterial(0, Material.Object);
	}
	MeshComp->SetCollisionProfileName("BlockAllDynamic");
	RootComponent = MeshComp;

	bReplicates = true;
bReplicateMovement = true;
}

// Called when the game starts or when spawned
void AEscudo::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEscudo::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AEscudo::DestroyEscudo() {
	Destroy();
}
