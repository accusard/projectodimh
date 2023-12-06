// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/AI/POWorldAIController.h"

void APOWorldAIController::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = WorldAIControllerTags;
}
