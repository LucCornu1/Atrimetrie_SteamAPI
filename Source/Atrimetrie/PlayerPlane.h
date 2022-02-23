// CORNU Luc

#pragma once

#include "Net/UnrealNetwork.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "MultiplayerGameMode.h"
#include "PlayerPlane.generated.h"

UCLASS()
class ATRIMETRIE_API APlayerPlane : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


// Added
private:
	UCharacterMovementComponent* MovementComponent;
	UWorld* World;
	USkeletalMeshComponent* MeshComponent;
	APlayerController* CurrentController;
	AMultiplayerGameMode* GameMode;

protected:
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "PlaneMovement")
		FRotator Rotation;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "PlaneMovement")
		float RollAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlaneAttribute")
		float Maneuvrability;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		virtual void MoveRight(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		virtual void TurnRight(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		virtual void MoveForward(float AxisValue);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ServerSide")
		void CTS_Rotation(float AxisValue);

};
