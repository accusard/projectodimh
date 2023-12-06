// Copyright 2022 Vanny Sou. All Rights Reserved.

#include "EventManager.h"
#include "Engine/World.h"
#include "ProjectOdimh/ClassInterface/DelegateBindingInterface.h"
#include "ProjectOdimh/Events/BaseEvent.h"
#include "ProjectOdimh/Entities/Game/Queue.h"


// Sets default values for this component's properties
UEventManager::UEventManager()
{
    EventQueue = CreateDefaultSubobject<AQueue>("Event Queue");
    InitEventQueue();
}

void UEventManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

UBaseEvent* UEventManager::NewEvent(TSubclassOf<UBaseEvent> Class, UObject* Outer, const FName& Identifier, const bool bStartNow)
{
    UBaseEvent* Event = NewObject<UBaseEvent>(Outer, Class, Identifier);
    
    Event->Init();
    if(bStartNow) Event->Start();
        
    AddEvent(Event);
    
    return Event;
}

const int32 UEventManager::GetNumEventsInQueue() const
{
    return EventQueue->GetNumObjects();
    
}

const bool UEventManager::HasA(TSubclassOf<UBaseEvent> EventClass)
{
    bool HasEvent = false;
    
    for(int i = 0; i < EventQueue->GetNumObjects(); i++)
    {
        if(UObject* EventPtr = EventQueue->GetIndex(i))
        {
            if(EventPtr->IsA(EventClass))
            {
                HasEvent = true;
                break;
            }
        }
    }
    
    return HasEvent;
}

UBaseEvent* UEventManager::Find(const TSubclassOf<UBaseEvent> Class)
{
    for(int i = 0; i < EventQueue->GetNumObjects(); i++)
    {
        if(UBaseEvent* EventPtr = Cast<UBaseEvent>(EventQueue->GetIndex(i)))
        {
            if(EventPtr->IsA(Class) && IsValid(EventPtr))
            {
                return EventPtr;
            }
        }
    }
    
    return nullptr;
}

TArray<UBaseEvent*> UEventManager::FindAll(TSubclassOf<UBaseEvent> EventClass)
{
    TArray<UBaseEvent*> Events;
    for(int i = 0; i < EventQueue->GetNumObjects(); i++)
    {
        if(UBaseEvent* EventPtr = Cast<UBaseEvent>(EventQueue->GetIndex(i)))
        {
            if(EventPtr->IsA(EventClass))
                Events.Add(EventPtr);
        }
    }
    return Events;
}

void UEventManager::InitEventQueue()
{
    if(!EventQueue)
        EventQueue = GetWorld()->SpawnActor<AQueue>();
    
}

void UEventManager::AddEvent(UBaseEvent* Event)
{
    if(EventQueue)
        EventQueue->AddToList(Event);
    else
    {
        Event->End();
        Event->MarkAsGarbage();
#if !UE_BUILD_SHIPPING
        UE_LOG(LogTemp,Warning,TEXT("- Couldn't add event %s, so calling End() and kill immediately."), *Event->GetName());
#endif
    }
}

void UEventManager::EndEvent(const FName EventId) const
{
    UE_LOG(LogTemp,Warning,TEXT("EndEvent object contain - %i"), EventQueue->GetNumObjects());
    
    const int Size = EventQueue->GetNumObjects();
    
    for(int i = 0; i < Size; ++i)
    {
        if(UBaseEvent* EvtPending = Cast<UBaseEvent>(EventQueue->GetIndex(i)))
        {
            if(EvtPending->Is(EventId))
            {
                UE_LOG(LogTemp,Warning,TEXT("Ending Event (%s)"), *EvtPending->GetName());
                EvtPending->End();
            }
        }
    }
}

void UEventManager::ClearEventQueue(const bool bForceClear) const
{
    for(int i = 0; i < EventQueue->GetNumObjects(); ++i)
    {
        UObject* Obj = EventQueue->GetIndex(i);
        if(IsValid(Obj))
        {
            if(UBaseEvent* Evt = Cast<UBaseEvent>(Obj))
                if(!bForceClear && Evt->IsPendingFinish())
                    continue;
            Obj->MarkAsGarbage();
        }
        
    }
#if !UE_BUILD_SHIPPING
    UE_LOG(LogTemp,Warning,TEXT("Clearing event queue: %i"), EventQueue->GetNumObjects());
#endif
    EventQueue->EmptyList();
}


void UEventManager::BindDelegateToSubsystems(FOnActorsCollisionSignature& Delegate) const
{
    const TArray<UGameInstanceSubsystem*>& MySubsystems = GetGameInstance()->GetSubsystemArray<UGameInstanceSubsystem>();
    
    for(UGameInstanceSubsystem* System : MySubsystems)
    {
        if(System->Implements<UDelegateBindingInterface>())
        {
            IDelegateBindingInterface* Interface = Cast<IDelegateBindingInterface>(System);
            Interface->AddOnActorsCollisionHandler(Delegate);
        }
    }
}

void UEventManager::BindDelegateToSubsystems(FOnStagingSignature& Delegate) const
{
    const TArray<UGameInstanceSubsystem*>& MySubsystems = GetGameInstance()->GetSubsystemArray<UGameInstanceSubsystem>();
    
    for(UGameInstanceSubsystem* System : MySubsystems)
    {
        if(System->Implements<UDelegateBindingInterface>())
        {
            IDelegateBindingInterface* Interface = Cast<IDelegateBindingInterface>(System);
            Interface->AddOnRequestStagingContextHandler(Delegate);
        }
    }
}

void UEventManager::UnbindDelegateFromSubsystems(FOnStagingSignature& Stage) const
{
    const TArray<UGameInstanceSubsystem*>& MySubsystems = GetGameInstance()->GetSubsystemArray<UGameInstanceSubsystem>();
    
    for(UGameInstanceSubsystem* System : MySubsystems)
    {
        if(System->Implements<UDelegateBindingInterface>())
        {
            IDelegateBindingInterface* Interface = Cast<IDelegateBindingInterface>(System);
            Interface->RemoveStagingContextHandler(Stage);
        }
    }
}

void UEventManager::BindDelegateToActor(FOnActorsCollisionSignature& Delegate, AActor* Actor) const
{
    if(Actor->Implements<UDelegateBindingInterface>())
    {
        IDelegateBindingInterface* Interface = Cast<IDelegateBindingInterface>(Actor);
        Interface->AddOnActorsCollisionHandler(Delegate);
    }
}