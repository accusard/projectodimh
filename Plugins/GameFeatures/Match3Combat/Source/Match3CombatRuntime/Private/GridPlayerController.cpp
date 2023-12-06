// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#include "GridPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
#include "ProjectOdimh/Components/ActorPickHandlerComponent.h"
#include "ProjectOdimh/Components/ActionTurnBasedComponent.h"
#include "ProjectOdimh/Events/PlayerInputEvent.h"
#include "ProjectOdimh/Events/TurnEvent.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/Subsystems/EventManager.h"

AGridPlayerController::AGridPlayerController()
{
    TileHandlerComponent = CreateDefaultSubobject<UActorPickHandlerComponent>("Tile Handler");
    ActionComp = CreateDefaultSubobject<UActionTurnBasedComponent>("Turn-Based Action Handler");
    
    static ConstructorHelpers::FObjectFinder<USoundCue> DefaultGrabSoundCue(TEXT("SoundCue'/Game/Interface/The_Future_Is_Now/cues/1_Neutral/UI_Neutral_173_Cue.UI_Neutral_173_Cue'"));
    
    static ConstructorHelpers::FObjectFinder<USoundCue> DefaultReleaseSoundCue(TEXT("SoundCue'/Game/Interface/The_Future_Is_Now/cues/1_Neutral/UI_Neutral_205_Cue.UI_Neutral_205_Cue'"));
    
    if(DefaultGrabSoundCue.Object)
        PickCue = DefaultGrabSoundCue.Object;
    
    if(DefaultReleaseSoundCue.Object)
        ReleaseCue = DefaultReleaseSoundCue.Object;
    bAutoManageActiveCameraTarget = false;
}

void AGridPlayerController::BeginPlay()
{
    Super::BeginPlay();
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    Instance->GetSubsystem<UEventManager>()->Delegates.OnActorPicked.AddDynamic(this, &AGridPlayerController::HandlePick);
}

UActorPickHandlerComponent* AGridPlayerController::GetPickHandler()
{
    return TileHandlerComponent;
}

void AGridPlayerController::HandlePick(AActor* PickedTile)
{
    GetPickHandler()->AddHandledActor(PickedTile);
}

void AGridPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    InputComponent->BindTouch(IE_Pressed, this, &AGridPlayerController::BeginTouch);
    InputComponent->BindTouch(IE_Released, this, &AGridPlayerController::EndTouch);
    
}

void AGridPlayerController::BeginTouch(ETouchIndex::Type FingerIndex, FVector Location)
{
    // if(Match3Grid)
    // {
        if(!UGameplayStatics::IsGamePaused(this) && TileHandlerComponent)
        {
            FHitResult Hit = FHitResult();
            const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes { UEngineTypes::ConvertToObjectType(ECC_WorldDynamic) };
            if(GetHitResultUnderFingerForObjects(FingerIndex, ObjectTypes, false, Hit))
            {
                NewInput("PlayerInputEvent", FingerIndex, Hit);
                Execute_OnPickActor(this, Hit.Location);
            }
        }
    // }
}

void AGridPlayerController::EndTouch(ETouchIndex::Type FingerIndex, FVector Location)
{
    if(IsValid(TouchInputEvent))
    {
        Execute_OnReleaseActor(this, TouchInputEvent->TouchResult);
        TouchInputEvent->End();
        TouchInputEvent->MarkAsGarbage();
    }
}

AActor* AGridPlayerController::GetLastTouched()
{
    AActor* LastTouched = Cast<IPickHandlerInterface>(this)->GetLastGrab(TileHandlerComponent);
    return LastTouched;
}

void AGridPlayerController::NewInput(const FName& Name, ETouchIndex::Type FingerIndex, FHitResult& Hit)
{
    if(IsValid(TouchInputEvent))
    {
        TouchInputEvent->End();
        TouchInputEvent->MarkAsGarbage();
    }
    
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    
    TouchInputEvent = Instance->GetSubsystem<UEventManager>()->NewEvent<UPlayerInputEvent>(this, Name, false);
    TouchInputEvent->FingerIndex = FingerIndex;
    TouchInputEvent->TouchResult = Hit.GetActor();
    TouchInputEvent->Start();
    
}

void AGridPlayerController::ForceRelease()
{
    Execute_OnReleaseActor(this, TouchInputEvent->TouchResult);
}
