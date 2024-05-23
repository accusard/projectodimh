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

void APOMapPoint::GetAllActiveTags(FGameplayTagContainer& Tags, const TArray<AActor*>& Actors) const
{
	Tags.AppendTags(MapTags);
	
	for(AActor* Actor : ActiveCharacters)
	{
		if(const IGameplayTagAssetInterface* Interface = Cast<IGameplayTagAssetInterface>(Actor))
		{
			Interface->GetOwnedGameplayTags(Tags);
		}
	}
}

void APOMapPoint::BeginPlay()
{
	Super::BeginPlay();

	APOGameMode* GameMode = Cast<APOGameMode>(GetWorld()->GetAuthGameMode());
	const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
	const UEventManager* EventManager = Instance->GetSubsystem<UEventManager>();

	CollectorSphere->OnActorsCollected.AddDynamic(this, &APOMapPoint::OnActorsCollision);
	
	EventManager->BindDelegateToActor(CollectorSphere->OnActorsCollected, GameMode);
}

void APOMapPoint::StashActor(AActor* TargetActor, const AActor* StashOn, const float ZAxis, const bool bDisableCollision, const bool bSetHidden)
{
	const FVector& StashLocation = FVector(StashOn->GetActorLocation().X, StashOn->GetActorLocation().Y, StashOn->GetActorLocation().Z + ZAxis);

	TargetActor->SetActorLocation(StashLocation);
	TargetActor->SetActorEnableCollision(!bDisableCollision);
	TargetActor->SetActorHiddenInGame(bSetHidden);
}

bool APOMapPoint::IsCharacterAtMapPoint(AActor* Character) const
{
	TArray<AActor*> MapPointActors;
	
	if(!ActiveCharacters.IsEmpty())
	{
		MapPointActors += ActiveCharacters;
	}
	if(!InactiveCharacters.IsEmpty())
	{
		MapPointActors += InactiveCharacters;
	}
	
	return MapPointActors.Find(Character) == INDEX_NONE ? false : true;
}

bool APOMapPoint::HasPlayerControlledCharacter() const
{
	for(AActor* Actor : ActiveCharacters)
	{
		if(const APOCharacter* Character = Cast<APOCharacter>(Actor))
		{
			return Character->IsPlayerControlled();
		}
	}

	for(AActor* Actor : InactiveCharacters)
	{
		if(const APOCharacter* Character = Cast<APOCharacter>(Actor))
		{
			return Character->IsPlayerControlled();
		}
	}
	
	return false;
}

const TArray<AActor*>& APOMapPoint::GetActiveCharacters() const
{
	return ActiveCharacters;
}

void APOMapPoint::MakeInactive(const APOStage* Stage, const TArray<AActor*>& Actors)
{
	ensure(Stage->GetMapPoint() == this);
	
	for(AActor* It : Actors)
	{
		if(IsValid(It))
		{
			ActiveCharacters.Remove(It);
			InactiveCharacters.Add(It);
		}
	}
}

void APOMapPoint::SetLocationFields(const FGameplayTagContainer& InTags)
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