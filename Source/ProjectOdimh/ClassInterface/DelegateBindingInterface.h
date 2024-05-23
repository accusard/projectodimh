// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectOdimh/Entities/Game/POStage.h"
#include "ProjectOdimh/Components/POActorCollectorSphere.h"
#include "UObject/Interface.h"
#include "DelegateBindingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDelegateBindingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTODIMH_API IDelegateBindingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AddOnActorsCollisionHandler(FOnActorsCollectedAtPointSignature& OnCollision) {}

	virtual void AddOnRequestStagingContextHandler(FOnStagingSignature& StageDelegate) {}

	virtual void RemoveStagingContextHandler(FOnStagingSignature& StageDelegate) {}
};
