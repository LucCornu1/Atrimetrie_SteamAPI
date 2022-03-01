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
	SteamNetworkingUtils()->InitRelayNetworkAccess();
	SetSteamFriendArray();

	/*for (int i = 0; i < Size; i++)
	{
		const char* FriendPersonaName = SteamFriends()->GetFriendPersonaName(SteamFriendArray[i]);
		size_t n = strlen(FriendPersonaName);
		const FString& Str = FString(n, FriendPersonaName);
		const TCHAR* Text = *Str;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("%s : %d"), Text, SteamFriendArray[i].ConvertToUint64()));
		}
	}*/

}

void USteamAPISubsystem::Deinitialize()
{
	SteamAPI_Shutdown();

}

bool USteamAPISubsystem::Tick(float DeltaTime)
{
	SteamAPI_RunCallbacks();

	if (GetWorld()->IsServer())
	{
		if (!bInvited)
			CreateFriendLobby(10);
	}
	
	/*if (GetWorld()->IsServer())
	{
		// CreateFriendLobby(10); // Should be in a seperate function

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Server")));
		}
	}
	else {
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Not server")));
		}
	}*/

	/*if (!bInvited)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("FALSE")));
		}

		const char* ControlFriend = "ElectricMammothDruid";
		for (int i = 0; i < Size; i++)
		{
			const char* FriendPersonaName = SteamFriends()->GetFriendPersonaName(SteamFriendArray[i]);
			if (strcmp(ControlFriend, FriendPersonaName) == 0)
			{
				InviteFriendA(SteamFriendArray[i]);
			}
		}
	}*/
	return true;

}


// Friends
void USteamAPISubsystem::SetSteamFriendArray()
{
	int MaxFriendCount = SteamFriends()->GetFriendCount(k_EFriendFlagImmediate);
	CSteamID* NewArray = new CSteamID[MaxFriendCount];

	for (int i = 0; i < MaxFriendCount; i++)
	{
		NewArray[i] = SteamFriends()->GetFriendByIndex(i, k_EFriendFlagImmediate);
	}

	Size = MaxFriendCount;
	delete[] SteamFriendArray;
	SteamFriendArray = NewArray;

}


// Lobbies
void USteamAPISubsystem::OnLobbyEntered(LobbyEnter_t* pCallback)
{
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("You've entered a Lobby")));
	}*/

}

void USteamAPISubsystem::OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure)
{
	if (pCallback->m_eResult != k_EResultOK || bIOFailure)
	{
		UE_LOG(LogTemp, Error, TEXT("Lobby creation failed"));
		return;
	}
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("You've created a Lobby %d"), pCallback->m_ulSteamIDLobby));
	}*/
	LobbyId = CSteamID(pCallback->m_ulSteamIDLobby);

}

void USteamAPISubsystem::InviteFriendA(CSteamID UserId)
{
	if (SteamMatchmaking()->InviteUserToLobby(LobbyId, UserId))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Friend invited")));
		}
		bInvited = true;
	}
	else {
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Friend not invited")));
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

}