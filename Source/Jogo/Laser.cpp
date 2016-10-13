// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Laser.h"
#include "Doctor.h"


// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Laser = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Laser"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshLaser(TEXT("StaticMesh'/Game/StarterContent/Architecture/Pillar_50x500.Pillar_50x500'"));
	if (MeshLaser.Succeeded()) {
		Laser->SetStaticMesh(MeshLaser.Object);
	}
	Laser->SetRelativeScale3D(FVector(1.0f,1.0f,1.0f));

	Laser->OnComponentHit.AddDynamic(this, &ALaser::OnHit);
	Laser->OnComponentBeginOverlap.AddDynamic(this, &ALaser::OnOverlapBegin);
	

	ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/InfinityBladeEffects/LightsPavement.LightsPavement'"));
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

void ALaser::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor->IsA(ADoctor::StaticClass()))) {

		ADoctor* Doctor = Cast<ADoctor>(OtherActor);
		Doctor->SetLife(Doctor->GetLife() - 50);
		Doctor->OnDeath();
		UE_LOG(LogTemp, Warning, TEXT("Life = %d"), Doctor->GetLife());
	}

}

void ALaser::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) &&
		(OtherComp != nullptr) && (OtherActor->IsA(ADoctor::StaticClass()))) {
	
		ADoctor* Doctor = Cast<ADoctor>(OtherActor);
		Doctor->SetLife(Doctor->GetLife() - 50);
		Doctor->OnDeath();
		UE_LOG(LogTemp, Warning, TEXT("Life = %d"), Doctor->GetLife());
	}
}