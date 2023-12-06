// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#include "POGameInstance.h"
#include "Data/POSaveGame.h"
#include "Events/GridEvent.h"
#include "Events/PlayerInputEvent.h"
#include "ClassInterface/DataSaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Subsystems//EventManager.h"
#include "Events/BaseEvent.h"
#include "GameFramework/GameStateBase.h"
#include "Subsystems/POSaveGameSubsystem.h"


UPOGameInstance::UPOGameInstance(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UPOGameInstance::StashActor(AActor* Actor, const FVector& Location, const bool bDisableCollision,
                                 const bool bSetHidden)
{
    // TODO call MapPoint->StashActors
}

void UPOGameInstance::Init()
{
    Super::Init();
    
}

void UPOGameInstance::SaveGameToSlot_Deprecated(USaveGame* Data, const FString& SlotName, const int32 Player)
{
    UGameplayStatics::SaveGameToSlot(Data, SlotName, Player);
}

void UPOGameInstance::ResetGame(const int32 PlayerIndex)
{
    UGameplayStatics::SetGamePaused(this, false);
    UGameplayStatics::GetGameMode(GetWorld())->ResetLevel();
    
    GetSubsystem<UPOSaveGameSubsystem>()->ReadSave(RESET_GAME_SLOT,PlayerIndex);
    GetSubsystem<UPOSaveGameSubsystem>()->WriteSave(CONTINUE_GAME_SLOT,PlayerIndex);
    GetSubsystem<UPOSaveGameSubsystem>()->WriteSave(LAST_SUCCESSFUL_SLOT,PlayerIndex);
}

bool UPOGameInstance::SafeToSave(const bool bNewGameState) const
{
    UEventManager* EventManager = Cast<UEventManager>(GetWorld()->GetGameInstance<UPOGameInstance>()->GetSubsystem<UEventManager>());
    const bool bGameStateValid = (EventManager->HasA(UGridEvent::StaticClass()) || bNewGameState);
    bool bNoPendingInput = true;
    
    TArray<UBaseEvent*> Events = EventManager->FindAll(UPlayerInputEvent::StaticClass());

    for(const auto PlayerInput : Events)
    {
        if(PlayerInput->IsPendingFinish())
        {
            bNoPendingInput = false;
            break;
        }
    }
    
    return (bGameStateValid && bNoPendingInput);
}

void UPOGameInstance::AddOnScreenDebugMessage(const FString& Message)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Message);
}

const bool UPOGameInstance::LoadActor_Deprecated(AActor* Actor, USaveGame* Data)
{
    const bool bNoErrorLoading = true;
    if(IDataSaveInterface* ActorSaveInterface = Cast<IDataSaveInterface>(Actor))
    {
        if(ActorSaveInterface->Load(Data))
        {
            UE_LOG(LogTemp, Warning, TEXT("LoadActor: %s success!"), *ActorSaveInterface->_getUObject()->GetName());
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("LoadActor: %s failed!"), *ActorSaveInterface->_getUObject()->GetName());
            return false;
        }
    }
    
    return bNoErrorLoading;
}

void UPOGameInstance::LoadComponents_Deprecated(AActor* Actor, USaveGame* Data)
{
    TInlineComponentArray<UActorComponent*> Components;
    Actor->GetComponents(Components);
    
    for(UActorComponent* Comp : Components)
    {
        if(IDataSaveInterface* LoadingInterface = Cast<IDataSaveInterface>(Comp))
        {
            if(LoadingInterface->Load(Data))
            {
                UE_LOG(LogTemp, Warning, TEXT("LoadComponent: %s data loaded successfully.."), *LoadingInterface->_getUObject()->GetName());
            }
        }
    }
}

