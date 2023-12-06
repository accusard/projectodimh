// // Copyright 2022 Vanny Sou. All Rights Reserved.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "GameFramework/Actor.h"
// #include "ProjectOdimh/POGameplayTags.h"
// #include "POStagingInfo.generated.h"
//
// // DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStagingInfoSignature, APOStagingInfo*, Info);
//
// class UPOContextNeedsComponent;
// class APOMapPoint;
//
// USTRUCT(BlueprintType)
// struct FStageInfoRow : public FTableRowBase
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly)
// 	FPrimaryAssetId Id;
//
// 	// The Prerequisites will be compared with other Actors' OwnedGameplayTags 
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly)
// 	FGameplayTagContainer Prerequisites;
// 	
// };
//
// /**
//  * Once spawned by BeginDeferredActorSpawnFromClass and Intialized, will get passed
//  * to other subsystems to gather context that will be used to spawn a new Stage
//  */
// UCLASS(Abstract, Blueprintable, Deprecated)
// class PROJECTODIMH_API APOStagingInfo : public AInfo
// {
// 	GENERATED_BODY()
//
// public:
// 	// Sets default values for this actor's properties
// 	APOStagingInfo();
//
// 	UPROPERTY(EditAnywhere, Category=Staging)
// 	uint8 bAutoStageActorsOnSpawn : 1;
//
// 	UPROPERTY(VisibleAnywhere, Category=Staging)
// 	uint8 bMakeStagePlayerControlled : 1;
// 	
// 	void Initialize(AActor* Point, const TArray<AActor*>& InActors);
//
// 	// FStagingInfoSignature OnRequestStageType;
//
// 	APOMapPoint* GetMapPoint() const;
// 	
// 	FORCEINLINE bool ShouldPlayerControl() const { return bMakeStagePlayerControlled; }
// 	FORCEINLINE void SetMapPoint(AActor* InMapPoint) { AssociatedMapPoint = InMapPoint; }
// 	
// protected:
// 	virtual void BeginPlay() override;
// 	
// 	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
// 	
// private:
// 	void AddSubsystemsStagingHandler(FStagingInfoSignature& Delegate) const;
//
// 	void RemoveSubsystemsStagingHandler(FStagingInfoSignature& Delegate) const;
//
// 	UPROPERTY(EditDefaultsOnly, Category=Component)
// 	UPOContextNeedsComponent* ContextRequirements;
//
// 	UPROPERTY()
// 	AActor* AssociatedMapPoint;
// };
