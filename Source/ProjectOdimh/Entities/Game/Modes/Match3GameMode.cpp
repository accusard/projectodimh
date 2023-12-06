// Copyright 2022 Vanny Sou. All Rights Reserved.

#include "Match3GameMode.h"
#include "ProjectOdimh/POGameInstance.h"
#include "EngineUtils.h"
#include "ProjectOdimh/Entities/Game/Modes/GameplayOptions/Gameplay.h"
#include "ProjectOdimh/Entities/Game/Modes/GameplayOptions/GameplayRunModeBase.h"
#include "ProjectOdimh/ClassInterface/GameplayOptionsInterface.h"
#include "Engine/World.h"
#include "ProjectOdimh/Entities/Game/ParticipantTurn.h"
#include "ProjectOdimh/Entities/Game/Grid.h"
#include "ProjectOdimh/Components/ActionTurnBasedComponent.h"
#include "ProjectOdimh/Events/GameEvent.h"
#include "ProjectOdimh/Subsystems//EventManager.h"
#include "ProjectOdimh/Data/Tier.h"
#include "ProjectOdimh/Entities/States/POGameState.h"
#include "ProjectOdimh/Entities/Game/Tile.h"
#include "ProjectOdimh/Subsystems/POSaveGameSubsystem.h"

AMatch3GameMode::AMatch3GameMode()
{
    
    
}

void AMatch3GameMode::BeginPlay()
{
    Super::BeginPlay();

    FActorSpawnParameters SpawnParams;
    
    // SpawnParams.Name = "Match3GameState";
    Match3GameState = GetGameState<APOGameState>();

    SpawnParams.Name = "Match3PlayerState";
    // Match3PlayerState = GetWorld()->SpawnActor<APOPlayerState>(SpawnParams); //GetGameInstance()->GetPrimaryPlayerController()->GetPlayerState<APOPlayerState>();

    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    
    UEventManager* EvtManager = Instance->GetSubsystem<UEventManager>();

    EvtManager->Delegates.OnActorPicked.AddUniqueDynamic(this, &AMatch3GameMode::ReceiveActorPickedNotification);
    EvtManager->Delegates.OnActorReleased.AddUniqueDynamic(this, &AMatch3GameMode::ReceiveActorReleasedNotification);
    EvtManager->Delegates.OnActorEvent.AddUniqueDynamic(this, &AMatch3GameMode::HandleTierThreshold);
    EvtManager->Delegates.OnActorEvent.AddUniqueDynamic(this, &AMatch3GameMode::OnTurnEnd);
    
    LastMove = nullptr;
    
    if(RunMode)
        Mode = GetWorld()->SpawnActor<AGameplayRunModeBase>(RunMode);
    else
        Mode = GetWorld()->SpawnActor<AGameplayRunModeBase>();

    for(TSubclassOf<AGameplay> Class : GameplayOptions)
    {
        Gameplays.Add(GetWorld()->SpawnActor<AGameplay>(Class));
    }
    
    bool bCreateNewGrid = false;
    
    if(UPOSaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UPOSaveGameSubsystem>())
    {
        const FString SlotName = SaveSubsystem->CreateSaveGameObject()->CONTINUE;
        constexpr int32 PlayerIndex = 0;
        bCreateNewGrid = !SaveSubsystem->ReadSave(SlotName, PlayerIndex);
    }
    
    if(bCreateNewGrid)
    {
        GetGrid()->NewGrid();
    }
}

void AMatch3GameMode::StartPlay()
{
    Super::StartPlay();
    
    // initialize
    for(AGameplay* Option : Gameplays)
    {
        if(Option->Implements<UGameplayOptionsInterface>())
        {
            const uint32 StepsBeforeTick = Option->GetNumOfTicksBeforeRun();
            const FGameStats Steps(StepsBeforeTick);
            
            Mode->SetGameplayToTickOn(Option, Steps);
        }
    }
    
    GetWorldTimerManager().SetTimer(GameStartTimerHandler, this, &AMatch3GameMode::StartGame, 1.f, true);
}

void AMatch3GameMode::StartMove(UBaseEvent* PrevMove)
{
    UE_LOG(LogTemp, Warning, TEXT("--> AMatch3GameMode::StartMove"));
    if(LastMove == nullptr)
        LastMove = NewTurn("Begin", false);
    else if(PrevMove)
    {
        LastMove = PrevMove;
        LastMove->End();
    }
    
    if(IsPlayerTurn(GetWorld()->GetFirstPlayerController()))
    {
        PlayerMove->Reset();
        PlayerMove->Start();
        GetGrid()->RegisterBoardState(TEXT("Pick"));
    }
}

