// Copyright 2017-2019 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickHandlerInterface.generated.h"

class UActorPickHandlerComponent;
class ATile;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class PROJECTODIMH_API IPickHandlerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual UActorPickHandlerComponent* GetPickHandler() = 0;
    
    AActor* GetLastGrab(UActorPickHandlerComponent* Comp);
    AActor* GetActorPicked(UActorPickHandlerComponent* Comp);
    AActor* GrabActor(AActor* InterfaceHandler, UActorPickHandlerComponent* PickHandler, const FHitResult& Hit);
    AActor* GrabActor(AActor* InterfaceHandler, UActorPickHandlerComponent* PickHandler, AActor* TargetGrab, const FVector& GrabLocation);

    const bool IsActorPicked(UActorPickHandlerComponent* Comp);
    
protected:
    UFUNCTION(BlueprintImplementableEvent)
    void OnReleaseActor(AActor* InActor);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnPickActor(const FVector& Location);
};
