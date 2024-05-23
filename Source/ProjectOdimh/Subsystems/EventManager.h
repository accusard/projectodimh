// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectOdimh/Events/BaseEvent.h"
#include "ProjectOdimh/Components/POActorCollectorSphere.h"
#include "ProjectOdimh/Entities/Game/POStage.h"
#include "EventManager.generated.h"

class AQueue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGlobalWidgetDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickActorEvent, AActor*, HandleActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateNumSignature, AActor*, Trigger, const int, Num);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEventSignature, AActor*, Actor, UBaseEvent*, Event);

USTRUCT(BlueprintType)
struct FDelegates
{
    GENERATED_BODY();
    
    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FGlobalWidgetDelegate OnInteractWidget;
    
    UPROPERTY()
    FPickActorEvent OnActorReleased;
    
    UPROPERTY()
    FPickActorEvent OnActorPicked;
    
    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnEventSignature OnActorEvent;
    
    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FUpdateNumSignature OnTurnEnd;
    
    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FUpdateNumSignature OnScoreUp;
};

/**
 * Contains delegates used for multicasting
 */
UCLASS(Blueprintable)
class PROJECTODIMH_API UEventManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UEventManager();
    
    /** Implement this for initialization of instances of the system */
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    template<class T>
    T* NewEvent(UObject* Outer, const FName& Identifier, const bool bStartNow)
    {
        T* NewEvent = NewObject<T>(Outer, Identifier);
        if(UBaseEvent* Event = Cast<UBaseEvent>(NewEvent))
        {
            Event->Init();
            if(bStartNow) Event->Start();
            
            AddEvent(Event);
            
            return static_cast<T*>(NewEvent);
        }
        else
        {
            Cast<UObject>(NewEvent)->MarkAsGarbage();
#if !UE_BUILD_SHIPPING
            UE_LOG(LogTemp,Warning,TEXT("- Event Manager is attempting to add to event list but object %s is an invalid event so calling kill immediately."), *NewEvent->GetName());
#endif
            return nullptr;
        }
    }
    
    UFUNCTION(BlueprintCallable, Category="Events")
    UBaseEvent* NewEvent(TSubclassOf<UBaseEvent> Class, UObject* Outer, const FName& Identifier, const bool bStartNow);
    
    void InitEventQueue();
    
    void AddEvent(UBaseEvent* Event);
    
    void EndEvent(const FName EventId) const;
    
    UFUNCTION(BlueprintCallable, Category="Events")
    void ClearEventQueue(const bool bForceClear = false) const;
    
    const int32 GetNumEventsInQueue() const;
    
    const bool HasA(TSubclassOf<UBaseEvent> EventClass);

    UFUNCTION(BlueprintCallable)
    UBaseEvent* Find(TSubclassOf<UBaseEvent> Class);
    
    TArray<UBaseEvent*> FindAll(TSubclassOf<UBaseEvent> EventClass);
    
    void BindDelegateToSubsystems(FOnActorsCollectedAtPointSignature& Delegate) const;
    void BindDelegateToSubsystems(FOnStagingSignature& Delegate) const;
    void UnbindDelegateFromSubsystems(FOnStagingSignature& Stage) const;
    
    void BindDelegateToActor(FOnActorsCollectedAtPointSignature& Delegate, AActor* Actor) const;

    UPROPERTY(BlueprintReadOnly, Category="Match 3 Events")
    FDelegates Delegates;
    
private:
    /** A list of events that are pending processing */
    UPROPERTY()
    AQueue* EventQueue;
};
