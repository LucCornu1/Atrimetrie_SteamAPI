// CORNU Luc


#include "GameFramework/CharacterMovementComponent.h"
#include "NetworkPlayerStart.h"
#include "PlayerPlane.h"

// Sets default values
APlayerPlane::APlayerPlane()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	Maneuvrability = 1.f;

}

// Called when the game starts or when spawned
void APlayerPlane::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	World = GetWorld();
	MeshComponent = GetMesh();
	CurrentController = Cast<APlayerController>(GetController());
	/*GameMode = Cast<AMultiplayerGameMode>(GetWorld()->GetAuthGameMode());

	GameMode->SetTargetSpawner(ESpawnerType::SPAWNER_RESISTANT);*/
	
}

// Called every frame
void APlayerPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward(1.f);

}

// Called to bind functionality to input
void APlayerPlane::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Right", this, &APlayerPlane::MoveRight);

}


// Added
void APlayerPlane::MoveRight(float AxisValue)
{
	if (!IsValid(MovementComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("No 'MovementComponent' found"));
		return;
	}

	if (!IsValid(MeshComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("No 'MeshComponent' found"));
		return;
	}

//	UE_LOG(LogTemp, Warning, TEXT("Some warning message"));
	MovementComponent->MaxFlySpeed = FMath::Lerp(2000.f, 1800.f, AxisValue);

	CTS_Rotation(AxisValue);
	MeshComponent->SetRelativeRotation(Rotation, true, false);

	TurnRight(AxisValue);

}

void APlayerPlane::TurnRight(float AxisValue)
{
	if (!IsValid(CurrentController))
	{
		UE_LOG(LogTemp, Error, TEXT("No 'Controller' found"));
		return;
	}

	CurrentController->InputYawScale = FMath::Abs(RollAngle) / 100;

	AddControllerYawInput(AxisValue);

}

void APlayerPlane::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);

}


// Replication
void APlayerPlane::CTS_Rotation_Implementation(float AxisValue)
{
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("No 'ZA WARUDO' found"));
		return;
	}

	RollAngle = FMath::FInterpTo(RollAngle, FMath::Lerp(0.f, 45.f, AxisValue), World->GetDeltaSeconds(), Maneuvrability);
	Rotation = FRotator(0.f, 0.f, RollAngle);
	
}

void APlayerPlane::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerPlane, Rotation);
	DOREPLIFETIME(APlayerPlane, RollAngle);

}
