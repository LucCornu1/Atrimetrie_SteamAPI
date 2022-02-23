// CORNU Luc

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#pragma warning(push)
#pragma warning(disable: 4996)
#include "Steam/steam_api.h"
#pragma warning(pop)

#include "SteamAPISubsystem.generated.h"

#define RAW_APP_ID "480"

/**
 * 
 */
UCLASS()
class ATRIMETRIE_API USteamAPISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	static constexpr char* APP_ID = RAW_APP_ID;
	CSteamID MyId;
		
public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem
};
