// Copyright 2023 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/Components/POGroupThrowingCollision.h"
#include "ProjectOdimh/POGameInstance.h"


UPOGroupThrowingCollision::UPOGroupThrowingCollision()
{
	SetGenerateOverlapEvents(true);
}

bool UPOGroupThrowingCollision::IsReadyToBroadcast() const
{
	return CollisionList.Num() > 0 ?  true : false;
}

void UPOGroupThrowingCollision::ClearPendingBroadcastList()
{
	PendingBroadcastList.Empty();
}

void UPOGroupThrowingCollision::BeginPlay()
{
	Super::BeginPlay();
	
	OnComponentBeginOverlap.AddDynamic(this, &UPOGroupThrowingCollision::OnBeginOverlap);
}

void UPOGroupThrowingCollision::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsActorBeingDestroyed())
	{
		return;
	}
	
	if(DelayBeforeBroadcast < 0.f)
	{
		PendingBroadcastList.Add(OtherActor);
	}
	else
	{
		CollisionList.Add(OtherActor);
		
		FTimerDelegate ThrowTimerDelegate;
		ThrowTimerDelegate.BindUFunction(this, FName("BroadcastCollision"));

		GetOwner()->GetWorldTimerManager().SetTimer(
			BroadcastTimerHandle, ThrowTimerDelegate, DelayBeforeBroadcast, false);
	}
}

void UPOGroupThrowingCollision::BroadcastCollision()
{
	if(!IsReadyToBroadcast())
		return;

	GetOwner()->GetWorldTimerManager().ClearTimer(BroadcastTimerHandle);

	OnActorsCollided.Broadcast(GetOwner(), CollisionList);
	
	CollisionList.Empty();
}

void UPOGroupThrowingCollision::ReadyActorsForBroadcast()
{
	CollisionList.Append(PendingBroadcastList);
	PendingBroadcastList.Empty();
}
