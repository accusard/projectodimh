// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProjectOdimh/ClassInterface/DelegateBindingInterface.h"
#include "POGameMode.generated.h"


class UPOStageData;

/**
 * 
 */
UCLASS()
class PROJECTODIMH_API APOGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void HandleCollectedActor(AActor* Actor);

protected:
	virtual void BeginPlay() override;
	
public:
	//~ Begin Debug Commands
	UFUNCTION(Exec)
	void PODebugSave(const int32 PlayerIndex) const;

	UFUNCTION(Exec)
	void PODebugLoad(const int32 PlayerIndex) const;
	//~ End Debug Commands

};
