// Copyright 2023 Vanny Sou. All Rights Reserved.


#include "Match3Grid.h"

#include "ProjectOdimh/Data/Points.h"
#include "ProjectOdimh/Entities/Game/Tile.h"


// Sets default values
AMatch3Grid::AMatch3Grid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMatch3Grid::OnBurstEnd_Implementation(const int32 NumMatchingTiles)
{
	CurrentScore->Add(NumMatchingTiles);
}

void AMatch3Grid::RandomizeTiles(TSubclassOf<ATile> Class)
{
	if(Class)
	{
		if(GetGridTiles().Num() > 0)
		{
			ClearTiles();
		}
		
		// orientation top-down
		const float Offset = GetTileSize() / 2.f;
		const FVector GridOffset = FVector(Offset, Offset, 0.f);
		const FVector& GridLocation = GetActorLocation();

		int x = 0;
		int y = 0;
		
		while(x < GetColumnGrids().Num())
		{
			const FVector Column = GetColumnGrids()[x];
			
			while(y < GetRowGrids().Num())
			{
				const FVector Row = GetRowGrids()[y];
				
				const int32 RandomType = GetRandomTileType();
				
				const FVector TilePos = Row + Column + GridLocation + GridOffset;
				const FVector TileScale = FVector(GetTileScale());
				const FRotator TileRotation = FRotator();
			
				const FTransform TileTransform = FTransform(TileRotation, TilePos, TileScale);

				ATile* Tile = SpawnTile(Class, TileTransform, RandomType);
				FVector2D Coords = FVector2D(x, y);
				
				RegisterPosition(Tile, Coords);
				y++;
			}
			y = 0;
			x++;
		}
	}
}

ATile* AMatch3Grid::SpawnTile(TSubclassOf<ATile> Class, const FTransform& Transform, const int Type)
{
	ATile* NewTile = GetWorld()->SpawnActor<ATile>(Class, Transform);
    
	if(NewTile)
	{
		NewTile->SetId(Type);
		NewTile->LoadSprite();
	}
	
	return NewTile;
}

void AMatch3Grid::HandleTilesSwapped(AController* GridController, ATile* DynamicTile, ATile* StaticTile)
{
	bTilesHaveSwapped = true;

	
}

FVector2D AMatch3Grid::GetTileCoords(const ATile* Tile)
{
	return GetTileCoords(Tile->GetActorLocation());
}

FVector2D AMatch3Grid::GetTileCoords(const uint32 TileIndex) const
{
	return FVector2D(TileIndex % GetGridSizeX(), TileIndex / GetGridSizeY());
}

// Called when the game starts or when spawned
void AMatch3Grid::BeginPlay()
{
	Super::BeginPlay();

	CurrentScore = GetWorld()->SpawnActor<APoints>();
	// RandomizeTiles(TileClass);
}

// Called every frame
void AMatch3Grid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

