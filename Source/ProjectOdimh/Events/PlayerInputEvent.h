// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "ProjectOdimh/Events/BaseEvent.h"
#include "PlayerInputEvent.generated.h"





/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTODIMH_API UPlayerInputEvent : public UBaseEvent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	AActor* TouchResult;
	
    ETouchIndex::Type FingerIndex;
	
};
