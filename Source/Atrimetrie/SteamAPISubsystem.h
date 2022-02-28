// CORNU Luc

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Net/UnrealNetwork.h"
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
	CSteamID LobbyId;

	FDelegateHandle TickDelegateHandle;
	
public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	// Called every frame
	bool Tick(float DeltaTime);


// Friends
protected:
	int Size = 1;
	CSteamID* SteamFriendArray = new CSteamID[Size];

protected:
	UFUNCTION(BlueprintCallable, Category = "SteamAPI|Friends")
		void SetSteamFriendArray();


// Lobbies
private:
	STEAM_CALLBACK(USteamAPISubsystem, OnLobbyEntered, LobbyEnter_t);

	void OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure);
	CCallResult<USteamAPISubsystem, LobbyCreated_t> m_LobbyCreatedCallResult;

public:
	void InviteFriendA(CSteamID UserId);

	UFUNCTION(BlueprintCallable, Category = "SteamAPI|Lobby")
		void JoinFriendLobby();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "SteamAPI|Lobby")
		void CreateFriendLobby(int nMaxMembers);
};
