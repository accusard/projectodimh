

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "POActionComponent.generated.h"

class UPOAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTODIMH_API UPOActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPOActionComponent();
    
    UPROPERTY(EditAnywhere, Category = "Tags")
    FGameplayTagContainer ActiveGameplayTags;
    
    UFUNCTION(BlueprintCallable, Category = "Action")
    void AddAction(AActor* Instigator, TSubclassOf<UPOAction> ActionClass);
    
    UFUNCTION(BlueprintCallable, Category = "Action")
    void RemoveAction(UPOAction* ActionToRemove);
    
    UFUNCTION(BlueprintCallable, Category = "Action")
    UPOAction* GetAction(TSubclassOf<UPOAction> ActionClass);
    
    UFUNCTION(BlueprintCallable, Category = "Action")
    bool StartActionByName(AActor* Instigator, FName ActionName);
    
    UFUNCTION(BlueprintCallable, Category = "Action")
    bool StopActionByName(AActor* Instigator, FName ActionName);
    
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Action")
    TArray<TSubclassOf<UPOAction>> DefaultActions;
    
    UPROPERTY(BlueprintReadOnly, Category = "Action")
    TArray<UPOAction*> Actions;
    
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
