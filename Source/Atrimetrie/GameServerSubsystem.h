// CORNU Luc

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "SteamAPISubsystem.h"

#include "GameServerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ATRIMETRIE_API UGameServerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	// Called every frame
	void Tick(float DeltaTime);
};
