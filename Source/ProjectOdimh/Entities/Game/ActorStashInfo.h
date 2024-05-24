// Copyright 2023 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectOdimh/POGameplayTags.h"
#include "ActorStashInfo.generated.h"
USTRUCT(BlueprintType)
struct FStashingData
{
	GENERATED_BODY()

	FStashingData()
	{
		bIsHidden = true;
		bIsCollisionDisabled = true;
		StashingZAxisOffset = 0.f;
	}
	
	FStashingData(const FVector& InStashedLocation, const FVector& InOriginalLocation, const float ZAxisOffset = 200.f, const bool bIsHidden = true, const bool bIsCollisionDisabled = true)
		: StashedLocation(InStashedLocation), OriginalLocation(InOriginalLocation), StashingZAxisOffset(ZAxisOffset), bIsHidden(bIsHidden), bIsCollisionDisabled(bIsCollisionDisabled)
	{
		
	}
	
	const FVector& GetStashedLocation() const { return StashedLocation; }
	const FVector& GetOriginalLocation() const { return OriginalLocation; }
	float GetZAxisOffset() const { return StashingZAxisOffset; }
	bool IsHidden() const { return bIsHidden; }
	bool IsCollisionDisabled() const { return bIsCollisionDisabled; }
	
	
private:
	UPROPERTY()
	FVector StashedLocation;
	UPROPERTY()
	FVector OriginalLocation;
	UPROPERTY()
	float StashingZAxisOffset;
	UPROPERTY()
	uint8 bIsHidden : 1;
	UPROPERTY()
	uint8 bIsCollisionDisabled : 1;
};

UCLASS()
class PROJECTODIMH_API AActorStashInfo : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActorStashInfo();

	static const TArray<AActor*> GetStashedActors();
	void GetStashedActorsTags(FGameplayTagContainer& Tags);
	static const TMap<AActor*, FStashingData>& GetStashDataByActors() { return StashDataByActors; }
	
	
	static void MakeActive(AActor* Actor, const FVector& NewLocation);
	static void StashAtLocation(AActor* Actor, const FVector& StashLocation, const float ZAxis, const bool bDisableCollision, const bool bSetHidden);
	static void StashOnActor(AActor* Actor, const AActor* StashOn, const float ZAxis, const bool bDisableCollision, const bool bSetHidden);

private:
	static TMap<AActor*, FStashingData> StashDataByActors;
};
