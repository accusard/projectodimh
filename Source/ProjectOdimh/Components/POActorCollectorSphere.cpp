// Copyright 2024 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/Components/POActorCollectorSphere.h"

#include "ProjectOdimh/Subsystems/EventManager.h"


UPOActorCollectorSphere::UPOActorCollectorSphere()
{
	SetGenerateOverlapEvents(true);
	InitSphereRadius(40.f);
}

void UPOActorCollectorSphere::BeginPlay()
{
	Super::BeginPlay();
	
	if(UStaticMeshComponent* Mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>())
	{
		TArray<UStaticMeshComponent*> Components;
		GetOwner()->GetComponents<UStaticMeshComponent>(Components);
		
		if(ensureMsgf(Components.Num() == 1,
			TEXT("Warning (%s) Only one StaticMeshComponent is allowed at the moment"),
			*GetOwner()->GetName()))
		{
			return;
		}
		
		GetOwner()->SetRootComponent(this);
		Mesh->SetupAttachment(this);
	}
		
	OnComponentBeginOverlap.AddDynamic(this, &UPOActorCollectorSphere::OnBeginOverlap);
}

void UPOActorCollectorSphere::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsActorBeingDestroyed())
	{
		return;
	}
	
	OnActorCollected.Broadcast(OtherActor);
}
