// CORNU Luc

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "NetworkPlayerStart.generated.h"

UENUM(BlueprintType)
enum class ESpawnerType : uint8
{
	SPAWNER_PLANE = 0 UMETA(DisplayName = "PlaneSpawner"),
	SPAWNER_RESISTANT = 1 UMETA(DisplayName = "ResSpawner"),
	SPAWNER_MECHA = 2 UMETA(DisplayName = "MechSpawner")
};

/**
 * 
 */
UCLASS()
class ATRIMETRIE_API ANetworkPlayerStart : public APlayerStart
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


// Added
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnerType")
		ESpawnerType Type;
};
