// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "ProjectOdimh/AI/BTTask_POActorAction.h"
#include "AIController.h"
#include "ProjectOdimh/Components/POActionComponent.h"

EBTNodeResult::Type UBTTask_POActorAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = Cast<AAIController>(OwnerComp.GetAIOwner());
	if(ensure(AI))
	{
		APawn* MyPawn = AI->GetPawn();
		if(MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		UPOActionComponent* ActionComp = MyPawn->FindComponentByClass<UPOActionComponent>();

		if(ensure(ActionComp))
		{
			switch(ActionState)
			{
				case EPOActionState::Start:
				{
					if(ActionComp->StartActionByName(MyPawn, ActionName))
					{
						return EBTNodeResult::Succeeded;
					}
					break;
				}
				case EPOActionState::Stop:
				{
					if(ActionComp->StopActionByName(MyPawn, ActionName))
					{
						return EBTNodeResult::Succeeded;
					}
					break;
				}
				default:
				{
					break;
				}
			}
			
		}
	}
	return EBTNodeResult::Failed;
}

FString UBTTask_POActorAction::GetStaticDescription() const
{
	FString State = FString::Printf(TEXT(" %s"), *UEnum::GetValueAsString(ActionState));
	
	const int FoundIndex = State.Find("::");
	State = State.RightChop(FoundIndex+2);
	
	return "Action " + State;
}
