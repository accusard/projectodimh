// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectOdimh/Events/BaseEvent.h"
#include "GridEvent.generated.h"



class ATile;

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTODIMH_API UGridEvent : public UBaseEvent
{
	GENERATED_BODY()

    
public:
	UFUNCTION(BlueprintCallable, Category="Grid State")
	void UpdateGridState(uint8 NewState, UGridEvent* OldEvent);

	UFUNCTION(BlueprintCallable, Category="Grid State")
	void UpdateTilesState(const TArray<ATile*>& BurstingTiles, const TArray<ATile*>& ReservingTiles);
	
	UPROPERTY(BlueprintReadOnly, Category="Grid State")
	uint8 CurrentState;
    
	UPROPERTY(BlueprintReadOnly, Category="Grid State")
	uint8 LastState;
	
protected:
    UPROPERTY(BlueprintReadWrite, Category="Tile Data")
    TArray<ATile*> ReservedTiles;
    
    UPROPERTY(BlueprintReadWrite, Category="Tile Data")
    TArray<ATile*> TilesToBurst;
};
