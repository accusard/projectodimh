// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/AI/BTTask_POActionMoveTo.h"

#include "AIController.h"
#include "ProjectOdimh/Components/POActionComponent.h"

UAITask_MoveTo* UBTTask_POActionMoveTo::PrepareMoveTask(UBehaviorTreeComponent& OwnerComp, UAITask_MoveTo* ExistingTask,
                                                        FAIMoveRequest& MoveRequest)
{
	UAITask_MoveTo* Task = Super::PrepareMoveTask(OwnerComp, ExistingTask, MoveRequest);
	
	if(Task)
	{
		AAIController* AI = Cast<AAIController>(OwnerComp.GetAIOwner());
		
		if(ensure(AI))
		{
			APawn* MyPawn = AI->GetPawn();
			
			UPOActionComponent* ActionComp = MyPawn->FindComponentByClass<UPOActionComponent>();
			
			if(ensure(ActionComp))
			{
				ActionComp->StartActionByName(MyPawn, ActionName);
			}
		}
	}
	return Task;
}

EBTNodeResult::Type UBTTask_POActionMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTask_POActionMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                            EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if(const AAIController* AI = Cast<AAIController>(OwnerComp.GetAIOwner()); ensure(AI))
	{
		if(APawn* MyPawn = AI->GetPawn())
		{
			if(UPOActionComponent* ActionComp = MyPawn->FindComponentByClass<UPOActionComponent>(); ensure(ActionComp))
			{
				ActionComp->StopActionByName(MyPawn, ActionName);
			}
		}
	}
}
