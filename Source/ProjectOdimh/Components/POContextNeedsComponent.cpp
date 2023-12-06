// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/Components/POContextNeedsComponent.h"

// Sets default values for this component's properties
UPOContextNeedsComponent::UPOContextNeedsComponent()
{
	
}

void UPOContextNeedsComponent::GetRequiredContextsRemaining(FGameplayTagContainer& TagContainer) const
{
	const FGameplayTagContainer SkipTags = FulfilledTags;
	
	for(const FGameplayTagContainerEntry Entry : RequiresTags)
	{
		const FGameplayTagContainer CheckRequiredTags = Entry.Container;

		for(FGameplayTag T : CheckRequiredTags)
		{
			if(SkipTags.HasTag(T))
			{
				continue;
			}

			TagContainer.AddTag(T);
		}
	}

}

void UPOContextNeedsComponent::FulfillContext(const FGameplayTag& TagToFulfill)
{
	FGameplayTagContainer Requirements;
	GetRequiredContextsRemaining(Requirements);
		
	if(Requirements.HasTag(TagToFulfill) && !FulfilledTags.HasTag(TagToFulfill))
	{
		FulfilledTags.AddTag(TagToFulfill);
	}
}

bool UPOContextNeedsComponent::HasFulfilledContext(const FGameplayTag& InTag) const
{
	FGameplayTagContainer Requirements;
	GetRequiredContextsRemaining(Requirements);
		
	return Requirements.HasTag(InTag) && FulfilledTags.HasTag(InTag);
}

void UPOContextNeedsComponent::AddContextsEntry(const FName& Label, const FGameplayTagContainer& TagsToAppend, const FString& Description)
{
	if(Label != NAME_None)
	{
		const FName& LabelToFind = Label;
	
		for(FGameplayTagContainerEntry MyEntry : RequiresTags)
		{
			if(MyEntry.GroupLabel == LabelToFind)
			{
				MyEntry.Container.AppendTags(TagsToAppend);
				return;
			}
		}
		
		const FName& NewLabel = Label;
	
		FGameplayTagContainerEntry NewEntry;
		NewEntry.GroupLabel = NewLabel;
		NewEntry.Container = TagsToAppend;
		NewEntry.Description = Description;
		RequiresTags.Add(NewEntry);
	}
}

void UPOContextNeedsComponent::ApplyContexts(const AActor* TargetActor, const FGameplayTagContainerEntry& ContextEntry)
{
	if(UPOContextNeedsComponent* Component = TargetActor->FindComponentByClass<UPOContextNeedsComponent>())
	{
		Component->AddContextsEntry(ContextEntry.GroupLabel, ContextEntry.Container, ContextEntry.Description);
	}	
}

bool UPOContextNeedsComponent::HasAllContextsFulfilled() const
{
	FGameplayTagContainer Requirements;
	
	for(FGameplayTagContainerEntry Entry : RequiresTags)
	{
		Requirements.AppendTags(Entry.Container);
	}
	
	return FulfilledTags.HasAllExact(Requirements);
}

void UPOContextNeedsComponent::UpdateContext(const FName& Label, const FGameplayTag& TagToAdd, const FGameplayTag& TagToRemove)
{
	AddContextsEntry(Label, FGameplayTagContainer(TagToAdd));
	
	for(FGameplayTagContainerEntry Entry : RequiresTags)
	{
		Entry.Container.RemoveTag(TagToRemove);
	}

	FulfilledTags.RemoveTag(TagToRemove);
}
