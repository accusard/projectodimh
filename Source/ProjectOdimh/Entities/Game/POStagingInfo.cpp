// // Copyright 2022 Vanny Sou. All Rights Reserved.
//
//
// #include "POStagingInfo.h"
// #include "POCharacter.h"
// #include "POMapPoint.h"
// #include "ProjectOdimh/Components/POContextNeedsComponent.h"
// #include "ProjectOdimh/Subsystems/EventManager.h"
//
//
// // Sets default values
// APOStagingInfo::APOStagingInfo()
// {
// 	bAutoStageActorsOnSpawn = true;
// 	bMakeStagePlayerControlled = false;
// 	ContextRequirements = CreateDefaultSubobject<UPOContextNeedsComponent>("Context Requirements");
// }
//
// APOMapPoint* APOStagingInfo::GetMapPoint() const
// {
// 	return Cast<APOMapPoint>(AssociatedMapPoint);
// }
//
// void APOStagingInfo::AddSubsystemsStagingHandler(FStagingInfoSignature& Delegate) const
// {
// 	// GetGameInstance()->GetSubsystem<UEventManager>()->BindDelegateToSubsystems(Delegate);
// }
//
// void APOStagingInfo::RemoveSubsystemsStagingHandler(FStagingInfoSignature& Delegate) const
// {
// 	GetGameInstance()->GetSubsystem<UEventManager>()->UnbindDelegateFromSubsystems(Delegate);
// }
//
// void APOStagingInfo::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// 	ensure(AssociatedMapPoint);
// 	
// 	AddSubsystemsStagingHandler(OnRequestStageType);
// }
//
// void APOStagingInfo::EndPlay(const EEndPlayReason::Type EndPlayReason)
// {
// 	if(EndPlayReason != EEndPlayReason::Quit)
// 	{
// 		RemoveSubsystemsStagingHandler(OnRequestStageType);
// 	}
// 	
// 	Super::EndPlay(EndPlayReason);
// }
//
// void APOStagingInfo::Initialize(AActor* Point, const TArray<AActor*>& InActors)
// {
// 	if(APOMapPoint* MapPoint = Cast<APOMapPoint>(Point))
// 	{
// 		SetMapPoint(MapPoint);
// 		SetActorTransform(MapPoint->GetTransform());
// 	}
//
// 	for(AActor* Actor : InActors)
// 	{
// 		if(const APOCharacter* Character = Cast<APOCharacter>(Actor))
// 		{
// 			bMakeStagePlayerControlled = Character->IsPlayerControlled();
// 			break;
// 		}
// 	}
// }
//
//
