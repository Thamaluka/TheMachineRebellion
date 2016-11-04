// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "InimigoBot.h"
#include "Doctor.h"
#include "ProjectileActor.h"
#include "Cyborg.h"


// Sets default values
AInimigoBot::AInimigoBot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");

	ConstructorHelpers::FObjectFinder<USkeletalMesh>SkeletalMesh(TEXT("SkeletalMesh'/Game/Models/Inimigos/InimigoBot/bot.bot'"));
	if (SkeletalMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
	}
	GetMesh()->SetWorldLocation(FVector(-310.0f, -180.0f, -80.0f));
	GetMesh()->SetWorldScale3D(FVector(0.9f, 0.9f, 0.9f));
	GetMesh()->SetCollisionProfileName("BlockAll");

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AInimigoBot::OnOverlapBegin);
	GetMesh()->OnComponentHit.AddDynamic(this, &AInimigoBot::OnHit);

	ConstructorHelpers::FObjectFinder<UAnimSequence>
		AnimLoad(TEXT("AnimSequence'/Game/Models/Inimigos/InimigoBot/bot_Anim.bot_Anim'"));
	if (AnimLoad.Succeeded()) {
		WalkAnim = AnimLoad.Object;
	}


	Dead = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DeadPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleSys(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/combat/P_RoboGolem_Death_Fire_01.P_RoboGolem_Death_Fire_01'"));
	if (ParticleSys.Succeeded()) {
		Dead->SetTemplate(ParticleSys.Object);
	}

	Dead->SetupAttachment(GetMesh());
	Dead->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	Dead->bAutoActivate = false;

	Death = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DeathPart"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>Particle(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/combat/P_Impact_RoboGolem_Gib_Fire.P_Impact_RoboGolem_Gib_Fire'"));
	if (Particle.Succeeded()) {
		Death->SetTemplate(Particle.Object);
	}
	Death->SetupAttachment(GetMesh());
	Death->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	Death->bAutoActivate = false;


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
	Dead->ToggleActive();
	if (InimigoPeqLife <= 0) {
		Death->ToggleActive();
		Destroy();
	}
}

void AInimigoBot::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor->IsA(ADoctor::StaticClass()))) {

		ADoctor* Doctor = Cast<ADoctor>(OtherActor);
		Doctor->SetLife(Doctor->GetLife() - DamageAmount);
		Doctor->OnDeath();
		UE_LOG(LogTemp, Warning, TEXT("Life = %d"), InimigoPeqLife);

	}else if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor->IsA(ACyborg::StaticClass()))) {
		ACyborg* Cyborg = Cast<ACyborg>(OtherActor);
		Cyborg->SetLife(Cyborg->GetLife() - DamageAmount);
		Cyborg->OnDeath();
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