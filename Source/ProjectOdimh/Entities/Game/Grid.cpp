// Copyright 2022 Vanny Sou. All Rights Reserved.

#include "Grid.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "ProjectOdimh/ProjectOdimh.h"
#include "ProjectOdimh/Entities/Game/Modes/Match3GameMode.h"
#include "ProjectOdimh/Entities/Game/Tile.h"
#include "ProjectOdimh/ClassInterface/PickHandlerInterface.h"
#include "ProjectOdimh/Components/ActorPickHandlerComponent.h"
#include "ProjectOdimh/Data/POSaveGame.h"
#include "ProjectOdimh/Events/GridEvent.h"
#include "Sound/SoundCue.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/Subsystems/EventManager.h"


FGridSpawningParameters::FGridSpawningParameters()
{
    bRandomTileType = true;
    bLoadSprites = true;
    SaveSlotName = "";
}

FGridSpawningParameters::FGridSpawningParameters(const bool bRandomizeTiles, const bool bInitSprites, const FString& SlotName)
{
    bRandomTileType = bRandomizeTiles;
    bLoadSprites = bInitSprites;
    SaveSlotName = SlotName;
}

// Sets default values
AGrid::AGrid()
{
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    TilesNeededForMatch = 3;
    bTilesHaveSwapped = false;
    bNoMatchingTiles = false;
    bGridStateChanged = false;
    
    static ConstructorHelpers::FObjectFinder<USoundCue> DefaultTileMatchCue(TEXT("SoundCue'/Game/Interface/The_Future_Is_Now/cues/1_Neutral/UI_Neutral_226_Cue.UI_Neutral_226_Cue'"));
    
    if(DefaultTileMatchCue.Object)
        TileMatchCue = DefaultTileMatchCue.Object;
    
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
    Super::BeginPlay();
    
    // UEventManager* EvtMgr = Cast<UPOGameInstance>(GetGameInstance())->EventManager;
    // EvtMgr->OnActorPicked.AddDynamic(this, &AGrid::SetOldLocation);
    // EvtMgr->OnActorReleased.AddDynamic(this, &AGrid::TilesChangedPosition);
    // EvtMgr->OnActorEvent.AddDynamic(this, &AGrid::OnTurnEnded);
    
}

void AGrid::Save(USaveGame* SaveData)
{
    if(UPOSaveGame* Data = Cast<UPOSaveGame>(SaveData))
    {
        // save the tile types
        for(ATile* Tile : GetTiles())
        {
            if(Tile)
            {
                // for each tile, assign types to save data
                Data->AddTile(Tile->Id);
            }
        }
    }
}

const bool AGrid::Load(USaveGame* LoadData)
{
    if(UPOSaveGame* Data = Cast<UPOSaveGame>(LoadData))
    {
        FGridSpawningParameters Params(false, true, Data->SaveSlotName);
        InitTiles(Params);
        check(GetTiles().Num() == Data->SavedGrid.GetNumberOfTiles());
        
        return true;
    }
    
    return false;
}

const FVector2D& AGrid::GetTileCoords(const FVector& Location)
{
    // call the blueprint library function to retrieve the data
    OnRetrieveGridLocation(Location);
    
    return GridLocation;
}

const FVector2D& AGrid::GetTileCoords(ATile* Tile)
{
    return GetTileCoords(Tile->GetActorLocation());
}

const FVector2D AGrid::GetTileCoords(const uint32 TileIndex)
{
    return FVector2D(TileIndex % SizeX, TileIndex / SizeX);
}

const TArray<FTileCount> AGrid::TallyAllTileTypes()
{
    TArray<FTileCount> TotalCount;
    
    for(auto* Tile : GetTiles())
    {
        if(Tile)
        {
            bool bCreateNewTally = true;
            
            // count each tile type
            for(FTileCount& CurrTileCount : TotalCount)
            {
                if(CurrTileCount.Type == Tile->Id)
                {
                    CurrTileCount.TotalNum++;
                    bCreateNewTally = false;
                    break;
                }
            }
            
            // create new data only if a certain type doesn't exist
            if(bCreateNewTally)
            {
                FTileCount NewCount;
                NewCount.Type = Tile->Id;
                NewCount.TotalNum++;
                TotalCount.Add(NewCount);
            }
        }
    }
    
    return TotalCount;
}

const int AGrid::GetNumOfOccurence(const int Type)
{
    int Count = 0;
    
    for(const FTileCount& CurrTileData : TallyAllTileTypes())
    {
        if(CurrTileData.Type == Type)
        {
            Count = CurrTileData.TotalNum;
            break;
        }
    }
    return Count;
}

const bool AGrid::MatchingTilesAvailable(int NumOfTileTypes)
{
    for(int type = 0; type < NumOfTileTypes; type++ )
    {
        int TileCount = GetNumOfOccurence(type);
        
        // return true if there are at least 1 available matches
        if(TileCount >= TilesNeededForMatch)
        {
            return true;
        }
    }
    
    return false;
}

