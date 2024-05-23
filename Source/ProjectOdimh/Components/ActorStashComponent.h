// Copyright 2023 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorStashComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTODIMH_API UActorStashComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActorStashComponent();

	void GetActiveCharacterTags(FGameplayTagContainer& Tags) const;
	
	bool IsCharacterAtMapPoint(AActor* Character) const;

	bool HasPlayerControlledCharacter() const;

	const TArray<AActor*>& GetActiveCharacters() const;

	UFUNCTION()
	void MakeInactive(const APOStage* Stage, const TArray<AActor*>& Actors);
	
	static void StashActor(AActor* TargetActor, const AActor* StashOn, const float ZAxis, const bool bDisableCollision, const bool bSetHidden);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Map)
	TArray<AActor*> ActiveCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Map)
	TArray<AActor*> InactiveCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Map)
	TArray<AActor*> Treasure;

	UPROPERTY(EditAnywhere, Category="Stashing")
	float StashingZAxisOffset = 200.f;

	UPROPERTY(EditAnywhere, Category="Stashing")
	uint8 bStashActorsOnCollision : 1;
	
	UPROPERTY(EditAnywhere, Category="Stashing")
	uint8 bDisableCollisionOnStash : 1;

	UPROPERTY(EditAnywhere, Category="Stashing")
	uint8 bHideActorsOnStash : 1;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
