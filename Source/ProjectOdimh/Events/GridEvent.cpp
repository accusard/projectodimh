// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#include "GridEvent.h"
#include "Engine/World.h"
#include "ProjectOdimh/Entities/Game/Grid.h"





void UGridEvent::UpdateGridState(const uint8 NewState, UGridEvent* OldEvent)
{
    CurrentState = NewState;

    if(OldEvent)
    {
        LastState = OldEvent->CurrentState;
        OldEvent->End();
    }
}

void UGridEvent::UpdateTilesState(const TArray<ATile*>& BurstingTiles, const TArray<ATile*>& ReservingTiles)
{
    this->TilesToBurst = BurstingTiles;
    this->ReservedTiles = ReservingTiles;
}
