// Copyright 2023 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ConflictResult.generated.h"

/**
 * 
 */

USTRUCT()
struct FMatchOutcome
{
	GENERATED_BODY()
	int WinnerScore;
	int NumberOfTurnsTaken;
};

UCLASS()
class PROJECTODIMH_API UConflictResult : public UObject
{

	GENERATED_BODY()

	friend class UPOConflictSystem;
	
	UConflictResult();

public:
	void Init(AActor* InWinner, AActor* InLoser, const FMatchOutcome& Outcome, class APOMapPoint* InMapPoint);
	
	AActor* GetWinner() const;
	
	AActor* GetLoser() const;
	
	const FMatchOutcome& GetOutcome() const;
	
	APOMapPoint* GetMapPoint() const;

private:
	UPROPERTY()
	AActor* Winner;

	UPROPERTY()
	AActor* Loser;

	UPROPERTY()
	FMatchOutcome MatchOutcome;

	UPROPERTY()
	APOMapPoint* MapPoint;
};
