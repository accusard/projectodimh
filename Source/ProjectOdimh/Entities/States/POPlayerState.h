// Copyright 2023 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectOdimh/Data/POSaveGame.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectOdimh/Data/Gametypes.h"
#include "POPlayerState.generated.h"



UCLASS()
class PROJECTODIMH_API APOPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	APOPlayerState();
	
	void SaveState(UPOSaveGame* SaveObject);

	void LoadState(UPOSaveGame* SaveObject);
	
	UPROPERTY(BlueprintReadWrite, SaveGame)
	FMatch3PlayerData PlayerData = FMatch3PlayerData();
};

