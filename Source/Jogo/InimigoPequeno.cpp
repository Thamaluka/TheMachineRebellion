// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "InimigoPequeno.h"
#include "Doctor.h"
#include "ProjectileActor.h"


// Sets default values
AInimigoPequeno::AInimigoPequeno()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	Root->bGenerateOverlapEvents = true;
	Root->OnComponentBeginOverlap.AddDynamic(this, &AInimigoPequeno::OnOverlapBegin);

	RootComponent = Root;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName("BlockAll");
	MeshComp->AttachTo(RootComponent);

	MeshComp->OnComponentHit.AddDynamic(this, &AInimigoPequeno::OnHit);

}

// Called when the game starts or when spawned
void AInimigoPequeno::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInimigoPequeno::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
void AInimigoPequeno::SetInimigoPeqLife(int NewLife) {
	InimigoPeqLife = NewLife;
}


int AInimigoPequeno::GetInimigoPeqLife() {
	return InimigoPeqLife;
}

void AInimigoPequeno::InimigoPeqDeath() {
	if (InimigoPeqLife<=0) {
		Destroy();
	}
}

void AInimigoPequeno::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor->IsA(ADoctor::StaticClass()))) {

		ADoctor* Doctor = Cast<ADoctor>(OtherActor);

		Doctor->SetLife(Doctor->GetLife()- DamageAmount);
		Doctor->OnDeath();
	}

}

void AInimigoPequeno::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) &&
		(OtherComp != nullptr) && (OtherActor->IsA(AProjectileActor::StaticClass()))) {
		AProjectileActor* Projectile = Cast<AProjectileActor>(OtherActor);
		InimigoPeqLife -= 50;
		Projectile->DestroyProjectile();	
		InimigoPeqDeath();

		UE_LOG(LogTemp, Warning, TEXT("Life = %d"), InimigoPeqLife);
	}
}
