// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "InimigoBot.h"
#include "Doctor.h"
#include "ProjectileActor.h"


// Sets default values
AInimigoBot::AInimigoBot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName("BlockAll");
	MeshComp->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SkeletalMesh(TEXT("SkeletalMesh'/Game/Models/SPIDBOT1.SPIDBOT1'"));
	if (SkeletalMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
	}
	GetMesh()->SetWorldLocation(FVector(-310.0f, -180.0f, -80.0f));
	GetMesh()->SetWorldScale3D(FVector(0.9f, 0.9f, 0.9f));

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AInimigoBot::OnOverlapBegin);
	GetMesh()->OnComponentHit.AddDynamic(this, &AInimigoBot::OnHit);

}

// Called when the game starts or when spawned
void AInimigoBot::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AInimigoBot::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AInimigoBot::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

int AInimigoBot::GetInimigoPeqLife() {
	return InimigoPeqLife;
}

void AInimigoBot::SetInimigoPeqLife(int NewLife) {
	InimigoPeqLife = NewLife;
}

void AInimigoBot::InimigoPeqDeath() {
	if (InimigoPeqLife <= 0) {
		Destroy();
	}
}

void AInimigoBot::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor->IsA(ADoctor::StaticClass()))) {

		ADoctor* Doctor = Cast<ADoctor>(OtherActor);
		Doctor->SetLife(Doctor->GetLife() - DamageAmount);
		Doctor->OnDeath();
		UE_LOG(LogTemp, Warning, TEXT("Life = %d"), InimigoPeqLife);
	}

}

void AInimigoBot::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) &&
		(OtherComp != nullptr) && (OtherActor->IsA(AProjectileActor::StaticClass()))) {
		AProjectileActor* Projectile = Cast<AProjectileActor>(OtherActor);
		InimigoPeqLife -= 50;
		Projectile->DestroyProjectile();
		InimigoPeqDeath();

	}
}
