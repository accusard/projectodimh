// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "POStagingComponent.h"
#include "ProjectOdimh/Entities/Game/POMapPoint.h"
#include "ProjectOdimh/Entities/Game/POStage.h"
#include "ProjectOdimh/Entities/Game/POStagingInfo.h"


// Sets default values for this component's properties
UPOStagingComponent::UPOStagingComponent()
{
	
}

bool UPOStagingComponent::StageActor(AActor* InActor)
{
	if(IsStageFull())
	{
		return false;
	}
	
	// if(const APOMapPoint* MapPoint = GetOwner<APOStage>()->GetMapPoint())
	// {
	// 	const TArray<AActor*> List = MapPoint->GetActiveCharacters();
	// 	
	// 	if(List.Find(InActor) == INDEX_NONE)
	// 	{
	// 		UE_LOG(LogTemp,Warning,TEXT("No active actors located at MapPoint to be placed on stage!"));
	// 		return false;
	// 	}
	// }
	
	const FString NumStr = FString::Printf(TEXT("%i"), ActorsInPlay.Num()+1);
	
	ActorsInPlay.Add(FName(ActorsLabel + NumStr), InActor);
	
	return true;
}

void UPOStagingComponent::StageActors(const TArray<AActor*>& InActors)
{
	for(AActor* Actor : InActors)
	{
		if(ActorsInPlay.FindKey(Actor))
		{
			UE_LOG(LogTemp,Warning,TEXT("Actor is already on stage!"));
			continue;;
		}
		
		if(StageActor(Actor) == false)
		{
			break;
		}
	}
}

bool UPOStagingComponent::IsStageFull() const
{
	return ActorsInPlay.Num() >= Capacity;
}

void UPOStagingComponent::GetActorsInPlay(TArray<AActor*>& OutActors)
{
	for(auto It = ActorsInPlay.CreateIterator(); It; ++It)
	{
		if(AActor* ValidActor = It->Value)
		{
			OutActors.Add(ValidActor);
		}
	}
}

void UPOStagingComponent::EmptyStage()
{
	if(const APOStage* Stage = Cast<APOStage>(GetOwner()))
	{
		if(APOMapPoint* MapPoint = Stage->GetMapPoint())
		{
			TArray<AActor*> Actors;
			GetActorsInPlay(Actors);
			// MapPoint->MakeInactive(Stage, Actors);
			ActorsInPlay.Empty(Capacity);
		}
	}
}



