// Copyright 2023 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/Entities/Controllers/Player/POWorldPlayerController.h"

#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/Entities/Game/POStage.h"
#include "ProjectOdimh/Entities/Game/Modes/POGameMode.h"
#include "ProjectOdimh/Subsystems/EventManager.h"


APOWorldPlayerController::APOWorldPlayerController()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

void APOWorldPlayerController::ToggleStageControl()
{
    if(GetPawn()->IsA<APOStage>())
    {
        UnPossess();
    }
    else if(const APOGameMode* GameMode = Cast<APOGameMode>(GetWorld()->GetAuthGameMode()))
    {
        // if(APawn* Stage = GameMode->GetActiveStage())
        // {
        //     Possess(Stage);
        // }
    }
}

void APOWorldPlayerController::HandleActorPicked(AActor* InPickedActor)
{
    PickedActor = InPickedActor;
}

void APOWorldPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if(PickedActor)
    {
        
    }
}

void APOWorldPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    // Instance->GetSubsystem<UEventManager>()->Delegates.OnActorPicked.AddDynamic(this, &APOWorldPlayerController::HandlePick);
}
