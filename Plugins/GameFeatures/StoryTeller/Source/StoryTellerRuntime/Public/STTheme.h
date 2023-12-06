// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "STTheme.generated.h"

/**
 * 
 */
UCLASS()
class STORYTELLERRUNTIME_API USTTheme : public UObject
{
	GENERATED_BODY()
	
public:
	FORCEINLINE TArray<AActor*> GetCombatants() { return Combatants; }

	FORCEINLINE AActor* GetInstigator() const { return Instigator; }

protected:
	UPROPERTY(EditDefaultsOnly, Category="Staging")
	AActor* Instigator;
	
	UPROPERTY(EditDefaultsOnly, Category="Staging")
	TArray<AActor*> Combatants;
};
