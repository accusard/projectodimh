// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "ProjectOdimh/Components/POStagingComponent.h"
#include "POStage.generated.h"

class APOMapPoint;
class UPOStagingComponent;
class UPOStageData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStagingSignature);

USTRUCT(BlueprintType)
struct FStageInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId Id;

	// The Prerequisites will be compared with other Actors' OwnedGameplayTags 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Prerequisites;
	
};

/**
 * The container for conflict resolution
 * Utilize GameplayStatics::BeginDeferredActorSpawnFromClass and ensure Initialize() 
 * is called before BeginPlay()
 * BeginPlay() is the entry point (starts logic when spawn)
 */
UCLASS()
class PROJECTODIMH_API APOStage : public APawn
{
	GENERATED_BODY()

	friend class UCombatModerator;
	
public:
	// Sets default values for this actor's properties
	APOStage();
	
	virtual void BeginDestroy() override;
	
	void Initialize(const UPOStageData* Data);

	FOnStagingSignature OnStagingFinished;
	
	APOMapPoint* GetMapPoint() const;
	
	bool IsStageFull() const { return StagingComponent->IsStageFull(); }
	
protected:
	UPROPERTY(VisibleAnywhere, Category=Staging)
	UPOStagingComponent* StagingComponent;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	void EmptyStage() const;
	
	
};
