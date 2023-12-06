// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/Data/Points.h"
#include "ProjectOdimh/Subsystems/EventManager.h"
#include "ProjectOdimh/Events/GameEvent.h"
#include "ProjectOdimh/POGameInstance.h"

void APoints::NotifyPointsUp(const uint32 Value)
{
    const UPOGameInstance* Instance = GetGameInstance<UPOGameInstance>();
    Instance->GetSubsystem<UEventManager>()->Delegates.OnScoreUp.Broadcast(this, Value);
}

void APoints::NotifyPointsThreshold()
{
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    UEventManager* EvtMgr = Instance->GetSubsystem<UEventManager>();
    
    UGameEvent* GameEvt = EvtMgr->NewEvent<UGameEvent>(this, F_THRESHOLD_EVENT, true);
    EvtMgr->Delegates.OnActorEvent.Broadcast(this, GameEvt);
}

void APoints::Set(const uint32 Value)
{
    Points.Value = Value;
}

void APoints::Add(const int32 Val, const bool bNotePointsUp)
{
    Points.Value += Val;
    if(bNotePointsUp)
    {
        NotifyPointsUp(Val);
    
        if(Points.Value >= GetThreshold())
            NotifyPointsThreshold();
    }
}

const int32 APoints::GetTotalPoints() const
{
    return Points.Value;
}

void APoints::SetThreshold(const uint32 Value)
{
    Points.Default = Value;
}

const int32 APoints::GetThreshold() const
{
    return Points.Default;
}

const int32 APoints::GetDeltaPoints() const
{
    const uint32 Delta = GetThreshold() - GetTotalPoints();
    return Delta;
}


