// Copyright 2023 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/Entities/Game/POMapPoint.h"
#include "POCharacter.h"
#include "Modes/POGameMode.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/Subsystems/EventManager.h"
#include "ProjectOdimh/Components/POContextNeedsComponent.h"
#include "ProjectOdimh/Components/POGroupThrowingCollision.h"
#include "ProjectOdimh/POGameplayTags.h"

APOMapPoint::APOMapPoint()
{
	SetMobility(EComponentMobility::Movable);

	bDisableCollisionOnStash = true;
	bStashActorsOnCollision = true;
	bHideActorsOnStash = true;
	
	GroupCollisionComponent = CreateDefaultSubobject<UPOGroupThrowingCollision>("Group Throwing Component");
	GroupCollisionComponent->InitSphereRadius(40.f);
	
	RootComponent = GroupCollisionComponent;
	GetStaticMeshComponent()->SetupAttachment(GroupCollisionComponent);
}

void APOMapPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APOMapPoint::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(Locations);
}

void APOMapPoint::GetAllActiveContextTags(FGameplayTagContainer& Tags) const
{
	Tags.AppendTags(Locations);
	
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

	GroupCollisionComponent->OnActorsCollided.AddDynamic(this, &APOMapPoint::OnActorsCollision);
	
	EventManager->BindDelegateToActor(GroupCollisionComponent->OnActorsCollided, GameMode);
}

void APOMapPoint::OnActorsCollision(AActor* AtPoint, const TArray<AActor*>& CollidedActors)
{
	if(AtPoint->IsA<APOMapPoint>())
	{
		ActiveCharacters.Append(CollidedActors);
	
		if(bStashActorsOnCollision)
		{
			for(AActor* Actor : CollidedActors)
			{
				StashActor(Actor, this, StashingZAxisOffset, bDisableCollisionOnStash, bHideActorsOnStash);
			}
		}
	}
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
		Locations.AppendTags(InTags);
	}
	
}

void APOMapPoint::GetLocationFields(FGameplayTagContainer& OutFields)
{
	for(FGameplayTag Tag : Locations)
	{
		if(Tag.MatchesTag(TAG_ALPHA_FIELD))
		{
			OutFields.AddTag(Tag);
		}
	}
}