// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "POWorldPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTODIMH_API APOWorldPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APOWorldPlayerController();
	
	void ToggleStageControl();

	UFUNCTION()
	void HandleActorPicked(AActor* InPickedActor);

	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AActor* PickedActor;
};
