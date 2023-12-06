// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"
#include "CombatGridComponent.generated.h"

class AMatch3Grid;

UCLASS(ClassGroup=(Match3), meta=(BlueprintSpawnableComponent))
class MATCH3COMBATRUNTIME_API UCombatGridComponent : public UChildActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatGridComponent();

	AMatch3Grid* GetCombatGrid() const;
	
protected:
	virtual void BeginPlay() override;
};
