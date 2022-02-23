// CORNU Luc

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerGameMode.generated.h"

class ANetworkPlayerStart;

/**
 * 
 */
UCLASS()
class ATRIMETRIE_API AMultiplayerGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	AMultiplayerGameMode();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


// Added
private:
	enum ESpawnerType TargetSpawner;

protected:
	TArray<ANetworkPlayerStart*> PlayerStartArray;

public:
	void SetTargetSpawner(enum ESpawnerType Target) { TargetSpawner = Target; };

	enum ESpawnerType GetNext(enum ESpawnerType Current);

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

};
