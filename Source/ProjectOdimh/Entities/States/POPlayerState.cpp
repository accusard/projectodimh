// Copyright 2023 Vanny Sou. All Rights Reserved.


#include "POPlayerState.h"

APOPlayerState::APOPlayerState()
{
	
}

void APOPlayerState::SaveState(UPOSaveGame* SaveObject)
{
	SaveObject->Player = PlayerData;
	UE_LOG(LogTemp,Warning,TEXT("SavePlayerState: SaveObject->Player = PlayerData"));
}

void APOPlayerState::LoadState(UPOSaveGame* SaveObject)
{
	PlayerData = SaveObject->Player;
	UE_LOG(LogTemp,Warning,TEXT("LoadPlayerState: PlayerData = SaveObject->Player"));
}
