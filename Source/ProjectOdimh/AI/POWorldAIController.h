// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagAssetInterface.h"
#include "POWorldAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTODIMH_API APOWorldAIController : public AAIController, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
	UPROPERTY(EditAnywhere, Category = GameplayTags)
	FGameplayTagContainer WorldAIControllerTags;
};