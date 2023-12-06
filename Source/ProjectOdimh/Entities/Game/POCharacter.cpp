// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/Entities/Game/POCharacter.h"

#include "Components/CapsuleComponent.h"
#include "ProjectOdimh/Components/POActionComponent.h"
#include "ProjectOdimh/Components/POContextNeedsComponent.h"

// Sets default values
APOCharacter::APOCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameplayAction = CreateDefaultSubobject<UPOActionComponent>("Gameplay Action Component");
	GameplayObjectives = CreateDefaultSubobject<UPOContextNeedsComponent>("Gameplay Objectives");
}

// Called when the game starts or when spawned
void APOCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APOCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APOCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

}

void APOCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(GameplayAction->ActiveGameplayTags);
	GameplayObjectives->GetRequiredContextsRemaining(TagContainer);
}



