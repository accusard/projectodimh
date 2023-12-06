// Copyright 2022 Vanny Sou. All Rights Reserved.

#include "Gameplay.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/Subsystems/EventManager.h"



// Sets default values
AGameplay::AGameplay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AGameplay::Reset()
{
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    Instance->GetSubsystem<UEventManager>()->Delegates.OnActorEvent.AddDynamic(this, &AGameplay::Init);
}

void AGameplay::SetNumTicksBeforeRun(const uint32 Set)
{
    NumOfTicksBeforeRun = Set;
}

void AGameplay::Init_Implementation(AActor* Actor, UBaseEvent* EvtPtr)
{
    // unbind the Init event
    // blueprint must call to parent after it finishes initialization
    
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    Instance->GetSubsystem<UEventManager>()->Delegates.OnActorEvent.RemoveDynamic(this, &AGameplay::Init);
    
    // don't end event yet
    // EvtPtr->End();
}
