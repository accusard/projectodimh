// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "POSaveGameSubsystem.h"

#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"
#include "ProjectOdimh/Data/POSaveGame.h"
#include "ProjectOdimh/Entities/States/POPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectOdimh/ClassInterface/DataSaveInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void UPOSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	
}

UPOSaveGame* UPOSaveGameSubsystem::CreateSaveGameObject()
{
	return Cast<UPOSaveGame>(UGameplayStatics::CreateSaveGameObject(UPOSaveGame::StaticClass()));
}

void UPOSaveGameSubsystem::WriteSave(const FString& SlotName, const int32 PlayerIndex)
{
	CurrentGameSave->SaveSlotName = SlotName;
	CurrentGameSave->UserIndex = PlayerIndex;
	CurrentGameSave->Actors.Empty();

	AGameStateBase* GameState = GetWorld()->GetGameState();

	ensure(GameState);
    
	APOPlayerState* MyPlayerState = Cast<APOPlayerState>(GameState->PlayerArray[PlayerIndex]);
	MyPlayerState->SaveState(CurrentGameSave);
    
	for(FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
        
		if(!IsValid(Actor) || !Actor->Implements<UDataSaveInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.Name = Actor->GetFName();
		ActorData.Transform = Actor->GetTransform();
		FMemoryWriter MemWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter,true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);
		CurrentGameSave->Actors.Add(ActorData);
	}
    
	UGameplayStatics::SaveGameToSlot(CurrentGameSave, CurrentGameSave->SaveSlotName, CurrentGameSave->UserIndex);
	UE_LOG(LogTemp,Warning,TEXT("Saved to slot - %s"), *CurrentGameSave->SaveSlotName);
}

const bool UPOSaveGameSubsystem::ReadSave(const FString& SlotName, const int32 PlayerIndex)
{
	bool bDataLoaded = false;

	AGameStateBase* GameState = GetWorld()->GetGameState();

	ensure(GameState);
    
	if(UGameplayStatics::DoesSaveGameExist(SlotName, PlayerIndex))
	{
		// load game from slot
		CurrentGameSave = Cast<UPOSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, PlayerIndex));

		// load player state
        
		for(FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
            
			if(!Actor->Implements<UDataSaveInterface>())
			{
				continue;
			}

			for(FActorSaveData ActorData : CurrentGameSave->Actors)
			{
				if(ActorData.Name == Actor->GetFName())
				{
					Actor->SetActorTransform(ActorData.Transform);
					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader,true);
					Ar.ArIsSaveGame = true;
					Actor->Serialize(Ar);
					bDataLoaded = true;
                    
					break;
				}
			}
		}
        
		return bDataLoaded;
	}

	CurrentGameSave = CreateSaveGameObject();
    
	UE_LOG(LogTemp,Warning,TEXT("Created a new save game object."));
    
	return bDataLoaded;
}

void UPOSaveGameSubsystem::HandleStartingNewPlayer(APlayerController* NewPlayer)
{
	NewPlayer->GetPlayerState<APOPlayerState>()->LoadState(CurrentGameSave);
}
