// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "POConflictSystem.h"
#include "EventManager.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/ClassInterface/CombatModeInterface.h"
#include "ProjectOdimh/Data/Points.h"
#include "ProjectOdimh/Entities/Game/ConflictResult.h"
#include "ProjectOdimh/Entities/Game/POStage.h"


void UPOConflictSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	
	const UGameInstance* Instance = GetGameInstance();
	Instance->GetSubsystem<UEventManager>()->Delegates.OnActorEvent.AddUniqueDynamic(this, &UPOConflictSystem::OnThresholdMet);
}

void UPOConflictSystem::OnThresholdMet(AActor* Actor, UBaseEvent* Event)
{
	if(APoints* ScoreThreshold = Cast<APoints>(Actor))
	{
		UE_LOG(LogTemp,Warning,TEXT("Award winner"));
		// TODO: Award winner
	}
}

UGameInstanceSubsystem* UPOConflictSystem::GetConflictInterface() const
{
	const TArray<UGameInstanceSubsystem*>& List = GetGameInstance()->GetSubsystemArray<UGameInstanceSubsystem>();
	
	TArray<UGameInstanceSubsystem*> Resolvers;

	for(UGameInstanceSubsystem* CombatSystem : List)
	{
		if(!CombatSystem->GetClass()->ImplementsInterface(UCombatModeInterface::StaticClass()))
		{
			continue;
		}

		Resolvers->Add(CombatSystem);

	}
	
	ensureMsg(Resolvers->Num() == 1, TEXT("Conflict resolver should only be 1. Using the first available ConflictResolver");
	return Resolvers[0];
}

TSubclassOf<AActor> UPOConflictSystem::GetConflictFieldClass() const
{
	if(ICombatModeInterface* Interface = Cast<ICombatModeInterface>(GetConflictInterface()))
	{
		ensure(Interface);
		return Interface->GetCombatFieldClass();
	}
	
	return nullptr;
}

UObject* UPOConflictSystem::PickRandom(const TArray<UObject*>& List) const
{
	const int32 RandomIndex = FMath::RandRange(0, List.Num() - 1);
	return List[RandomIndex];
}

