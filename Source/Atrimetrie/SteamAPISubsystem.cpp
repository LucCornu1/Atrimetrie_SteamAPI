// CORNU Luc


#include <stddef.h>
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
	
	FSteamFriend* Self = new FSteamFriend;
	Self->SteamID = SteamUser()->GetSteamID();
	FriendsArray.Init(Self, 1);

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
		const char* ControlFriend = "ElectricMammothDruid";
		for (int i = 0; i < FriendsArray.Num(); i++)
		{
			const char* FriendPersonaName = SteamFriends()->GetFriendPersonaName(FriendsArray[i]->SteamID);
			if (strcmp(ControlFriend, FriendPersonaName) == 0)
			{
				CreateFriendLobby(10);
				InviteFriendA(FriendsArray[i]->SteamID);
				break;
			}
		}
	}*/
	return true;

}


// Friends
void USteamAPISubsystem::SetSteamFriendArray()
{
	int MaxFriendCount = SteamFriends()->GetFriendCount(k_EFriendFlagImmediate);

	for (int i = 1; i < MaxFriendCount; i++)
	{
		FSteamFriend* SteamFriend = new FSteamFriend;
		SteamFriend->SteamID = SteamFriends()->GetFriendByIndex(i, k_EFriendFlagImmediate);
		FriendsArray.Emplace(SteamFriend);
	}

}


// Lobbies
void USteamAPISubsystem::OnLobbyEntered(LobbyEnter_t* pCallback)
{
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("You've entered a Lobby")));
	}*/

}

void USteamAPISubsystem::OnLobbyUpdate(LobbyChatUpdate_t* pCallback)
{
	if (pCallback->m_rgfChatMemberStateChange == k_EChatMemberStateChangeEntered)
	{	
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Test Lobby Entered")));
		}

		BYTE* data = new BYTE[200];
		SteamNetworking()->SendP2PPacket(/*pCallback->m_ulSteamIDUserChanged*/ SteamUser()->GetSteamID(), data, 200, k_EP2PSendReliable);
	}

}

void USteamAPISubsystem::OnPacketReceived(P2PSessionRequest_t* pCallback)
{
	if (SteamNetworking()->AcceptP2PSessionWithUser(pCallback->m_steamIDRemote))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Packet Received")));
		}
	}

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

void USteamAPISubsystem::CreateFriendLobby(int nMaxMembers)
{
	SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(k_ELobbyTypeFriendsOnly, nMaxMembers);
	m_LobbyCreatedCallResult.Set(hSteamAPICall, this, &USteamAPISubsystem::OnLobbyCreated);

}

void USteamAPISubsystem::TestFunction_Debug()
{
	const char* ControlFriend = "ElectricMammothDruid";
	for (int i = 0; i < FriendsArray.Num(); i++)
	{
		const char* FriendPersonaName = SteamFriends()->GetFriendPersonaName(FriendsArray[i]->SteamID);
		if (strcmp(ControlFriend, FriendPersonaName) == 0)
		{
			CreateFriendLobby(10);
			InviteFriendA(FriendsArray[i]->SteamID);
			JooJ = FriendsArray[i]->SteamID;
			break;
		}
	}

}

void USteamAPISubsystem::SendJooJAMessage()
{
	BYTE* data = new BYTE[200];
	if (SteamNetworking()->SendP2PPacket(/*pCallback->m_ulSteamIDUserChanged*/ JooJ, data, 200, k_EP2PSendReliable))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Package successfully sent")));
		}
	}

}

