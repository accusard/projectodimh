// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#include "GridController.h"
#include "ProjectOdimh/POGameInstance.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
#include "ProjectOdimh/Entities/Game/Grid.h"
#include "ProjectOdimh/Components/ActorPickHandlerComponent.h"
#include "ProjectOdimh/Components/ActionTurnBasedComponent.h"
#include "ProjectOdimh/Entities/Game/Tile.h"
#include "ProjectOdimh/Subsystems/EventManager.h"


AGridController::AGridController()
{
    TileHandlerComponent = CreateDefaultSubobject<UActorPickHandlerComponent>("Tile Handler Component");
    ActionComp = CreateDefaultSubobject<UActionTurnBasedComponent>("Turn-Based Action Handler");
    
    TileSpeed = DEFAULT_TILE_SPEED;
    
    static ConstructorHelpers::FObjectFinder<USoundCue> DefaultGrabSoundCue(TEXT("SoundCue'/Game/Interface/The_Future_Is_Now/cues/1_Neutral/UI_Neutral_173_Cue.UI_Neutral_173_Cue'"));
    
    static ConstructorHelpers::FObjectFinder<USoundCue> DefaultReleaseSoundCue(TEXT("SoundCue'/Game/Interface/The_Future_Is_Now/cues/1_Neutral/UI_Neutral_205_Cue.UI_Neutral_205_Cue'"));
    
    if(DefaultGrabSoundCue.Object)
        PickCue = DefaultGrabSoundCue.Object;
    
    if(DefaultReleaseSoundCue.Object)
        ReleaseCue = DefaultReleaseSoundCue.Object;
    
}

// Called every frame
void AGridController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGridController::BeginPlay()
{
    Super::BeginPlay();
    
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    Instance->GetSubsystem<UEventManager>()->Delegates.OnActorPicked.AddDynamic(this, &AGridController::OnHandlePick);
    
    GetOnNewPawnNotifier().AddUObject(this, &AGridController::OnBeginGridCheck);
}

UActorPickHandlerComponent* AGridController::GetPickHandler()
{
    return TileHandlerComponent;
}

EDirection AGridController::GetRandomDirection()
{
    const int32 Max = (int32)EDirection::MaxDir - 1;
    EDirection Direction = static_cast<EDirection>(FMath::RandRange(0, Max));
    return Direction;
    
}

ATile* AGridController::PickRandomTile(AGrid* Grid)
{
    TArray<ATile*> Tiles = Grid->GetTiles();
    uint32 RandomNum = FMath::RandRange(0, Tiles.Num() - 1);
    AActor* RandomTile = Tiles[RandomNum];
    
    return Cast<ATile>(GrabActor(this, GetPickHandler(), RandomTile, RandomTile->GetActorLocation()));
}

