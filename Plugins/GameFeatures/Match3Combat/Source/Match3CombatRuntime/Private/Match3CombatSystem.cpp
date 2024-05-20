// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "Match3CombatSystem.h"
#include "CombatGridComponent.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/POSubsystemsOverride.h"
#include "ProjectOdimh/Components/POContextNeedsComponent.h"


void UMatch3CombatSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// EvtManager->OnActorPicked.AddUniqueDynamic(this, &ADEPRECATED_Match3GameMode::ReceiveActorPickedNotification);
	// EvtManager->OnActorReleased.AddUniqueDynamic(this, &ADEPRECATED_Match3GameMode::ReceiveActorReleasedNotification);
	// EvtManager->OnActorEvent.AddUniqueDynamic(this, &ADEPRECATED_Match3GameMode::OnTurnEnd);
}

TSubclassOf<AActor> UMatch3CombatSystem::GetCombatFieldClass()
{
	return GetDefault<UPOSubsystemsOverride>()->ConflictFieldClass.Get();
}

UConflictResult* UMatch3CombatSystem::ResolveConflict(TArray<AActor*> Combatants) const
{
	// TODO#1234: Grid already created by GameMode->OnStagingLoaded
	// continue logic here
	UPOGameInstance::AddOnScreenDebugMessage("ResolveConflict Begins!");
	UE_LOG(LogTemp, Warning, TEXT("ResolveConflict Begins!"));
	return nullptr;
}

void UMatch3CombatSystem::NewMatch3Session(AActor* CombatStage)
{
	bool bCreateNewGrid = false;
    
	if(bCreateNewGrid)
	{
		// GetCombatGrid(CombatStage)->NewGrid();
	}
}

AMatch3Grid* UMatch3CombatSystem::GetMatch3Grid(AActor* CombatStage) const
{
	AMatch3Grid* Grid = nullptr;

	if(const UCombatGridComponent* FoundComp = CombatStage->FindComponentByClass<UCombatGridComponent>())
	{
		Grid = FoundComp->GetCombatGrid();
	}
	
	return Grid;
}

UObject* UMatch3CombatSystem::NewTurn(const FName& ParticipantName, const bool bStartTurnNow)
{
	if(IsValid(CombatTurn))
	{
		CombatTurn->MarkAsGarbage();
	}
	
	CombatTurn = NewObject<UObject>(this, ParticipantName);

	if(bStartTurnNow)
	{
		OnTurnStart(ParticipantName.ToString());
	}
	
	return CombatTurn;
}

void UMatch3CombatSystem::OnTurnStart(const FString& ParticipantName)
{
	UE_LOG(LogTemp,Warning,TEXT("Next Turn: %s"), *ParticipantName);
}

void UMatch3CombatSystem::ReceiveRequestToEndTurn()
{
	if(!IsValid(CombatTurn))
	{
		return;
	}

	TryEndTurn();
	// Actor->GetWorldTimerManager().SetTimer(TurnTickTimerHandler, this, &UMatch3CombatSystem::TryEndTurn, 0.1f, true);
}

void UMatch3CombatSystem::TryEndTurn()
{
	bool bSuccess = true;
	// UOldBaseEvent* Event = Instance->EventManager->NewEvent<UGameEvent>(this, F_TURN_ENDING_EVENT, true);
	// Instance->EventManager->OnActorEvent.Broadcast(this, Event);
	// GetWorldTimerManager().ClearTimer(TurnTickTimerHandler);
	if(bSuccess)
	{
		OnTurnEnd(bSuccess);
	}
}

void UMatch3CombatSystem::OnTurnEnd(const bool bEndTurnNow)
{
	if(bEndTurnNow)
	{
		CombatTurn->MarkAsGarbage();
	}
}

bool UMatch3CombatSystem::IsPlayerTurn() const
{
	return true;
}

void UMatch3CombatSystem::ReceiveActorReleasedNotification(const AActor* ReleasedActor)
{
	if(IsValid(ReleasedActor))
	{
		ReceiveRequestToEndTurn();
	}
}

void UMatch3CombatSystem::ReceiveActorPickedNotification(const AActor* PickedActor)
{
	if(IsValid(PickedActor))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Blue,"ReceiveActorPickedNotification");
	}
}