void AMatch3GameMode::ResetLevel()
{
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    Instance->GetSubsystem<UEventManager>()->ClearEventQueue(true);
    
    // at the moment, AGameModeBase::ResetLevel will reset controller actors which
    // result in undesired behaviors with the game's puzzle grid.
    // instead only use a snippet of its code until a solution is found
    for (FActorIterator It(GetWorld()); It; ++It)
    {
        AActor* A = *It;
        if (IsValid(A) && A != this && !A->IsA<AController>() && ShouldReset(A))
        {
            A->Reset();
        }
    }
}

void AMatch3GameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    GetGameInstance()->GetSubsystem<UPOSaveGameSubsystem>()->HandleStartingNewPlayer(NewPlayer);
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

const bool AMatch3GameMode::LoadParticipants_Deprecated(USaveGame* Data)
{
    // load from save
    if(UPOSaveGame* SaveData = Cast<UPOSaveGame>(Data))
    {
        
        // GameState->ParticipantIndex = SaveData->CustomInt["CurrentParticipantIndex"];
        
        if(ParticipantsList_Deprecated.Num() > 0)
            return true;
        
        if(SaveData->ParticipantsRegistry.Num() != 0)
        {
            for(int32 i = 0; i < SaveData->ParticipantsRegistry.Num(); ++i)
            {
                FActorSpawnParameters Params;
                
                Params.Name = FName(*SaveData->ParticipantsRegistry[i].Id);
                Params.Owner = this;
                
                uint32 TurnNum = SaveData->ParticipantsRegistry[i].Value;
                
#if !UE_BUILD_SHIPPING
                UE_LOG(LogTemp,Warning,TEXT("Loading Participant: %s, Turn: %i"),*Params.Name.ToString(), TurnNum);
#endif
                AParticipantTurn* NewEntity = NewParticipant_Deprecated(Params);
                ParticipantsList_Deprecated.Add(TurnNum, NewEntity);
            }
            return true;
        }
    }
    return false;
}

void AMatch3GameMode::ClearEventQueue(const bool bForceClear, const FName SetEventPending, const float DelaySeconds)
{
    if(SetEventPending != NAME_None)
    {
        FTimerDelegate EventTimerDelegate;
        
        EventTimerDelegate.BindUFunction(this, FName("EndPendingEvents"), SetEventPending);
        GetWorldTimerManager().SetTimer(EventTimerHandle, EventTimerDelegate, 0.1f, true, DelaySeconds);
    }

    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    Instance->GetSubsystem<UEventManager>()->ClearEventQueue(bForceClear);
}

void AMatch3GameMode::EndPendingEvents(const FName EventId)
{
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    Instance->GetSubsystem<UEventManager>()->EndEvent(EventId);
    
    GetWorldTimerManager().ClearTimer(EventTimerHandle);
}

AParticipantTurn* AMatch3GameMode::NewParticipant_Deprecated(const FActorSpawnParameters& Params)
{
    AParticipantTurn* NewEntity = GetWorld()->SpawnActor<AParticipantTurn>(AParticipantTurn::StaticClass(), Params);
    NewEntity->Init(*Params.Name.ToString());
    
    return NewEntity;
}

TMap<uint32, AParticipantTurn*>& AMatch3GameMode::GetParticipants_Deprecated()
{
    return ParticipantsList_Deprecated;
}

const uint32 AMatch3GameMode::CalculateTotalTileValue(const uint32 TileCount, const uint32 Multiplier)
{
    return TileCount * Multiplier * TileValue;
}

void AMatch3GameMode::UpdateGameState(const int32 TileCount, const int32 ComboCount)
{
    if(Match3GameState->bGameHasStarted)
    {
        // uint32 Total = CalculateTotalTileValue(TileCount, ComboCount);
        // Match3PlayerState->PlayerData.ScoreMultiplier = ComboCount;
        // Match3PlayerState->PlayerData.Match3Score->Add(Total);
        Match3GameState->TierProgression->Add(TileCount);
    }
}

const int AMatch3GameMode::GetCurrentScore()
{
    return 0; //Match3PlayerState->PlayerData.Match3Score->GetTotalPoints();
}

void AMatch3GameMode::StartMatch()
{
    Super::StartMatch();
    
    
}

void AMatch3GameMode::NotifyGameplayOptionsTurnEnding(const int OnTick)
{
    // call to whoever is concerned with the turn ending on this tick
    for(AActor* It : Gameplays)
    {
        const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
        Instance->GetSubsystem<UEventManager>()->Delegates.OnTurnEnd.Broadcast(It, OnTick);
    }
    
    UE_LOG(LogTemp, Warning, TEXT("--> NotifyGameplayOptionsTurnEnding on TURN: %i"), OnTick);
}

