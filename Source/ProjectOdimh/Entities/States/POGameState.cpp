// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#include "POGameState.h"
#include "ProjectOdimh/Data/Tier.h"

APOGameState::APOGameState() : bGameHasStarted(false)
{
    TierProgression = CreateDefaultSubobject<ATier>("TierProgression");
    TierProgression->SetThreshold(ATier::DEFAULT_NEXT_TIER);
    
}
