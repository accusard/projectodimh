// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/Entities/Game/Modes/POGameMode.h"
#include "GameplayTagAssetInterface.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/ClassInterface/CombatModeInterface.h"
#include "ProjectOdimh/Data/POSaveGame.h"
#include "ProjectOdimh/Data/POStageData.h"
#include "ProjectOdimh/Entities/Game/POStage.h"
#include "ProjectOdimh/Entities/Game/POStagingInfo.h"
#include "ProjectOdimh/Subsystems/POConflictSystem.h"
#include "ProjectOdimh/Subsystems/POSaveGameSubsystem.h"

void APOGameMode::AddOnActorsCollisionHandler(FOnActorsCollisionSignature& OnCollision)
{
	OnCollision.AddDynamic(this, &APOGameMode::OnActorsArrivedAtPoint);
}

FPrimaryAssetId APOGameMode::GetStagingAssetId(const UDataTable* Table, const FGameplayTagContainer& TagContainer) const
{
	if(Table)
	{
		TArray<FStageInfoRow*> Rows;
		Table->GetAllRows<FStageInfoRow>("", Rows);

		// get first available qualifications
		for(uint32 i = 0; i < Rows.Num(); ++i)
		{
			if(TagContainer.HasAll(Rows[i]->Prerequisites))
			{
				FPrimaryAssetId Id = Rows[i]->Id;
				return Id;
			}
		}
	}
	
	return FPrimaryAssetId();
}

void APOGameMode::OnActorsArrivedAtPoint(AActor* AtPoint, const TArray<AActor*>& NewArrivals)
{
	if(IsValid(AtPoint))
	{
		if(const IGameplayTagAssetInterface* Point = Cast<IGameplayTagAssetInterface>(AtPoint))
		{
			Point->GetOwnedGameplayTags(StagingRequirements);
		}
		
		for(AActor* Actor : NewArrivals)
		{
			PendingActorsToBeStaged.Add(Actor);
			
			if(const IGameplayTagAssetInterface* Interface = Cast<IGameplayTagAssetInterface>(Actor))
			{
				Interface->GetOwnedGameplayTags(StagingRequirements);
			}
		}

		const FPrimaryAssetId Id = GetStagingAssetId(StagingTable, StagingRequirements);
			
		if(Id.PrimaryAssetName != NAME_None)
		{
			if(UAssetManager* AM = UAssetManager::GetIfValid())
			{
				const TArray<FName> Bundles;
				const FStreamableDelegate Delegate = FStreamableDelegate::CreateUFunction(this, 
					"OnStagingLoaded", Id, AtPoint, PendingActorsToBeStaged);

				AM->LoadPrimaryAsset(Id, Bundles, Delegate);

				PendingActorsToBeStaged.Empty();
				StagingRequirements.Reset();
			}
		}
	}
}

void APOGameMode::OnStagingLoaded(const FPrimaryAssetId Id, AActor* AtPoint, const TArray<AActor*> ActorsToBeStaged)
{
	if(const UAssetManager* AssetManager = UAssetManager::GetIfValid())
	{
		if(UPOStageData* Data = Cast<UPOStageData>(AssetManager->GetPrimaryAssetObject(Id)))
		{
			Data->AssociatedStagingPoint = AtPoint;
			Data->ActorsOnStage = ActorsToBeStaged;
			
			if(const APOStage* NewStage = Cast<APOStage>(SpawnStage(Data)))
			{
				if(UPOConflictSystem* ConflictSystem = GetGameInstance()->GetSubsystem<UPOConflictSystem>())
				{
					ensureMsgf(ConflictSystem, TEXT("CombatSystem is not valid!"));
					if(const ICombatModeInterface* Interface = Cast<ICombatModeInterface>(ConflictSystem->GetConflictInterface()))
					{
						// Spawn the conflict field mode
						if(const TSubclassOf<AActor> FieldClass = ConflictSystem->GetConflictFieldClass())
						{
							FActorSpawnParameters Params;
							Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
							UConflictResult* r = Interface->ResolveConflict(ActorsToBeStaged);
							GetWorld()->SpawnActor<AActor>(FieldClass, NewStage->GetTransform(), Params);
							UPOGameInstance::AddOnScreenDebugMessage("YAAY! Spawned the conflict field!");
						}
					}
				}
			}
			
		}
	}
}

void APOGameMode::SetActiveStage(APawn* StageToSet)
{
	ActiveStage = StageToSet;

	// TODO: should set camera
}

void APOGameMode::PODebugSave(const int32 PlayerIndex) const
{
	GetGameInstance<UGameInstance>()->GetSubsystem<UPOSaveGameSubsystem>()->WriteSave(CONTINUE_GAME_SLOT, PlayerIndex);
}

void APOGameMode::PODebugLoad(const int32 PlayerIndex) const
{
	GetGameInstance<UGameInstance>()->GetSubsystem<UPOSaveGameSubsystem>()->ReadSave(CONTINUE_GAME_SLOT, PlayerIndex);
}

APawn* APOGameMode::SpawnStage(const UPOStageData* Data) const
{
	if(Data == nullptr)
	{
		return nullptr;
	}
	
	constexpr ESpawnActorCollisionHandlingMethod SpawnMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	APOStage* NewStage = Cast<APOStage>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), Data->StageClass,
		FTransform(), SpawnMethod));
	
	NewStage->Initialize(Data);
	
	UGameplayStatics::FinishSpawningActor(NewStage, NewStage->GetTransform());
	
	if(NewStage->IsStageFull())
	{
		NewStage->OnStagingFinished.Broadcast();
	}
	
	return NewStage;
}

void APOGameMode::BeginPlay()
{
	Super::BeginPlay();


	
}
