// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_POActorAction.generated.h"

UENUM(BlueprintType)
namespace EPOActionState
{
	enum Type
	{
		Start,
		Stop,
	};
}
/**
 * This task allow users to start or stop an action by name
 */
UCLASS()
class PROJECTODIMH_API UBTTask_POActorAction : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
	
	UPROPERTY(EditAnywhere, Category="Action")
	TEnumAsByte<EPOActionState::Type> ActionState;
	
	UPROPERTY(EditAnywhere, Category="Action")
	FName ActionName;
};