const bool AMatch3GameMode::ParticipantsBlueprintIsValid_Deprecated()
{
    if(Participants_Deprecated.Num() == 0)
    {
        UE_LOG(LogTemp,Warning,TEXT("Participant's blueprint contains no element."));
        return false;
    }
    
    for(auto& Check : Participants_Deprecated)
    {
        AParticipantTurn* IsValid = Check.Value.GetDefaultObject();
        uint32 TurnOrder = Check.Key;
        
        if(IsValid && TurnOrder > 0)
            continue;
        else
        {
            UE_LOG(LogTemp,Warning,TEXT("Participant's blueprint contain invalid data. Ensure blueprint is assigned in GameMode and TurnOrder is greater than 0."));
            return false;
        }
    }
    
    return true;
}

const bool AMatch3GameMode::LoadParticipantsFromBlueprint_Deprecated()
{
    UE_LOG(LogTemp,Warning,TEXT("Creating a Participants list from preassigned blueprint."));

    for(auto& Elem : Participants_Deprecated)
    {
        if(AParticipantTurn* Spawn_BP = GetWorld()->SpawnActor<AParticipantTurn>(Elem.Value))
        {
            const FString& ClassName = Spawn_BP->GetName();
            Spawn_BP->Init(Spawn_BP->GetDisplayName());
            
#if !UE_BUILD_SHIPPING
            UE_LOG(LogTemp,Warning,TEXT("Creating new Participant: %s, %i"), *ClassName, Elem.Key);
#endif
            ParticipantsList_Deprecated.Add(Elem.Key, Spawn_BP);
        }
        else
        {
            UE_LOG(LogTemp,Warning,TEXT("Failed to load Participants from blueprint."))
            return false;
        }
    }
        
    return true;
}

void AMatch3GameMode::SaveParticipants_Deprecated(USaveGame* DataPtr)
{
#if !UE_BUILD_SHIPPING
    uint32 EntitiesRecorded = 0;
#endif
    if(UPOSaveGame* SaveData = Cast<UPOSaveGame>(DataPtr))
    {
        const int32 NumOfEntities = ParticipantsList_Deprecated.Num();
        // SaveData->CustomInt.Add("CurrentParticipantIndex", GameState->ParticipantIndex);
        
        // loop and cycle through for each element
//        for(int i = 0; i < ParticipantsList.Num(); i++)
        for(auto& Elem : ParticipantsList_Deprecated)
        {
            if(AParticipantTurn* CurrentEntity = Elem.Value)
            {
                FCustomIntData NewInfo(CurrentEntity->GetDisplayName(),
                                                     Elem.Key);
                
                // add to save data
                SaveData->ParticipantsRegistry.Add(NewInfo);
#if !UE_BUILD_SHIPPING
                EntitiesRecorded++;
                UE_LOG(LogTemp,Warning,TEXT("Saving Participant: %s, QUEUE POSITION: %i"), *NewInfo.Id, NewInfo.Value);
#endif
            }
            
        }
#if !UE_BUILD_SHIPPING
        UE_LOG(LogTemp,Warning,TEXT("Participants contain (%i) entities; data saved (%i) entities."), NumOfEntities, EntitiesRecorded);
        UE_LOG(LogTemp,Warning,TEXT(""));
#endif
    }
}

UGameEvent* AMatch3GameMode::NewTurn(const FName& TurnDescription, const bool bStartTurnNow)
{
    UGameEvent* Turn = NewObject<UGameEvent>(GetWorld()->GetFirstPlayerController(), TurnDescription);
    Turn->Init();
    if(bStartTurnNow)
        Turn->Start();
    return Turn;
}

void AMatch3GameMode::StartGame()
{
    if(AGrid* ActiveGame = GetGrid())
    {
        PlayerMove = NewTurn("Player Move", false);
        GetWorldTimerManager().ClearTimer(GameStartTimerHandler);
        Match3GameState->StartState = NewObject<UGameEvent>(GetWorld()->GetFirstPlayerController(), F_GAME_HAS_STARTED_EVENT);
        Match3GameState->StartState->Init();
    }
    
}

void AMatch3GameMode::OnStartGame_Implementation(const bool bSaveGame)
{
    Match3GameState->bGameHasStarted = true;
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    
    if(bSaveGame)
    {
        const int32 Player1 = (int32)EPlayer::One;
        
        Instance->GetSubsystem<UPOSaveGameSubsystem>()->WriteSave(RESET_GAME_SLOT, Player1);
        Instance->GetSubsystem<UPOSaveGameSubsystem>()->WriteSave(CONTINUE_GAME_SLOT, Player1);
        Instance->GetSubsystem<UPOSaveGameSubsystem>()->WriteSave(LAST_SUCCESSFUL_SLOT, Player1);
    }

    Match3GameState->StartState->Start();
    
    Instance->GetSubsystem<UEventManager>()->Delegates.OnActorEvent.Broadcast(this, Match3GameState->StartState);
}

