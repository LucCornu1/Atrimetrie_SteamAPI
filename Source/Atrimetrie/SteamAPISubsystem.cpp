// CORNU Luc


#include "SteamAPISubsystem.h"

// Init & Deinit
void USteamAPISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// Register delegate for ticker callback
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &USteamAPISubsystem::Tick));


	// Steam init
	FFileHelper::SaveStringToFile(TEXT(RAW_APP_ID), TEXT("steam_appid.txt"));

	if (SteamAPI_RestartAppIfNecessary(atoi(APP_ID)))
	{
		FGenericPlatformMisc::RequestExit(true);
	}

	if (!SteamAPI_Init())
	{
		FGenericPlatformMisc::RequestExit(true);
	}
	
	MyId = SteamUser()->GetSteamID();
	CreateFriendLobby(10); // Should be in a seperate function

}

void USteamAPISubsystem::Deinitialize()
{
	SteamAPI_Shutdown();

}

bool USteamAPISubsystem::Tick(float DeltaTime)
{
	SteamAPI_RunCallbacks();
	return true;

}


// Lobbies
void USteamAPISubsystem::OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure)
{
	if (pCallback->m_eResult != k_EResultOK || bIOFailure)
	{
		UE_LOG(LogTemp, Error, TEXT("Lobby creation failed"));
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("You've created a Lobby")));
	}

}

void USteamAPISubsystem::OnLobbyEntered(LobbyEnter_t* pCallback)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("You've entered a Lobby")));
	}

}

void USteamAPISubsystem::CreateFriendLobby_Implementation(int nMaxMembers)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Max members in this lobby : %d"), nMaxMembers));
	}
	SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(k_ELobbyTypeFriendsOnly, nMaxMembers);
	m_LobbyCreatedCallResult.Set(hSteamAPICall, this, &USteamAPISubsystem::OnLobbyCreated);

}