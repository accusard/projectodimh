// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "POStageData.generated.h"

class APOStage;
/**
 * 
 */
UCLASS()
class PROJECTODIMH_API UPOStageData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Staging)
	TSubclassOf<APOStage> StageClass;

	UPROPERTY(EditDefaultsOnly, Category=Staging)
	FString ActorLabel;

	UPROPERTY(EditDefaultsOnly, Category=Staging)
	uint32 MaxStageSlots = 2;

	UPROPERTY(VisibleAnywhere)
	AActor* AssociatedStagingPoint;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> ActorsOnStage;
	
	UPROPERTY(EditAnywhere, Category=Staging)
	uint8 bAutoStageActorsOnSpawn : 1;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Stage", GetFName());
		
	}
};
