// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_POActionMoveTo.generated.h"

/**
 * Move To task node using pawn's action component.
 * Moves the AI pawn toward the specified Actor or Location blackboard entry using the navigation system.
 */
UCLASS()
class PROJECTODIMH_API UBTTask_POActionMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Action")
	FName ActionName;
	
	virtual UAITask_MoveTo* PrepareMoveTask(UBehaviorTreeComponent& OwnerComp, UAITask_MoveTo* ExistingTask, FAIMoveRequest& MoveRequest) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
