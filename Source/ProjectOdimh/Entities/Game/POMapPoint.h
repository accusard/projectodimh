// Copyright 2023 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "POStage.h"
#include "Engine/StaticMeshActor.h"
#include "POMapPoint.generated.h"

class UPOActorCollectorSphere;
class UPOStagingComponent;
/**
 * 
 */
UCLASS()
class PROJECTODIMH_API APOMapPoint : public AStaticMeshActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	APOMapPoint();
	
	virtual void PostInitializeComponents() override;

	// Get all tags from MapTags container
	UFUNCTION(BlueprintCallable, Category=GameplayTags)
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	// Get only 'field' tags from MapTags container
	void GetMapTags(FGameplayTagContainer& OutFields);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Map)
	FGameplayTagContainer MapTags;

private:
	void SetMapTags(const FGameplayTagContainer& SetFields);
};
