// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectOdimh/ClassInterface/CombatModeInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ProjectOdimh/POGameplayTags.h"
#include "Match3CombatSystem.generated.h"

class AMatch3Grid;

/**
 * + contain the combat mode for a match 3 session
 * + responsible for starting a session
 */
UCLASS(Blueprintable)
class MATCH3COMBATRUNTIME_API UMatch3CombatSystem : public UGameInstanceSubsystem, public ICombatModeInterface
{
	GENERATED_BODY()

public:

	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual TSubclassOf<AActor> GetMode() override;
	
	void NewMatch3Session(AActor* CombatStage);

	AMatch3Grid* GetMatch3Grid(AActor* CombatStage) const;
    
    UObject* NewTurn(const FName& ParticipantName, const bool bStartTurnNow);
	
	void OnTurnStart(const FString& ParticipantName);
	
	void ReceiveRequestToEndTurn();

	void TryEndTurn();
	
	void OnTurnEnd(const bool bEndTurnNow);
	
	bool IsPlayerTurn() const;

	void ReceiveActorReleasedNotification(const AActor* ReleasedActor);
	
	void ReceiveActorPickedNotification(const AActor* PickedActor);
	
private:
	UObject* CombatTurn;
	
	int TurnNumber;
};
