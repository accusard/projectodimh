// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "POStagingComponent.generated.h"

class APOMapPoint;
class APOStagingInfo;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTODIMH_API UPOStagingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FString ActorsLabel;
	
	uint32 Capacity = 2;
	
	// Sets default values for this component's properties
	UPOStagingComponent();
	
	void StageActors(const TArray<AActor*>& InActors);
	
	bool IsStageFull() const;

	void GetActorsInPlay(TArray<AActor*>& OutActors);
	
	// The Stage will be cleared of Characters, and the Characters in the associated
	// 'point' will be made inactive
	void EmptyStage();

private:
	bool StageActor(AActor* InActor);
	
	UPROPERTY()
	TMap<FName,AActor*> ActorsInPlay;
};
