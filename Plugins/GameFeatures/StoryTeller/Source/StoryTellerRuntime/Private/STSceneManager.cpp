// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "STSceneManager.h"
#include "Engine/AssetManager.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/Entities/Game/POStagingInfo.h"
#include "ProjectOdimh/Components/POActorCollectorSphere.h"
#include "ProjectOdimh/POGameplayTags.cpp"
#include "ProjectOdimh/Components/POContextNeedsComponent.h"


USTSceneManager::USTSceneManager()
{
	
}

void USTSceneManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}

void USTSceneManager::GetSceneInfoRows(const UDataTable* Table, SceneRows& RowsOut)
{
	Table->GetAllRows("", RowsOut);
	
}

