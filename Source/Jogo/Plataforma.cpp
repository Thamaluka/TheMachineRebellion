// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Plataforma.h"
#include "Doctor.h"
#include "Cyborg.h"
#include "Laser.h"


// Sets default values
APlataforma::APlataforma()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
  ConstructorHelpers::FObjectFinder<UStaticMesh>AlavancaMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
  if (AlavancaMesh.Succeeded()) {
    MeshComp->SetStaticMesh(AlavancaMesh.Object);
  }
  MeshComp->SetWorldScale3D(FVector(1.0f,1.0f,0.05f));

  ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/Materials/Acess.Acess'"));
  if (Material.Succeeded()) {
    MeshComp->SetMaterial(0, Material.Object);
  }

  MeshComp->OnComponentBeginOverlap.AddDynamic(this, &APlataforma::OnOverlapBegin);
  MeshComp->OnComponentEndOverlap.AddDynamic(this, &APlataforma::OnOverlapEnd);
  MeshComp->SetCollisionProfileName("OverlapAll");
}

// Called when the game starts or when spawned
void APlataforma::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlataforma::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APlataforma::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor->IsA(ACyborg::StaticClass()))) {

    ACyborg* Cyborg = Cast<ACyborg>(OtherActor);

    UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/Materials/Doctor.Doctor'")));
			if (NewMaterial != nullptr) {
				MeshComp->SetMaterial(0, NewMaterial);
			}
  }

}

void APlataforma::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor->IsA(ACyborg::StaticClass()))) {

    ACyborg* Cyborg = Cast<ACyborg>(OtherActor);

    UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/Materials/Acess.Acess'")));
      if (NewMaterial != nullptr) {
        MeshComp->SetMaterial(0, NewMaterial);
      }

  }
}
