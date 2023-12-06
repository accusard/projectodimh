// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#include "SelectButton.h"

#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/ProjectOdimh.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
#include "ProjectOdimh/Data/AssetLoader.h"
#include "ProjectOdimh/Subsystems/EventManager.h"


USelectButton::USelectButton()
{
    static ConstructorHelpers::FObjectFinder<USoundCue> SoundFile(*TILE_RELEASE);

    ReleaseSoundCue = SoundFile.Object;
}

void USelectButton::BroadcastEvent()
{
    if(const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>())
    {
        Instance->GetSubsystem<UEventManager>()->Delegates.OnInteractWidget.Broadcast();
    }
}