const bool AMatch3GameMode::HasGameStarted() const
{
    return Match3GameState->bGameHasStarted;
}

void AMatch3GameMode::ReceiveRequestToEndTurn()
{
    if(PlayerMove->IsPendingFinish())
        return;
    
    const int EndedOnTurnNum = 1; // Match3PlayerState->PlayerData.TurnCounter;
    
    const float Delay = GetGrid()->DELAY_BETWEEN_TILE_BURSTS;
    
    // don't notify if game hasn't started
    NotifyGameplayOptionsTurnEnding(EndedOnTurnNum);
    GetWorldTimerManager().SetTimer(TurnTickTimerHandler, this, &AMatch3GameMode::TryEndTurn, 0.1f, true, Delay);
}

void AMatch3GameMode::ReceiveRequestToEndTurn(ATile* LastTileGrabbed)
{
    ReceiveRequestToEndTurn();
}

void AMatch3GameMode::HandleTierThreshold(AActor* Source, UBaseEvent* Evt)
{
    if(ATier* TierProgress = Cast<ATier>(Source))
    {
        if(Evt->GetFName() == F_THRESHOLD_EVENT)
        {
            const uint32 Reset = 0;
            TierProgress->LevelUp(Reset, ATier::DEFAULT_NEXT_TIER);
        }
    }
}

void AMatch3GameMode::TryEndTurn()
{
    if(PendingGameplayFinish())
        return;
    
    UE_LOG(LogTemp, Warning, TEXT("--> TryEndTurn"));
    
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    
    UBaseEvent* Event = Instance->GetSubsystem<UEventManager>()->NewEvent<UGameEvent>(this, F_TURN_ENDING_EVENT, true);
    Instance->GetSubsystem<UEventManager>()->Delegates.OnActorEvent.Broadcast(this, Event);
    
    GetWorldTimerManager().ClearTimer(TurnTickTimerHandler);
}

const bool AMatch3GameMode::IsPlayerTurn(AController* InCon) const
{
    return true; //Cast<AGridPlayerController>(InCon);
}

void AMatch3GameMode::OnTurnEnd_Implementation(AActor* Source, UBaseEvent* Event)
{
    UE_LOG(LogTemp, Warning, TEXT("--> AMatch3GameMode::OnTurnEnded_Implementation"));
    
    // Match3PlayerState->PlayerData.TurnCounter++;
    // Match3PlayerState->PlayerData.ScoreMultiplier = 1;
    // Match3PlayerState->PlayerData.BonusPoints = 0;
    Match3GameState->LifetimeMatchedTiles += GetGrid()->GetTotalMatchedThisTurn();
    
    // SaveCurrentGameState(Instance, false);
    
    GetGrid()->ResetAccumulatedMatchedTiles();
    
    ClearEventQueue();
}

void AMatch3GameMode::Give(AActor* Controller, const FMatch3GameAction& Action, const bool bExecuteNow)
{
    // give to the current active participant
    if(bExecuteNow && Controller)
    {
        if(UActionTurnBasedComponent* TurnActionComp = Controller->FindComponentByClass<UActionTurnBasedComponent>())
            TurnActionComp->TryExecute(Action);
    }
    
    // TODO: give to pending action?
}

void AMatch3GameMode::ReceiveActorPickedNotification(AActor* PickedActor)
{
    if(Cast<ATile>(PickedActor))
    {
        FMatch3GameAction SwapAction;
        // SwapAction.GameMode = this;
        SwapAction.Identifier = SWAP_POSITIONS;
        SwapAction.Num = INIT_MAX_ACTIONS;
        
        Give(GetWorld()->GetFirstPlayerController(), SwapAction);
    }
}

void AMatch3GameMode::ReceiveActorReleasedNotification(AActor* ReleasedActor)
{
    if(ATile* Tile = Cast<ATile>(ReleasedActor))
    {
        ReceiveRequestToEndTurn(Tile);
        PlayerMove->End();
    }
}

const bool AMatch3GameMode::PendingGameplayFinish() const
{
    for(AActor* Option : Gameplays)
    {
        if(IGameplayOptionsInterface* Gameplay = Cast<IGameplayOptionsInterface>(Option))
        {
            if(Gameplay->Execute_IsRunning(Option))
                return true;
        }
    }
    return false;
}
