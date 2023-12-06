// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectOdimh/ClassInterface/DataSaveInterface.h"
#include "ProjectOdimh/ClassInterface/TurnEventInterface.h"
#include "ProjectOdimh/Data/FGameStats.h"
#include "GameplayRunModeBase.generated.h"

UCLASS()
class PROJECTODIMH_API AGameplayRunModeBase : public AActor, public IDataSaveInterface, public ITurnEventInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameplayRunModeBase();

    // interfaces
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void OnTurnEnded(AActor* Source, UBaseEvent* Event);
    
    const int GetOnTickFrom(AActor* Gameplay);
    
    UFUNCTION(BlueprintCallable)
    void SetGameplayToTickOn(AActor* SetGameplay, const FGameStats& Tick);
    
    UFUNCTION()
    void StepTick(AActor* Gameplay, const int OnTick);
    
    UFUNCTION(BlueprintPure)
    const int GetTurnNumBeforeRun(AActor* CheckGameplay, const int CurrentTurn);
    
    const bool ShouldTick(AActor* CheckGameplay, const int OnTick);
    const TArray<AActor*> ShouldTick(const int OnTick);
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY()
    TArray<AActor*> TickingGameplays;
    
    UPROPERTY(BlueprintReadOnly, SaveGame)
    FGameStats RunCount = FGameStats(0,1);

};
