// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Laser.h"
#include "Doctor.h"
#include "Cyborg.h"


// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Laser = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Laser"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshLaser(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	if (MeshLaser.Succeeded()) {
		Laser->SetStaticMesh(MeshLaser.Object);
	}
	Laser->SetWorldScale3D(FVector(0.1f,0.1f,4.5f));
	Laser->SetWorldRotation(FRotator(0.0f,0.0f,90.0f));

	Laser->SetCollisionProfileName("BlockAllDynamic");

	Laser->OnComponentBeginOverlap.AddDynamic(this, &ALaser::OnOverlapBegin);

		ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/Materials/LightsPavement.LightsPavement'"));
		if (Material.Succeeded()) {
			Laser->SetMaterial(0, Material.Object);
		}


	RootComponent = Laser;

}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALaser::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector Scale = Laser->GetComponentScale();

	if (Open && Scale.Z>=0) {
		Scale.Z -= 0.3f;
		Laser->SetCollisionProfileName("NoCollision");
	}
	else if (!Open && Scale.Z<5.0f) {
		Scale.Z += 0.3f;
		Laser->SetCollisionProfileName("BlockAllDynamic");
	}

	Laser->SetWorldScale3D(Scale);


}


void ALaser::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) &&
		(OtherComp != nullptr) && (OtherActor->IsA(ADoctor::StaticClass()))) {

		ADoctor* Doctor = Cast<ADoctor>(OtherActor);
		Doctor->SetLife(Doctor->GetLife() - 50);
		Doctor->OnDeath();
		UE_LOG(LogTemp, Warning, TEXT("LifeDoctor = %d"), Doctor->GetLife());
	}else
	
	if ((OtherActor != nullptr) && (OtherActor != this) &&
		(OtherComp != nullptr) && (OtherActor->IsA(ACyborg::StaticClass()))) {

		ACyborg* Cyborg = Cast<ACyborg>(OtherActor);
		Cyborg->SetLife(Cyborg->GetLife() - 50);
		Cyborg->OnDeath();
		UE_LOG(LogTemp, Warning, TEXT("Life = %d"), Cyborg->GetLife());
	}
}
