

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "POAction.generated.h"

class UPOActionComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTODIMH_API UPOAction : public UObject
{
	GENERATED_BODY()
	
public:
    void Initialize(UPOActionComponent* NewComp);
    
    UFUNCTION(BlueprintNativeEvent, Category = "Action")
    void StartAction(AActor* Instigator);
    
    UFUNCTION(BlueprintNativeEvent, Category = "Action")
    void StopAction(AActor* Instigator);
    
    UFUNCTION(BlueprintNativeEvent, Category = "Action")
    bool CanStart(AActor* Instigator);
    
    UFUNCTION(BlueprintPure)
    bool IsRunning() const;
    
    UFUNCTION(BlueprintCallable, Category = "Action")
    UPOActionComponent* GetOwningComponent() const;
    
protected:
    UPROPERTY(BlueprintReadOnly)
    bool bIsRunning;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tags")
    FGameplayTagContainer GrantsTags;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tags")
    FGameplayTagContainer BlockedTags;
    
    UPROPERTY()
    UPOActionComponent* ActionComp;
    
public:
    UPROPERTY(EditDefaultsOnly, Category = "Action")
    FName Name;
};
