// Copyright 2023 Vanny Sou. All Rights Reserved.


#include "ActorStashInfo.h"
#include "GameplayTagAssetInterface.h"

TMap<AActor*, FStashingData> AActorStashInfo::StashDataByActors;

// Sets default values
AActorStashInfo::AActorStashInfo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

const TArray<AActor*> AActorStashInfo::GetStashedActors()
{
	TArray<AActor*> StashedActorsArray;
	for(auto& Elem : StashDataByActors)
	{
		StashedActorsArray.Add(Elem.Key);
	}
	return StashedActorsArray;
}

void AActorStashInfo::GetStashedActorsTags(FGameplayTagContainer& Tags)
{
	for(AActor* Actor : GetStashedActors())
	{
		if(const IGameplayTagAssetInterface* Interface = Cast<IGameplayTagAssetInterface>(Actor))
		{
			Interface->GetOwnedGameplayTags(Tags);
		}
	}
}

void AActorStashInfo::MakeActive(AActor* Actor, const FVector& NewLocation)
{
	if(StashDataByActors.Find(Actor))
	{
		Actor->SetActorLocation(NewLocation);
		Actor->SetActorEnableCollision(true);
		Actor->SetActorHiddenInGame(false);
		StashDataByActors.Remove(Actor);
	}
}

void AActorStashInfo::StashAtLocation(AActor* Actor, const FVector& StashLocation, const float ZAxis,
	const bool bDisableCollision, const bool bSetHidden)
{
	const FStashingData& StashData = FStashingData(StashLocation, Actor->GetActorLocation(), ZAxis, bSetHidden, bDisableCollision);
	
	Actor->SetActorLocation(StashLocation);
	Actor->SetActorEnableCollision(!bDisableCollision);
	Actor->SetActorHiddenInGame(bSetHidden);

	StashDataByActors.Add(Actor, StashData);
}

void AActorStashInfo::StashOnActor(AActor* Actor, const AActor* StashOn, const float ZAxis,
                                   const bool bDisableCollision, const bool bSetHidden)
{
	const FVector& StashLocation = FVector(StashOn->GetActorLocation().X, StashOn->GetActorLocation().Y, StashOn->GetActorLocation().Z + ZAxis);
	const FStashingData& StashData = FStashingData(StashLocation, Actor->GetActorLocation(), ZAxis, bSetHidden, bDisableCollision);
	
	Actor->SetActorLocation(StashLocation);
	Actor->SetActorEnableCollision(!bDisableCollision);
	Actor->SetActorHiddenInGame(bSetHidden);

	StashDataByActors.Add(Actor, StashData);
}

