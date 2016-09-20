// Fill out your copyright notice in the Description page of Project Settings.

#include "Jogo.h"
#include "Doctor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "CuraPower.h"
#include "ProjectileActor.h"


// Sets default values
ADoctor::ADoctor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; 

										
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; 

															
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	/*
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	*/
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	/*
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetHiddenInGame(true);
	ArrowComp->ArrowSize = 2.0f;
	ArrowComp->AttachTo(RootComponent);
	*/
	Life = 3000;
	Power = 0;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ADoctor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoctor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

// Called to bind functionality to input
void ADoctor::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Cura", IE_Pressed, this, &ADoctor::Cura);
	InputComponent->BindAction("Bomba", IE_Pressed, this, &ADoctor::Bomba);
	InputComponent->BindAction("Quimico", IE_Pressed, this, &ADoctor::Quimico);
	InputComponent->BindAction("Nitrogenio", IE_Pressed, this, &ADoctor::Nitrogenio);

}

void ADoctor::SetLife(int NewLife) {
	Life = NewLife;
}


int ADoctor::GetLife() {
	return Life;
}

void ADoctor::OnDeath() {
	if (Life <= 0) {
		FVector InitialLocation(StartPlayer);
		Life = 3000;
		SetActorLocation(InitialLocation);
	}
}




//Energia
void ADoctor::SetPower(int NewPower) {
	if (NewPower > 0) {
		NewPower = NewPower + Power;
	}
	else {
		Power = NewPower;
	}
}

int ADoctor::GetPower() {
	if (Power > 3000) {
		SuperPower = true;
	}
	return Power;
}




//Poderes pelas teclas Q,W,E e R
void ADoctor::Cura() {
	if (Proj == nullptr) {
		FActorSpawnParameters SpawnParameters;
		UWorld* World = GetWorld();
		if (World != nullptr) {
			FRotator Rotation = RootComponent->GetComponentRotation();
			Proj = World->SpawnActor<ACuraPower>(GetActorLocation(), Rotation, SpawnParameters);
			if (Proj != nullptr) {
				//UE_LOG(LogTemp, Warning, TEXT("Spawn OK!"));
			}
		}
		if(Life+120 < 3000)
		Life = Life + 120;
	}
}

void ADoctor::Bomba() {
	FActorSpawnParameters SpawnParameters;
	UWorld* World = GetWorld();
	if (World != nullptr) {
		FRotator Rotation = RootComponent->GetComponentRotation();
		AProjectileActor* Proj = World->SpawnActor<AProjectileActor>
			(GetActorLocation(), Rotation,
				SpawnParameters);
		if (Proj != nullptr) {
		//	UE_LOG(LogTemp, Warning, TEXT("Spawn OK!"));
		}
	}

}
void ADoctor::Quimico() {
	//if (Proj == nullptr) {
		FActorSpawnParameters SpawnParameters;
		UWorld* World = GetWorld();
		if (World != nullptr) {
			FRotator Rotation = RootComponent->GetComponentRotation();
			Proj = World->SpawnActor<ACuraPower>(GetActorLocation(), Rotation, SpawnParameters);
			if (Proj != nullptr) {
				//UE_LOG(LogTemp, Warning, TEXT("Spawn OK!"));
			}
		}
		
	//}
}
void ADoctor::Nitrogenio() {
	//if(SuperPower)
	//if (Proj == nullptr) {
		FActorSpawnParameters SpawnParameters;
		UWorld* World = GetWorld();
		if (World != nullptr) {
			FRotator Rotation = RootComponent->GetComponentRotation();
			Proj = World->SpawnActor<ACuraPower>(GetActorLocation(), Rotation, SpawnParameters);
			if (Proj != nullptr) {
				//UE_LOG(LogTemp, Warning, TEXT("Spawn OK!"));
			}
		}

	//}

}

