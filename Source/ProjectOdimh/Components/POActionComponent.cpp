


#include "ProjectOdimh/Components/POActionComponent.h"
#include "ProjectOdimh/Entities/Game/POAction.h"

// Sets default values for this component's properties
UPOActionComponent::UPOActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPOActionComponent::AddAction(AActor* Instigator, TSubclassOf<UPOAction> ActionClass)
{
    if(!ensure(ActionClass))
    {
        return;
    }
    
    UPOAction* NewAction = NewObject<UPOAction>(GetOwner(), ActionClass);
    
    if(ensure(NewAction))
    {
        NewAction->Initialize(this);
        Actions.Add(NewAction);
    }
}

void UPOActionComponent::RemoveAction(UPOAction* ActionToRemove)
{
    if(ensure(ActionToRemove && !ActionToRemove->IsRunning()))
    {
        return;
    }
    
    Actions.Remove(ActionToRemove);
}

UPOAction* UPOActionComponent::GetAction(TSubclassOf<UPOAction> ActionClass)
{
    for(UPOAction* Action : Actions)
    {
        if(Action && Action->IsA(ActionClass))
        {
            return Action;
        }
    }
    
    return nullptr;
}

bool UPOActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
    for(UPOAction* Action : Actions)
    {
        if(Action && Action->Name == ActionName)
        {
            if(!Action->CanStart(Instigator))
            {
                continue;
            }

            Action->StartAction(Instigator);
            return true;
        }
        
        
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Cannot start action by name (actions not assigned?)"));
    
    return false;
}

bool UPOActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
    for(UPOAction* Action : Actions)
    {
        if(Action && Action->Name == ActionName)
        {
            if(Action->IsRunning())
            {
                Action->StopAction(Instigator);
                return true;
            }
        }
    }
    
    return false;
}

// Called when the game starts
void UPOActionComponent::BeginPlay()
{
	Super::BeginPlay();

    for(TSubclassOf<UPOAction> ActionClass : DefaultActions)
    {
        AddAction(GetOwner(), ActionClass);
    }
    
}


// Called every frame
void UPOActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

