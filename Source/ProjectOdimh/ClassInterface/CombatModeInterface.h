// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatModeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCombatModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * + CombatManager uses this as communication bridge between combat mode and combat stage
 */
class PROJECTODIMH_API ICombatModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual TSubclassOf<AActor> GetMode() = 0;
};
