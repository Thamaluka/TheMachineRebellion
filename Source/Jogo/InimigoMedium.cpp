// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "InimigoMedium.h"
#include "Doctor.h"
#include "ProjectileActor.h"
#include "Cyborg.h"


// Sets default values
AInimigoMedium::AInimigoMedium()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	ConstructorHelpers::FObjectFinder<USkeletalMesh>SkeletalMesh(TEXT("SkeletalMesh'/Game/Models/Inimigos/InimigoGrande/robotMedium_Cylinder.robotMedium_Cylinder'"));
	if (SkeletalMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
	}
	GetMesh()->SetWorldLocation(FVector(-310.0f, -180.0f, -80.0f));
	GetMesh()->SetWorldScale3D(FVector(0.9f, 0.9f, 0.9f));
	GetMesh()->SetCollisionProfileName("BlockAll");

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AInimigoMedium::OnOverlapBegin);


}

// Called when the game starts or when spawned
void AInimigoMedium::BeginPlay()
{
	Super::BeginPlay();



}

// Called every frame
void AInimigoMedium::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	Tiro = false;

}

// Called to bind functionality to input
void AInimigoMedium::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

int AInimigoMedium::GetInimigoMedLife() {
	return InimigoMedLife;
}

void AInimigoMedium::SetInimigoMedLife(int NewLife) {
	InimigoMedLife= NewLife;
}

void AInimigoMedium::InimigoMedDeath() {
	if (InimigoMedLife<= 0) {
		Destroy();
	}
}


void AInimigoMedium::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) &&
		(OtherComp != nullptr) && (OtherActor->IsA(AProjectileActor::StaticClass()))) {
		if (!Tiro) {
			AProjectileActor* Projectile = Cast<AProjectileActor>(OtherActor);
			InimigoMedLife -= 50;
			Projectile->DestroyProjectile();
			InimigoMedDeath();
		}else 	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor->IsA(ADoctor::StaticClass()))) {
				ADoctor* Doctor = Cast<ADoctor>(OtherActor);
				Doctor->SetLife(Doctor->GetLife() - DamageAmount);
				Doctor->OnDeath();
				UE_LOG(LogTemp, Warning, TEXT("Life = %d"), InimigoMedLife);
			}
			else if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor->IsA(ACyborg::StaticClass()))) {
				ACyborg* Cyborg = Cast<ACyborg>(OtherActor);
				Cyborg->SetLife(Cyborg->GetLife() - DamageAmount);
				Cyborg->OnDeath();
			}
	}
}
