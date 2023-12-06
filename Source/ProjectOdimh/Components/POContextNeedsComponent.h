// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ProjectOdimh/POGameplayTags.h"
#include "POContextNeedsComponent.generated.h"


/**
 * Uses GameplayTags to track an actor's contextual requirements
 */
UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTODIMH_API UPOContextNeedsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPOContextNeedsComponent();

	// Note: Only tags that have not been fulfilled. Will NOT include any tags in the FulfilledTags container
	UFUNCTION(BlueprintPure, Category="Context Tags")
	void GetRequiredContextsRemaining(FGameplayTagContainer& TagContainer) const;

	UFUNCTION(BlueprintCallable, Category="Context Tags")
	void FulfillContext(const FGameplayTag& TagToFulfill);
	
	UFUNCTION(BlueprintPure, Category="Context Tags")
	bool HasFulfilledContext(const FGameplayTag& InTag) const;

	// Add a new entry to the RequiresTags, otherwise create a new set with the label supplied
	UFUNCTION(BlueprintCallable, Category="Context Tags")
	void AddContextsEntry(const FName& Label, const FGameplayTagContainer& TagsToAppend, const FString& Description = "");

	bool HasAllContextsFulfilled() const;
	
	void UpdateContext(const FName& Label, const FGameplayTag& TagToAdd, const FGameplayTag& TagToRemove);

	UFUNCTION(BlueprintCallable, Category="Context Tags")
	static void ApplyContexts(const AActor* TargetActor, const FGameplayTagContainerEntry& ContextEntry);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Context Tags")
	TArray<FGameplayTagContainerEntry> RequiresTags;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Context Tags")
	FGameplayTagContainer FulfilledTags;
};
