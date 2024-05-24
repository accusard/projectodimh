// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "POActorCollectorSphere.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorCollected, AActor*, AActor);

/**
 * Keeps a collection of Actors that overlapped this component and broadcast the collected
 *
 */
UCLASS(ClassGroup="Collision", meta=(DisplayName="Actor Collector Sphere", BlueprintSpawnableComponent))
class PROJECTODIMH_API UPOActorCollectorSphere : public USphereComponent
{
	GENERATED_BODY()


public:
	UPOActorCollectorSphere();

	/**
	 * 
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnActorCollected OnActorCollected;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

};
