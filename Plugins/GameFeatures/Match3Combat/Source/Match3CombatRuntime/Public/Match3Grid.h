// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Match3Grid.generated.h"

class UGridEvent;
class ATile;
class APoints;

UCLASS()
class MATCH3COMBATRUNTIME_API AMatch3Grid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMatch3Grid();

	UPROPERTY(EditDefaultsOnly, Category=Grid)
	TSubclassOf<ATile> TileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Grid)
	float DelayBetweenTileBursts = 0.08f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Grid)
	int32 TilesNeededForMatch = 3;

	UPROPERTY(BlueprintReadWrite, Category=Grid)
	uint8 bTilesHaveSwapped : 1;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnBurstEnd(const int32 NumMatchingTiles);
	
	UFUNCTION(BlueprintImplementableEvent, Category=Grid)
	float GetTileSize() const;
	
	UFUNCTION(BlueprintImplementableEvent, Category=Grid)
	float GetTileScale() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category=Grid)
	TArray<ATile*> GetBurstingTiles() const;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category=Grid)
	int32 GetRandomTileType(const bool bUseWildTypes = false) const;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category=Grid)
	int32 GetNumOfTileTypes() const;
	
	UFUNCTION(BlueprintImplementableEvent, Category=Grid)
	TArray<FVector> GetRowGrids() const;

	UFUNCTION(BlueprintImplementableEvent, Category=Grid)
	TArray<FVector> GetColumnGrids() const;
	
	UFUNCTION(BlueprintCallable, Category=Grid)
	void RandomizeTiles(TSubclassOf<ATile> Class);
	
	UFUNCTION(BlueprintCallable, Category=Grid)
	ATile* SpawnTile(TSubclassOf<ATile> Class, const FTransform& Transform, const int Type);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Grid)
	void RegisterPosition(ATile* Tile, const FVector2D& Coord);

	UFUNCTION(BlueprintCallable)
	void HandleTilesSwapped(AController* GridController, ATile* DynamicTile, ATile* StaticTile);

	UFUNCTION(BlueprintImplementableEvent, Category=Grid)
	TArray<ATile*> GetGridTiles() const;
	
	UFUNCTION(BlueprintImplementableEvent, Category=Grid)
	int32 GetGridSizeX() const;

	UFUNCTION(BlueprintImplementableEvent, Category=Grid)
	int32 GetGridSizeY() const;
	
	UFUNCTION(BlueprintImplementableEvent, Category=Grid)
	FVector2D GetTileCoords(const FVector& Location);

	FVector2D GetTileCoords(const uint32 TileIndex) const;
	
	FVector2D GetTileCoords(const ATile* Tile);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category=Grid)
	void ClearTiles() const;
	
	UPROPERTY(BlueprintReadWrite, Category=Grid)
	UGridEvent* GridState;

	UPROPERTY(BlueprintReadWrite)
	APoints* CurrentScore;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	
};
