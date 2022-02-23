// CORNU Luc

#include "GameFramework/DefaultPawn.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/WorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "NetworkPlayerStart.h"
#include "MultiplayerGameMode.h"

AMultiplayerGameMode::AMultiplayerGameMode()
{
	bNetLoadOnClient = false;
	bPauseable = false;
	bStartPlayersAsSpectators = false;

	DefaultPawnClass = ADefaultPawn::StaticClass();
	PlayerControllerClass = APlayerController::StaticClass();
	PlayerStateClass = APlayerState::StaticClass();
//	GameStateClass = AGameStateBase::StaticClass();
	HUDClass = AHUD::StaticClass();
	GameSessionClass = AGameSession::StaticClass();
	SpectatorClass = ASpectatorPawn::StaticClass();
	ReplaySpectatorPlayerControllerClass = APlayerController::StaticClass();
	ServerStatReplicatorClass = AServerStatReplicator::StaticClass();

	TargetSpawner = ESpawnerType::SPAWNER_PLANE;

}

void AMultiplayerGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AMultiplayerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Added
enum ESpawnerType AMultiplayerGameMode::GetNext(enum ESpawnerType Current)
{
	if (Current == ESpawnerType::SPAWNER_PLANE)
	{
		return ESpawnerType::SPAWNER_RESISTANT;
	}
	else
	{
		return ESpawnerType::SPAWNER_MECHA;
	}
}

AActor* AMultiplayerGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
//	Super::ChoosePlayerStart(Player);

	TArray<AActor*> FoundArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANetworkPlayerStart::StaticClass(), FoundArray);
	for (AActor* Actor : FoundArray)
	{
		ANetworkPlayerStart* PlayerStart = Cast<ANetworkPlayerStart>(Actor);

		if (PlayerStart->Type == TargetSpawner)
		{
			SetTargetSpawner(GetNext(TargetSpawner));
			return Actor;
		}
	}
	
	return nullptr;

}


