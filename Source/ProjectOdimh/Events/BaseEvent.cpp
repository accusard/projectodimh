// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#include "ProjectOdimh/Events/BaseEvent.h"
#include "ProjectOdimh/POGameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"




UBaseEvent::UBaseEvent()
{
    bPendingFinish = false;
    bStarted = false;
    
    THRESHOLD_EVENT = F_THRESHOLD_EVENT;
    INIT_TILE_SPAWN_EVENT = F_INIT_TILE_SPAWN_EVENT;
    GRID_STATE_CHANGE_EVENT = F_GRID_STATE_CHANGE_EVENT;
    GAME_HAS_STARTED_EVENT = F_GAME_HAS_STARTED_EVENT;
    TURN_ENDING_EVENT = F_TURN_ENDING_EVENT;
}

UBaseEvent::~UBaseEvent()
{
    
}

void UBaseEvent::Init()
{
    // GameInstance = GetWorld()->GetGameInstance<UPOGameInstance>();
    // Manager = GameInstance->GetSubsystem<UEventManager>();
    // GameMode = UGameplayStatics::GetGameMode(GetWorld());
}

const bool UBaseEvent::Is(const FName EventId) const
{
    return GetFName() == EventId;
}

void UBaseEvent::Start()
{
    bPendingFinish = true;
    bStarted = true;
    OnEventStart();
}

const bool UBaseEvent::IsPendingFinish() const
{
    return bPendingFinish;
}

const bool UBaseEvent::HasStarted() const
{
    return bStarted;
}

void UBaseEvent::End()
{

    if(bPendingFinish)
    {
        bPendingFinish = false;
        OnEventEnd();
        MarkAsGarbage();
    }
}

void UBaseEvent::Reset()
{
    bPendingFinish = true;
    bStarted = false;
}

UObject* UBaseEvent::GetOwner() const
{
    return GetOuter();
}



