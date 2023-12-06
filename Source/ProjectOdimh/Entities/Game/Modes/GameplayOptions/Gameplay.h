// Copyright 2017-2021 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectOdimh/ClassInterface/DataSaveInterface.h"
#include "ProjectOdimh/ClassInterface/GameplayOptionsInterface.h"
#include "Gameplay.generated.h"

UCLASS()
class PROJECTODIMH_API AGameplay : public AActor, public IGameplayOptionsInterface, public IDataSaveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameplay();
    
    virtual void Reset() override;
	
	virtual void Init_Implementation(AActor* Actor, UBaseEvent* Evt) override;
    
    void SetNumTicksBeforeRun(const uint32 Set);
	FORCEINLINE uint32 GetNumOfTicksBeforeRun() const { return NumOfTicksBeforeRun; }
    
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame)
    int32 NumOfTicksBeforeRun;



    
};
