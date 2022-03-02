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


USTRUCT(BlueprintType)
struct FSteamFriend
{
	GENERATED_BODY()

private:
	FString SteamName = "";

public:
	CSteamID SteamID;

	FString GetNameFromID()
	{
		const char* FriendPersonaName = SteamFriends()->GetFriendPersonaName(SteamID);
		size_t n = strlen(FriendPersonaName);
		return FString(n, FriendPersonaName);
	}

	FString GetSteamName()
	{
		if (SteamName != GetNameFromID())
		{
			SteamName = GetNameFromID();
		}
		return SteamName;
	}
};


/**
 * 
 */
UCLASS()
class ATRIMETRIE_API USteamAPISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	static constexpr char* APP_ID = RAW_APP_ID;
	CSteamID LobbyId;
	bool bInvited = false;

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
	TArray<struct FSteamFriend*> FriendsArray;
	CSteamID JooJ;

public:
	UFUNCTION(BlueprintCallable, Category = "SteamAPI|Friends")
		void SetSteamFriendArray();


	// Lobbies
private:
	STEAM_CALLBACK(USteamAPISubsystem, OnLobbyEntered, LobbyEnter_t);
	STEAM_CALLBACK(USteamAPISubsystem, OnLobbyUpdate, LobbyChatUpdate_t);
	STEAM_CALLBACK(USteamAPISubsystem, OnPacketReceived, P2PSessionRequest_t);

	void OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure);
	CCallResult<USteamAPISubsystem, LobbyCreated_t> m_LobbyCreatedCallResult;

public:
	void InviteFriendA(CSteamID UserId);

	UFUNCTION(BlueprintCallable, Category = "SteamAPI|Lobby")
		void CreateFriendLobby(int nMaxMembers);
};