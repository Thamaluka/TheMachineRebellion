// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Towers.h"
#include "ProjectileActor.h"
#include "Boss.h"


// Sets default values
ATowers::ATowers(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Laser"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshTower(TEXT("StaticMesh'/Game/Boss/MaquinaMae_pilar.MaquinaMae_pilar'"));
	if (MeshTower.Succeeded()) {
		Tower->SetStaticMesh(MeshTower.Object);
	}
	Tower->SetWorldScale3D(FVector(0.25f,0.25f,1.0f));
	Tower->SetCollisionProfileName("BlockAll");

	EletricPart = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EnergyPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleSys(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Genno/P_Genno_Weapon_Lightning_01.P_Genno_Weapon_Lightning_01'"));
	if (ParticleSys.Succeeded()) {
			EletricPart->SetTemplate(ParticleSys.Object);
		}
	EletricPart->SetWorldLocation(FVector(-2120.0f, 0.0f, 100.0f));
	EletricPart->SetWorldScale3D(FVector(4.5f,4.5f,4.5f));
	EletricPart->SetWorldRotation(FRotator(-90.0f,0.0f,0.0f));
	EletricPart->SetupAttachment(Tower);
	EletricPart->bAutoActivate = true;

		Tower->OnComponentBeginOverlap.AddDynamic(this, &ATowers::OnOverlapBegin);

	bReplicates = true;
	bReplicateMovement = true;

}

// Called when the game starts or when spawned
void ATowers::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATowers::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATowers::SetLife(int NewLife) {
	Life = NewLife;
}


int ATowers::GetLife() {
	return Life;
}

void ATowers::OnDeath() {
	if (Life <= 0) {
		TArray<AActor*> Torres;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoss::StaticClass(), Torres);
		for (int i = 0; i < Torres.Num(); i++) {
			ABoss* Boss = Cast<ABoss>(Torres[0]);
			Boss->SetTorres(Boss->GetTorres()-1);
		}
		Destroy();
	}
}


void ATowers::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) &&(OtherComp != nullptr) && (OtherActor->IsA(AProjectileActor::StaticClass()))) {
		AProjectileActor* Projectile = Cast<AProjectileActor>(OtherActor);
		Life=Life -50;
		}
	}
