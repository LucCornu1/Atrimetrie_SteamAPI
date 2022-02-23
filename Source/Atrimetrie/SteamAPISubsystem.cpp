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
}