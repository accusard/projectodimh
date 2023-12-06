// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "POGameState.generated.h"


class APoints;
class ATier;

/**
 * 
 */
UCLASS()
class PROJECTODIMH_API APOGameState : public AGameState
{
	GENERATED_BODY()
	
public:
    APOGameState();
    
    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 AwarenessCounter;
    
    UPROPERTY(BlueprintReadOnly, SaveGame)
    int32 LifetimeMatchedTiles;
    
    UPROPERTY(BlueprintReadOnly, SaveGame)
    uint8 bGameHasStarted:1;
    
    UPROPERTY(SaveGame)
    class UGameEvent* StartState;
    
    UPROPERTY(BlueprintReadOnly, SaveGame)
    ATier* TierProgression;
};