const bool AGrid::HasTilePositionChanged(ATile* Tile)
{
    if(Tile)
    {
        const FVector2D TileCurrLocation = GetTileCoords(Tile);
        const FVector2D TileOldLocation = Tile->OldLocation;
        
        if(TileCurrLocation != TileOldLocation)
            return true;
    }
    
    return false;
}

void AGrid::NotifyGridStateChanged()
{
    const UPOGameInstance* Instance = GetGameInstance<UPOGameInstance>();
    
    Instance->GetSubsystem<UEventManager>()->NewEvent<UGridEvent>(this, F_GRID_STATE_CHANGE_EVENT, true);
    
    bGridStateChanged = false;
}

void AGrid::TilesChangedPosition(AActor* Actor)
{
    if(bGridStateChanged || HasTilePositionChanged(Cast<ATile>(Actor)))
        NotifyGridStateChanged();
}

void AGrid::SetOldLocation(AActor* Actor)
{
    if(ATile* Tile = Cast<ATile>(Actor))
        Tile->OldLocation = GetTileCoords(Tile);
}

const float AGrid::GetDistanceBetween(ATile* Tile, FVector2D OtherPosition)
{
    // determine the distance between its current position and its new position
    return FVector2D::Distance(FVector2D(Tile->GetActorLocation()), OtherPosition);
}

const float AGrid::GetDistanceBetween(ATile* TileA, ATile* TileB)
{
    return FVector::Distance(TileA->GetActorLocation(), TileB->GetActorLocation());
}

void AGrid::OnInitialTilesSpawned(TArray<ATile*> SpecialTiles)
{
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    
    UEventManager* EvtMgr = Instance->GetSubsystem<UEventManager>();
    
    UGridEvent* Event = EvtMgr->NewEvent<UGridEvent>(this, F_INIT_TILE_SPAWN_EVENT, true);
    
    // for(ATile* Tile : SpecialTiles)
    //     Event->ReservedTiles.Add(Tile);
    
    EvtMgr->Delegates.OnActorEvent.Broadcast(this, Event);
}

void AGrid::OnTurnEnded_Implementation(AActor* Source, UBaseEvent* Event)
{
    bTilesHaveSwapped = false;
    UBaseEvent* PrevMove = Event;
    
    if(AMatch3GameMode* Mode = Cast<AMatch3GameMode>(Event->GameMode))
    {
        Mode->StartMove(PrevMove);
        UE_LOG(LogTemp,Warning,TEXT("--> Grid::OnTurnEnded_Implementation"));
    }
    
}

void AGrid::HandleTilesSwapped(AController* GridController, ATile* DynamicTile, ATile* StaticTile)
{
    bTilesHaveSwapped = true;
    UActorPickHandlerComponent* Handler = Cast<IPickHandlerInterface>(GridController)->GetPickHandler();
    
    if(Handler)
    {
        Handler->AddHandledActor(DynamicTile);
        Handler->AddHandledActor(StaticTile);
    }
}

TArray<ATile*> AGrid::RandomizeNewTiles(TArray<ATile*> Old, TSubclassOf<ATile> Class)
{
    TArray<ATile*> NewTiles;
    for(ATile* T : Old)
    {
        const bool bRegisterNewPos = true;
        NewTiles.Add(SpawnTile(Class, T->GetActorTransform(), GetRandomMatchType(), bRegisterNewPos));
        T->Destroy();
        
    }
    return NewTiles;
}

void AGrid::NewGrid()
{
    FGridSpawningParameters Params;
    InitTiles(Params);
}

void AGrid::NotifyBoardStateChanged_Implementation(uint8 OldState, UGridEvent* GridEvent, const TArray<ATile*>& ReservingTiles)
{
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    Instance->GetSubsystem<UEventManager>()->Delegates.OnActorEvent.Broadcast(this, GridEvent);
    
    if(Cast<AMatch3GameMode>(GridEvent->GameMode))
    {
        constexpr uint8 PickState = 0;
        // if(EvtPtr->CurrentState == PickState)
        // {
        //     EvtPtr->End();
        //     // Mode->SaveCurrentGameState(EvtPtr->GameInstance, false);
        // }
    }
}

void AGrid::OnEventEnd()
{
    
}

void AGrid::SpawnTileToEmptyGrid_Implementation(ATile* Tile, const bool bNotifyStateChange)
{
    if(bNotifyStateChange)
    {
        const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
        Instance->GetSubsystem<UEventManager>()->NewEvent<UGridEvent>(this, F_GRID_STATE_CHANGE_EVENT, true);
    }
}

ATile* AGrid::SpawnTile(TSubclassOf<ATile> BlueprintClass, const FTransform& Transform, const int Type, const bool bRegisterPos)
{
    ATile* NewTile = GetWorld()->SpawnActor<ATile>(BlueprintClass, Transform);
    
    if(NewTile)
    {
        NewTile->SetId(Type);
        NewTile->LoadSprite();
        if(bRegisterPos)
            RegisterPosition(NewTile, GetTileCoords(Transform.GetLocation()));
    }
    
    return NewTile;
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

void AGrid::ResetAccumulatedMatchedTiles()
{
    TotalNumTilesMatchedThisTurn = 0;
}
