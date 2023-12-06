// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Data/POSaveGame.h"
#include "POGameInstance.generated.h"


class USaveGame;

/**
 * Aside from housing an instance of a game, give global access to an EventManager
 */
UCLASS()
class PROJECTODIMH_API UPOGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UPOGameInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void Init() override;
    
    UFUNCTION(BlueprintCallable)
    void ResetGame(const int32 PlayerIndex);

	bool SafeToSave(const bool bBypassGridState) const;
    
private:
    void SaveGameToSlot_Deprecated(USaveGame* Data, const FString& SlotName, const int32 Player);

public:
	// static functions
	/* Move an actor to a designated position, disable collisions, and hide the actor */
	static void StashActor(AActor* Actor, const FVector& Location, const bool bDisableCollision, const bool bSetHidden);
	
	UFUNCTION()
	static void AddOnScreenDebugMessage(const FString& Message);

	// deprecated
	const bool LoadActor_Deprecated(AActor* Actor, USaveGame* Data);
	void LoadComponents_Deprecated(AActor* Actor, USaveGame* Data);  
};
