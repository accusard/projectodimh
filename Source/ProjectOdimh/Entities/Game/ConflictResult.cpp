// Copyright 2023 Vanny Sou. All Rights Reserved.


#include "ConflictResult.h"

UConflictResult::UConflictResult()
{
}

void UConflictResult::Init(AActor* InWinner, AActor* InLoser, const FMatchOutcome& Outcome, APOMapPoint* InMapPoint)
{
	Winner = InWinner;
	Loser = InLoser;
	MatchOutcome = Outcome;
	MapPoint = InMapPoint;
}

AActor* UConflictResult::GetWinner() const
{
	return Winner;
}

AActor* UConflictResult::GetLoser() const
{
	return Loser;
}

const FMatchOutcome& UConflictResult::GetOutcome() const
{
	return MatchOutcome;
}

APOMapPoint* UConflictResult::GetMapPoint() const
{
	return MapPoint;
}