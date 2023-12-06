// Copyright 2023 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "POStage.h"
#include "Engine/StaticMeshActor.h"
#include "POMapPoint.generated.h"

class UPOGroupThrowingCollision;
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

	// Get all tags from Locations container
	UFUNCTION(BlueprintCallable, Category=GameplayTags)
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	void GetAllActiveContextTags(FGameplayTagContainer& Tags) const;

	// Get only 'field' tags from Locations container
	void GetLocationFields(FGameplayTagContainer& OutFields);

	UFUNCTION()
	void OnActorsCollision(AActor* AtPoint, const TArray<AActor*>& CollidedActors);

	bool IsCharacterAtMapPoint(AActor* Character) const;

	bool HasPlayerControlledCharacter() const;
	
	const TArray<AActor*>& GetActiveCharacters() const;

	UFUNCTION()
	void MakeInactive(const APOStage* Stage, const TArray<AActor*>& Actors);
	
	static void StashActor(AActor* TargetActor, const AActor* StashOn, const float ZAxis, const bool bDisableCollision, const bool bSetHidden);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Map)
	FGameplayTagContainer Locations;

	// any actors that have come into contact with owner's GroupCollisionComponent will be
	// added to this array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Map)
	TArray<AActor*> ActiveCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Map)
	TArray<AActor*> InactiveCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Map)
	TArray<AActor*> Treasure;

	UPROPERTY(VisibleAnywhere, Category=Staging)
	UPOGroupThrowingCollision*  GroupCollisionComponent;
	
	UPROPERTY(EditAnywhere, Category="Stashing")
	float StashingZAxisOffset = 200.f;

	UPROPERTY(EditAnywhere, Category="Stashing")
	uint8 bStashActorsOnCollision : 1;
	
	UPROPERTY(EditAnywhere, Category="Stashing")
	uint8 bDisableCollisionOnStash : 1;

	UPROPERTY(EditAnywhere, Category="Stashing")
	uint8 bHideActorsOnStash : 1;

private:
	void SetLocationFields(const FGameplayTagContainer& SetFields);
};
