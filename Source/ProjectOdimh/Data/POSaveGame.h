

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FGameStats.h"
#include "Gametypes.h"
#include "POSaveGame.generated.h"

const FString DEFAULT_SAVE_SLOT = "DefaultSlotName";
const FString RESET_GAME_SLOT = "ResetLevel";
const FString CONTINUE_GAME_SLOT = "ContinueGame";
const FString LAST_SUCCESSFUL_SLOT = "LastSuccessfulSlot";

USTRUCT()
struct FActorSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    FName Name;

    UPROPERTY()
    FTransform Transform;

    UPROPERTY()
    TArray<uint8> ByteData;
};

/**
 * The struct that contain the save data of the combat board
 */
USTRUCT(BlueprintType)
struct FBoardSaveData
{
    GENERATED_USTRUCT_BODY()
    
public:
    FBoardSaveData() {}
        
    /** Get the total tiles that data contained */
    const int32 GetNumberOfTiles() const;
    
    /** Data to hold tile types */
    UPROPERTY(BlueprintReadOnly)
    TArray<int> TileTypes;
    
};

USTRUCT(BlueprintType)
struct FCustomIntData
{
    GENERATED_USTRUCT_BODY()
public:
    FCustomIntData() : FCustomIntData("", 0) {}
    FCustomIntData(const FString& Str, const int Val) : Id(Str), Value(Val) {}
    
    UPROPERTY(BlueprintReadWrite)
    FString Id;
    
    UPROPERTY(BlueprintReadWrite)
    int Value;
};

/**
 * An object that contains the save data of the game
 */
UCLASS()
class PROJECTODIMH_API UPOSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadOnly)
    FString DEFAULT;
    UPROPERTY(BlueprintReadOnly)
    FString RESET;
    UPROPERTY(BlueprintReadOnly)
    FString CONTINUE;
    UPROPERTY(BlueprintReadOnly)
    FString PREVIOUS;
    
    // default constructor
    UPOSaveGame();

    UPROPERTY()
    FString SaveSlotName;
    
    UPROPERTY()
    uint32 UserIndex;
    
    UPROPERTY()
    FMatch3PlayerData Player = FMatch3PlayerData();

    UPROPERTY()
    TArray<FActorSaveData> Actors;
    
public:
    /** To be deleted; transitioning to serialization */
    void AddTile(int Type);
    /** To be deleted; transitioning to serialization */
    void AddGameStatsInfo(const FString& SaveStr, const FGameStats& Stats);
    /** To be deleted; transitioning to serialization */
    void GetGameStatsInfo(const FString& LoadStr, FGameStats& Out);
    
    /** To be deleted; transitioning to serialization. The state of the board as well as the current score is kept here */
    UPROPERTY(BlueprintReadOnly)
    FBoardSaveData SavedGrid;
    
    /** To be deleted; transitioning to serialization. Data for the turn queue */
    UPROPERTY()
    TArray<FCustomIntData> ParticipantsRegistry;

    /** To be deleted; transitioning to serialization. */
    UPROPERTY()
    TMap<FString, int32> CustomInt;

    /** To be deleted; transitioning to serialization. */
    UPROPERTY(BlueprintReadWrite)
    TMap<FString, FIntPoint> CustomIntPoint;

};


