#pragma once

#include "NativeGameplayTags.h"
#include "POGameplayTags.generated.h"

/**
 * A struct to wrap around a FGameplayTagContainer with an identifying GroupLabel.
 */
USTRUCT(BlueprintType)
struct FGameplayTagContainerEntry
{
	GENERATED_BODY()
	
	// The label for this GameplayTagContainer entry;
	UPROPERTY(EditAnywhere, Category=GameplayTag)
	FName GroupLabel;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=GameplayTag)
	FGameplayTagContainer Container;

#if WITH_EDITORONLY_DATA
	// Custom description for editor only
	UPROPERTY(EditAnywhere, Category=GameplayTag)
	FString Description;
#endif
	
};

// PROTOTYPE
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_PROTOTYPE_CONTEXT_STAGE_LOADED)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_PROTOTYPE_CONTEXT_STAGE_PENDING)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_PROTOTYPE_CONTEXT_STAGE_READY)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_PROTOTYPE_CONTEXT_STAGE_SLOTS_FULFILLED)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_PROTOTYPE_CONTEXT_COMBAT_REQUIRED_COMBATANTS)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_PROTOTYPE_FIELD)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_PROTOTYPE_FIELD_BATTLE)

// ALPHA
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ALPHA_FIELD)