// Copyright 2023 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "POSubsystemsOverride.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta=(DisplayName="Subsystems"))
class PROJECTODIMH_API UPOSubsystemsOverride : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPOSubsystemsOverride() {}
	
	UPROPERTY(Config, EditAnywhere, Category="Combat System")
	TSoftClassPtr<AActor> ConflictFieldClass;
};
