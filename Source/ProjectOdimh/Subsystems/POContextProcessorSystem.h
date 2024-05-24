// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectOdimh/POGameplayTags.h"
#include "ProjectOdimh/Events/BaseEvent.h"
#include "POContextProcessorSystem.generated.h"

class UConflictResult;
class UCombatModeInterface;

/**
 *
 */
UCLASS(Blueprintable)
class PROJECTODIMH_API UPOContextProcessorSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UGameInstanceSubsystem* GetConflictInterface() const;
	
	// request class (i.e Match3CombatGrid) from the FIRST subsystem that implements combat interface
	UFUNCTION()
	TSubclassOf<AActor> GetConflictFieldClass() const;

private:
	UPROPERTY(EditAnywhere, Category=Combat);
	FGameplayTagContainerEntry AppliesConflictContext;
};
