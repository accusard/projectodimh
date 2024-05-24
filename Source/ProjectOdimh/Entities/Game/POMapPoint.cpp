// Copyright 2024 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/Entities/Game/POMapPoint.h"
#include "POCharacter.h"
#include "Modes/POGameMode.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/Subsystems/EventManager.h"
#include "ProjectOdimh/Components/POContextNeedsComponent.h"
#include "ProjectOdimh/Components/POActorCollectorSphere.h"
#include "ProjectOdimh/POGameplayTags.h"

APOMapPoint::APOMapPoint()
{
	SetMobility(EComponentMobility::Movable);
}

void APOMapPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APOMapPoint::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(MapTags);
}

void APOMapPoint::BeginPlay()
{
	Super::BeginPlay();

	
}

void APOMapPoint::SetMapTags(const FGameplayTagContainer& InTags)
{
	const FGameplayTag FieldTag = TAG_ALPHA_FIELD;
	
	if(FieldTag.MatchesAnyExact(InTags))
	{
		MapTags.AppendTags(InTags);
	}
	
}

void APOMapPoint::GetMapTags(FGameplayTagContainer& OutFields)
{
	for(FGameplayTag Tag : MapTags)
	{
		if(Tag.MatchesTag(TAG_ALPHA_FIELD))
		{
			OutFields.AddTag(Tag);
		}
	}
}