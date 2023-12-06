

#include "ProjectOdimh/Entities/Game/POAction.h"
#include "ProjectOdimh/Components/POActionComponent.h"

void UPOAction::StartAction_Implementation(AActor* Instigator)
{
    bIsRunning = true;
    GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);
    const FString Msg = FString::Printf(TEXT("Starting %s"), *Name.ToString());
    GEngine->AddOnScreenDebugMessage(-1, 3.f,FColor::Green, Msg);
}
    
void UPOAction::StopAction_Implementation(AActor* Instigator)
{
    bIsRunning = false;
    GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);
    const FString Msg = FString::Printf(TEXT("Stopping %s"), *Name.ToString());
    GEngine->AddOnScreenDebugMessage(-1, 3.f,FColor::Green, Msg);
}

bool UPOAction::CanStart_Implementation(AActor* Instigator)
{
    if(IsRunning())
    {
        return false;
    }
    
    if(GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags))
    {
        return false;
    }
    
    return true;
}

bool UPOAction::IsRunning() const
{
    return bIsRunning;
}

void UPOAction::Initialize(UPOActionComponent* NewComp)
{
    ActionComp = NewComp;
}

UPOActionComponent* UPOAction::GetOwningComponent() const
{
    return ActionComp;
}
