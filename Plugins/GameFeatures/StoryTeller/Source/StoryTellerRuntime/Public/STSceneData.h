// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "STSceneData.generated.h"

class APOStagingInfo;
class USTTheme;

/**
 * 
 */
UCLASS()
class STORYTELLERRUNTIME_API USTSceneData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// moved to StageData
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Scene Info")
	// TSubclassOf<APOStagingInfo> ContextClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Scene Info")
	TSubclassOf<USTTheme> ThemeClass;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Scene", GetFName());
		
	}
	
};
