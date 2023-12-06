// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Object.h"
#include "ProjectOdimh/POGameplayTags.h"
#include "ProjectOdimh/ClassInterface/DelegateBindingInterface.h"
#include "STSceneManager.generated.h"

class APOStagingInfo;

typedef TArray<FSceneInfoRow*> SceneRows;

USTRUCT(BlueprintType)
struct FSceneInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId SceneId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;
	
};

/**
 * Keeps a pointer to the ObjectContextQuerier on Initialize
 * 
 */
UCLASS(Blueprintable)
class STORYTELLERRUNTIME_API USTSceneManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USTSceneManager();
	
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	static void GetSceneInfoRows(const UDataTable* Table, SceneRows& RowsOut);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category=Story)
	UDataTable* SceneTable;

	UPROPERTY(EditAnywhere, Category=Story)
	FGameplayTagContainerEntry AppliesSceneContext;
	
	UPROPERTY(VisibleAnywhere, Category="Game Info")
	AInfo* StagingInfo;
	
};
