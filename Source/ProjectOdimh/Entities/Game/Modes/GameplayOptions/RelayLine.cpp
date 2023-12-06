// Copyright 2017-2019 Vanny Sou. All Rights Reserved.


#include "RelayLine.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/Entities/Game/Grid.h"
#include "ProjectOdimh/Entities/Game/Tile.h"
#include "ProjectOdimh/Subsystems/EventManager.h"


// Sets default values
ARelayLine::ARelayLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARelayLine::BeginPlay()
{
	Super::BeginPlay();

	const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
	
    UEventManager* EvtMgr = Instance->GetSubsystem<UEventManager>();
    EvtMgr->Delegates.OnActorEvent.AddDynamic(this, &ARelayLine::Init);
    EvtMgr->Delegates.OnActorEvent.AddDynamic(this, &ARelayLine::Receive);
}

TArray<ATile*> ARelayLine::GetUnusedTiles(AGrid* Grid)
{
    TArray<ATile*> UnusedTiles;
    for(FIntPoint Coord : ReservedGridCoords)
    {
        if(ATile* Tile = Grid->GetTile(Coord))
            if(Tile->GetIdString() == "Reserved")
                UnusedTiles.Add(Tile);
    }
    
    return UnusedTiles;
}
