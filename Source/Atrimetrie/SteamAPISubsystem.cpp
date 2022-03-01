// CORNU Luc


#include "SteamAPISubsystem.h"

void USteamAPISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FFileHelper::SaveStringToFile(TEXT(RAW_APP_ID), TEXT("steam_appid.txt"));

	SteamAPI_RestartAppIfNecessary(atoi(APP_ID));

	if (SteamAPI_Init())
	{
		MyId = SteamUser()->GetSteamID();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SteamUserID : %d"), MyId.ConvertToUint64()));
		}
		// SteamFriends()->ActivateGameOverlay("friends");
	}
}

void USteamAPISubsystem::Deinitialize()
{
	SteamAPI_Shutdown();
<<<<<<< Updated upstream
=======

}

bool USteamAPISubsystem::Tick(float DeltaTime)
{
	SteamAPI_RunCallbacks();
	return true;

}


// Friends
void USteamAPISubsystem::SetSteamFriendArray()
{
	int MaxFriendCount = SteamFriends()->GetFriendCount(k_EFriendFlagImmediate);

	for (int i = 0; i++; i < MaxFriendCount)
	{
		SteamFriendArray[i] = SteamFriends()->GetFriendByIndex(i, k_EFriendFlagImmediate);
	}

}


// Lobbies
void USteamAPISubsystem::OnLobbyEntered(LobbyEnter_t* pCallback)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("You've entered a Lobby")));
	}

}

void USteamAPISubsystem::OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure)
{
	if (pCallback->m_eResult != k_EResultOK || bIOFailure)
	{
		UE_LOG(LogTemp, Error, TEXT("Lobby creation failed"));
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("You've created a Lobby %d"), pCallback->m_ulSteamIDLobby));
	}

}

void USteamAPISubsystem::InviteFriend(CSteamID UserId)
{
	if (SteamMatchmaking()->InviteUserToLobby(LobbyId, UserId))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Friend invited")));
		}
	}

}

void USteamAPISubsystem::JoinFriendLobby()
{
	// None

}

void USteamAPISubsystem::CreateFriendLobby_Implementation(int nMaxMembers)
{
	SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(k_ELobbyTypeFriendsOnly, nMaxMembers);
	m_LobbyCreatedCallResult.Set(hSteamAPICall, this, &USteamAPISubsystem::OnLobbyCreated);

>>>>>>> Stashed changes
}