// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/Entities/Game/Modes/POGameMode.h"
#include "EngineUtils.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/Data/POSaveGame.h"
#include "ProjectOdimh/Entities/Game/POStagingInfo.h"
#include "ProjectOdimh/Subsystems/POSaveGameSubsystem.h"




void APOGameMode::HandleCollectedActor(AActor* Actor)
{
	// use the ContextProcessorSystem to determine what to do with the actor
	
}

void APOGameMode::PODebugSave(const int32 PlayerIndex) const
{
	GetGameInstance<UGameInstance>()->GetSubsystem<UPOSaveGameSubsystem>()->WriteSave(CONTINUE_GAME_SLOT, PlayerIndex);
}

void APOGameMode::PODebugLoad(const int32 PlayerIndex) const
{
	GetGameInstance<UGameInstance>()->GetSubsystem<UPOSaveGameSubsystem>()->ReadSave(CONTINUE_GAME_SLOT, PlayerIndex);
}

void APOGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Setup handler for when an actor arrives at a point
	// Collects all actors that has this component and listens for when they are collected
	if(const UWorld* World = GetWorld())
	{
		for(TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
		{
			if(UPOActorCollectorSphere* Collector = ActorItr->FindComponentByClass<UPOActorCollectorSphere>())
			{
				Collector->OnActorCollected.AddUniqueDynamic(this, &APOGameMode::HandleCollectedActor);
			}
		}
	}
	
	
	
}
