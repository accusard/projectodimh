// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "POStage.h"
#include "POCharacter.h"
#include "POMapPoint.h"
#include "Modes/POGameMode.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/Components/POStagingComponent.h"
#include "ProjectOdimh/Data/POStageData.h"
#include "ProjectOdimh/Subsystems/POConflictSystem.h"


void APOStage::BeginPlay()
{
	Super::BeginPlay();

	ensure(GetOwner());

	if(const UPOConflictSystem* System = GetGameInstance()->GetSubsystem<UPOConflictSystem>())
	{
		if(const TSubclassOf<AActor> Class = System->GetModeClass())
		{
			FActorSpawnParameters Params;
			constexpr ESpawnActorCollisionHandlingMethod SpawnMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
			Params.Instigator = this;
			Params.SpawnCollisionHandlingOverride = SpawnMethod;
			GetWorld()->SpawnActor<AActor>(Class, GetTransform(), Params);
		}

		// TODO: set system for match3 grid, contestants, and wait for result
		// match3 will be responsible for notifying System after the session end
	}
}

void APOStage::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EmptyStage();
	
	if(EndPlayReason != EEndPlayReason::Quit)
	{
		// GetGameInstance()->GetSubsystem<UEventManager>()->UnbindDelegateFromSubsystems(OnStagingFinished);
	}
	
	Super::EndPlay(EndPlayReason);
}

void APOStage::EmptyStage() const
{
	StagingComponent->EmptyStage();
}

// Sets default values
APOStage::APOStage()
{
	StagingComponent = CreateDefaultSubobject<UPOStagingComponent>("Staging Component");
}

void APOStage::BeginDestroy()
{
	Super::BeginDestroy();

	if(IsValid(GetWorld()))
	{
		GetGameInstance<UPOGameInstance>()->AddOnScreenDebugMessage("Stage is being destroyed. \n This message was to verify that tha a spawned Stage is getting GCed with no PtrRef");
	}
}

void APOStage::Initialize(const UPOStageData* Data)
{
	StagingComponent->ActorsLabel = Data->ActorLabel;
	StagingComponent->Capacity = Data->MaxStageSlots;

	const TArray<AActor*> Actors = Data->ActorsOnStage;
	
	if(Data->bAutoStageActorsOnSpawn)
	{
		StagingComponent->StageActors(Actors);
	}
	
	AActor* AtPoint = Data->AssociatedStagingPoint;
	
	SetOwner(AtPoint);
	SetActorTransform(AtPoint->GetTransform());
	
	for(AActor* Actor : Actors)
	{
		if(const APOCharacter* Character = Cast<APOCharacter>(Actor))
		{
			if(Character->IsPlayerControlled())
			{
				if(APOGameMode* Mode = Cast<APOGameMode>(GetWorld()->GetAuthGameMode()))
				{
					Mode->SetActiveStage(this);
					break;
				}
			}
		}
	}
}

APOMapPoint* APOStage::GetMapPoint() const
{
	return Cast<APOMapPoint>(GetOwner());
}
