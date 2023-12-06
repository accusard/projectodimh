// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "ProjectOdimh/ClassInterface/DataSaveInterface.h"
#include "POCharacter.generated.h"

class UPOContextNeedsComponent;
class UPOActionComponent;

UCLASS()
class PROJECTODIMH_API APOCharacter : public ACharacter, public IGameplayTagAssetInterface, public IDataSaveInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APOCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category=Component)
	UPOActionComponent* GameplayAction;

	UPROPERTY(EditAnywhere, Category=Component)
	UPOContextNeedsComponent* GameplayObjectives;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
};
