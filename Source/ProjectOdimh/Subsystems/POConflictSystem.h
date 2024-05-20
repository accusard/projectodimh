// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectOdimh/POGameplayTags.h"
#include "ProjectOdimh/Events/BaseEvent.h"
#include "POConflictSystem.generated.h"

class UConflictResult;
class UCombatModeInterface;

/**
 *
 */
UCLASS(Blueprintable)
class PROJECTODIMH_API UPOConflictSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 
	UFUNCTION()
	void OnThresholdMet(AActor* Actor, UBaseEvent* Event);
	
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UGameInstanceSubsystem* GetCombatSystem() const;
	
	// request class (i.e Match3CombatGrid) from the FIRST subsystem that implements combat interface
	UFUNCTION()
	TSubclassOf<AActor> GetConflictFieldClass() const;
	
	UObject* PickRandom(const TArray<UObject*>& List) const;

	
private:

	UPROPERTY(EditAnywhere, Category=Combat);
	FGameplayTagContainerEntry AppliesConflictContext;
};
