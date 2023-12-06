// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "CombatGridComponent.h"
#include "Match3Grid.h"


// Sets default values for this component's properties
UCombatGridComponent::UCombatGridComponent()
{
	
}

AMatch3Grid* UCombatGridComponent::GetCombatGrid() const
{
	AMatch3Grid* Grid = Cast<AMatch3Grid>(GetChildActor());
	
	if(Grid == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("AMatch3GridUCombatGridComponent child actor class is invalid or is not set."));
	}
	
	return Grid;
}

void UCombatGridComponent::BeginPlay()
{
	Super::BeginPlay();
	
}



