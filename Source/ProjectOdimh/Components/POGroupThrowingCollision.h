// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "POGroupThrowingCollision.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorsCollisionSignature, AActor*, AtPoint, const TArray<AActor*>&, ActorsInCollision);

/**
 * Keeps a collection of Actors that overlapped this component and broadcast the collected
 * array of actors. Also keep a PendingBroadcastList for actors that are not to be broadcasted
 */
UCLASS(ClassGroup="Collision", meta=(DisplayName="Group Collision", BlueprintSpawnableComponent))
class PROJECTODIMH_API UPOGroupThrowingCollision : public USphereComponent
{
	GENERATED_BODY()


public:
	UPOGroupThrowingCollision();
	
	FORCEINLINE const TArray<AActor*>& GetPendingBroadcastList() const { return PendingBroadcastList; }
	
	bool IsReadyToBroadcast() const;
	
	void ClearPendingBroadcastList();

	/**
	 * Move actors in PendingBroadcastList to the CollisionList for broadcasting
	 */
	void ReadyActorsForBroadcast();
	
	/**
	 * This function will clear the component's CollisionList
	 */
	UFUNCTION()
	void BroadcastCollision();

	/**
	 * 
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnActorsCollisionSignature OnActorsCollided;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category="Group Collision")
	TArray<AActor*> PendingBroadcastList;

	UPROPERTY(VisibleAnywhere, Category="Group Collision")
	TArray<AActor*> CollisionList;
	
	FTimerHandle BroadcastTimerHandle;

	/** 
	 * Delay seconds before moving overlapped actors to owning component's position.
	 * Set < 0 to not throw (broadcast OnThrowActors delegate) immediately and add to a PendingThrows list
	 * Timer resets if another overlap event is detected
	 */
	UPROPERTY(EditAnywhere, Category="Group Collision")
	float DelayBeforeBroadcast = 1.5f;

};
