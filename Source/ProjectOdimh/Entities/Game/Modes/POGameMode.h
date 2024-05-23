// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProjectOdimh/ClassInterface/DelegateBindingInterface.h"
#include "POGameMode.generated.h"


class UPOStageData;

/**
 * 
 */
UCLASS()
class PROJECTODIMH_API APOGameMode : public AGameMode, public IDelegateBindingInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category=Staging)
	UDataTable* StagingTable;
	
	UFUNCTION()
	void OnActorsArrivedAtPoint(AActor* AtPoint, const TArray<AActor*>& NewArrivals);

	UFUNCTION()
	void OnStagingLoaded(const FPrimaryAssetId Id, AActor* AtPoint, const TArray<AActor*> ActorsToBeStaged);
	
	void SetActiveStage(APawn* StageToSet);
	
	FORCEINLINE APawn* GetActiveStage() const { return ActiveStage; }

	APawn* SpawnStage(const UPOStageData* Data) const;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<AActor*> PendingActorsToBeStaged;
	
	FGameplayTagContainer StagingRequirements;

	UFUNCTION()
	FPrimaryAssetId GetStagingAssetId(const UDataTable* Table, const FGameplayTagContainer& TagContainer) const;

	UPROPERTY()
	APawn* ActiveStage;
	
public:
	//~ Begin Debug Commands
	UFUNCTION(Exec)
	void PODebugSave(const int32 PlayerIndex) const;

	UFUNCTION(Exec)
	void PODebugLoad(const int32 PlayerIndex) const;
	//~ End Debug Commands

	//~ Begin IDelegateBindingInterface
	virtual void AddOnActorsCollisionHandler(FOnActorsCollectedAtPointSignature& OnCollision) override;
	//~ End IDelegateBindingInterface
};
