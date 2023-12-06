// Copyright 2017-2019 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectOdimh/Data/GameTypes.h"
#include "ProjectOdimh/Data/POSaveGame.h"
#include "ProjectOdimh/Entities/Game/Modes/GameplayOptions/Gameplay.h"
#include "RelayLine.generated.h"

class UBaseEvent;

UCLASS()
class PROJECTODIMH_API ARelayLine : public AGameplay
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARelayLine();
    
    UFUNCTION(BlueprintImplementableEvent)
    const TArray<FCustomIntData> OnSaveFromBlueprint() const;
    UFUNCTION(BlueprintImplementableEvent)
    const bool OnLoadFromBlueprint(const TArray<FCustomIntData>& Data);
    
    UFUNCTION(BlueprintPure)
    TArray<ATile*> GetUnusedTiles(class AGrid* Grid);
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
    FIntPointLine LineCoords;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FIntPoint> ReservedGridCoords;
    

};
