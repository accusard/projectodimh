// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectOdimh/ClassInterface/PickHandlerInterface.h"
#include "GridPlayerController.generated.h"

class AMatch3Grid;
class USoundCue;
class UPlayerInputEvent;
class UActorPickHandlerComponent;
class UActionTurnBasedComponent;

/**
 * The GridPlayerController handles player input and communicate them to the Grid and Tile objects
 */
UCLASS(BlueprintType, Blueprintable)
class MATCH3COMBATRUNTIME_API AGridPlayerController : public APlayerController, public IPickHandlerInterface
{
	GENERATED_BODY()
	
public:
    AGridPlayerController();
    
    virtual void BeginPlay() override;
    
    UFUNCTION(BlueprintPure)
    virtual UActorPickHandlerComponent* GetPickHandler() override;
    
    UFUNCTION()
    void HandlePick(AActor* PickedTile);
    
    // InputComponent setups
    virtual void SetupInputComponent() override;
    
    /** Start the process of a touch event */
    void BeginTouch(ETouchIndex::Type FingerIndex, FVector Location);
	
    /** End the process of a touch event */
    void EndTouch(ETouchIndex::Type FingerIndex, FVector Location);

    AActor* GetLastTouched();
    
    void NewInput(const FName& Name, ETouchIndex::Type FingerIndex, FHitResult& Hit);
    
    void ForceRelease();
    
protected:
    UPROPERTY(BlueprintReadOnly)
    UActorPickHandlerComponent* TileHandlerComponent;
    
    UPROPERTY(BlueprintReadOnly)
    UActionTurnBasedComponent* ActionComp;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    USoundCue* PickCue;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    USoundCue* ReleaseCue;
    
    UPROPERTY(BlueprintReadWrite)
	AMatch3Grid* Match3Grid;

	UPROPERTY(BlueprintReadOnly)
	UPlayerInputEvent* TouchInputEvent;
	
private:
    
    
};
