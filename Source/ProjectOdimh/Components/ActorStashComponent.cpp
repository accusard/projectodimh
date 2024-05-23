// Copyright 2024 Vanny Sou. All Rights Reserved.

#include "ProjectOdimh/Components/ActorStashComponent.h"
#include "GameplayTagAssetInterface.h"

UActorStashComponent::UActorStashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bDisableCollisionOnStash = true;
	bStashActorsOnCollision = true;
	bHideActorsOnStash = true;
}

void UActorStashComponent::GetActiveCharacterTags(FGameplayTagContainer& Tags) const
{
	for(AActor* Actor : ActiveCharacters)
	{
		if(const IGameplayTagAssetInterface* Interface = Cast<IGameplayTagAssetInterface>(Actor))
		{
			Interface->GetOwnedGameplayTags(Tags);
		}
	}
}

bool UActorStashComponent::IsCharacterAtMapPoint(AActor* Character) const
{
	// Implementation goes here
}

bool UActorStashComponent::HasPlayerControlledCharacter() const
{
	// Implementation goes here
}

const TArray<AActor*>& UActorStashComponent::GetActiveCharacters() const
{
	// Implementation goes here
}

void UActorStashComponent::MakeInactive(const APOStage* Stage, const TArray<AActor*>& Actors)
{
	// Implementation goes here
}

void UActorStashComponent::StashActor(AActor* TargetActor, const AActor* StashOn, const float ZAxis, const bool bDisableCollision, const bool bSetHidden)
{
	// Implementation goes here
}

