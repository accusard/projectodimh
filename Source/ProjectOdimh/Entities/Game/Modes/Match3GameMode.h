// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProjectOdimh/ClassInterface/DataSaveInterface.h"
#include "ProjectOdimh/Data/GameTypes.h"
#include "Match3GameMode.generated.h"



class AGrid;
class ATile;
class UGameEvent;
class AParticipantTurn;
class AGameplayRunModeBase;
class APOGameState;

/**
 * The mode for a Match3 game. Tracks a turn-based puzzle game.
 */
UCLASS()
class PROJECTODIMH_API AMatch3GameMode : public AGameMode, public IDataSaveInterface
{
	GENERATED_BODY()
	
public:
    // constructor
    AMatch3GameMode();
    
    virtual void StartPlay() override;
    virtual void StartMatch() override;
    virtual void ResetLevel() override;
    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
    
    UFUNCTION(BlueprintCallable)
    void StartMove(UBaseEvent* PrevMove);
    
    void NotifyGameplayOptionsTurnEnding(const int TurnNum);
    
    const uint32 CalculateTotalTileValue(const uint32 TileCount, const uint32 Multiplier = DEFAULT_SCORE_MULTIPLIER);
    
    /** Add to the score */
    UFUNCTION(BlueprintCallable)
    void UpdateGameState(const int32 TileCount, const int32 ComboCount);
    
    /** Get the current score of the game */
    UFUNCTION(BlueprintPure)
    const int GetCurrentScore();
    
    UFUNCTION(BlueprintCallable)
    void StartGame();
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void OnStartGame(const bool bSaveGame);
    
    UFUNCTION(BlueprintPure)
    const bool HasGameStarted() const;
    
    UGameEvent* NewTurn(const FName& TurnDescription, const bool bStartTurnNow);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnTurnStart(const FString& ParticipantName);
    
    UFUNCTION(BlueprintCallable)
    void ReceiveRequestToEndTurn();
    
    void ReceiveRequestToEndTurn(ATile* LastTileGrabbed);
    
    UFUNCTION()
    void HandleTierThreshold(AActor* Source, UBaseEvent* Evt);
    
    void TryEndTurn();
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void OnTurnEnd(AActor* Source, UBaseEvent* Event);
    
    const bool IsPlayerTurn(AController* InCon) const;
    
    void Give(AActor* Controller, const FMatch3GameAction& Action, const bool bExecuteNow = true);
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    AGrid* GetGrid() const;
    
    UBaseEvent* GetLastMove() const { return LastMove; }
    
    UFUNCTION()
    void ReceiveActorReleasedNotification(AActor* Actor);
    
    UFUNCTION()
    void ReceiveActorPickedNotification(AActor* PickedActor);
    
    const bool PendingGameplayFinish() const;
    
protected:
    virtual void BeginPlay() override;
    
    /** The gameplay mode to run when Match3GameMode receive request to end turn */
    UPROPERTY(EditAnywhere, Category="Additional Gameplay")
    TSubclassOf<AGameplayRunModeBase> RunMode;
    
    UPROPERTY(EditAnywhere, Category="Additional Gameplay")
    TArray<TSubclassOf<class AGameplay>> GameplayOptions;
    
    UPROPERTY(BlueprintReadOnly)
    AGameplayRunModeBase* Mode;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<class AGameplay*> Gameplays;

    UPROPERTY(BlueprintReadOnly)
    UBaseEvent* LastMove;
    
    UPROPERTY()
    APOGameState* Match3GameState;

    // UPROPERTY()
    // APOPlayerState* Match3PlayerState;

private:
    void ClearEventQueue(const bool bForceClear = false, const FName SetEventPending = NAME_None, const float DelaySeconds = 0.f);
    
    UFUNCTION()
    void EndPendingEvents(const FName EventId);
    
    UPROPERTY(EditDefaultsOnly)
    int32 TileValue = INIT_TILE_VALUE;
    
    UPROPERTY()
    UGameEvent* PlayerMove;
    
    FTimerHandle TurnTickTimerHandler;
    FTimerHandle GameStartTimerHandler;
    FTimerHandle EventTimerHandle;

// deprecated
public:
    const bool LoadParticipantsFromBlueprint_Deprecated();
    const bool LoadParticipants_Deprecated(USaveGame* Data);
    void SaveParticipants_Deprecated(USaveGame* Data);
    AParticipantTurn* NewParticipant_Deprecated(const FActorSpawnParameters& Params);
    const bool ParticipantsBlueprintIsValid_Deprecated();
    TMap<uint32, AParticipantTurn*>& GetParticipants_Deprecated();
    TMap<uint32, TSubclassOf<AParticipantTurn>> Participants_Deprecated;
    TMap<uint32, AParticipantTurn*> ParticipantsList_Deprecated;
    
};
