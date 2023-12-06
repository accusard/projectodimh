// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "POSaveGameSubsystem.generated.h"

class UPOSaveGame;

/**
 * 
 */
UCLASS()
class PROJECTODIMH_API UPOSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintPure)
	UPOSaveGame* CreateSaveGameObject();

	UFUNCTION(BlueprintCallable)
	void WriteSave(const FString& SlotName, const int32 PlayerIndex);

	UFUNCTION(BlueprintCallable)
	const bool ReadSave(const FString& SlotName, const int32 PlayerIndex);

	UFUNCTION(BlueprintCallable)
	void HandleStartingNewPlayer(APlayerController* NewPlayer);

private:
	UPROPERTY()
	UPOSaveGame* CurrentGameSave;
};
