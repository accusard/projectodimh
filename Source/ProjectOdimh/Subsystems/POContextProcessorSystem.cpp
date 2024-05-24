// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "POContextProcessorSystem.h"
#include "EventManager.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/ClassInterface/CombatModeInterface.h"
#include "ProjectOdimh/Entities/Game/POStage.h"


void UPOContextProcessorSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

UGameInstanceSubsystem* UPOContextProcessorSystem::GetConflictInterface() const
{
	const TArray<UGameInstanceSubsystem*>& List = GetGameInstance()->GetSubsystemArray<UGameInstanceSubsystem>();
	
	TArray<UGameInstanceSubsystem*> Resolvers;

	for(UGameInstanceSubsystem* CombatSystem : List)
	{
		if(!CombatSystem->GetClass()->ImplementsInterface(UCombatModeInterface::StaticClass()))
		{
			continue;
		}

		Resolvers.Add(CombatSystem);
		ensureMsgf(Resolvers.Num() == 1, TEXT("There should only be one combat system"));
	}
	
	return Resolvers[0];
}

TSubclassOf<AActor> UPOContextProcessorSystem::GetConflictFieldClass() const
{
	if(ICombatModeInterface* Interface = Cast<ICombatModeInterface>(GetConflictInterface()))
	{
		ensure(Interface);
		return Interface->GetCombatFieldClass();
	}
	
	return nullptr;
}

