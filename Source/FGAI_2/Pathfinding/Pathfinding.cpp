// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfinding.h"

#include "PriorityQueue.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APathfinding::APathfinding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APathfinding::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> AllGridActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGGridActor::StaticClass(), AllGridActors);

	if (AllGridActors.Num() > 0)
	{
		// just pick the first for now
		Grid = Cast<AFGGridActor>(AllGridActors[0]);
	}
}

float APathfinding::Heuristic(int32 CurrentIndex, int32 GoalIndex)
{
	return std::abs(CurrentIndex % Grid->Width - GoalIndex % Grid->Width) +
		std::abs(CurrentIndex / Grid->Width - GoalIndex / Grid->Width);
}

TArray<int32> APathfinding::FindPath(int32 StartIndex, int32 GoalIndex)
{
	TMap<int32, int32> ParentMap;
	TMap<int32, float> CostMap;
	TPriorityQueue<int32> Frontier;
	Frontier.Push(StartIndex, 0);

	ParentMap.Add(StartIndex, StartIndex);
	CostMap.Add(StartIndex, 0);

	while (!Frontier.IsEmpty())
	{
		int32 Current = Frontier.Pop();

		if (Current == GoalIndex)
			break;

		for (int32 Next : Grid->GetCardinalNeighbours(Current))
		{
			float NewCost = CostMap[Current] + 1;
			if (!CostMap.Contains(Next))
			{
				CostMap.Add(Next, NewCost);
				int32 Priority = NewCost + Heuristic(Next, GoalIndex);
				Frontier.Push(Next, Priority);
				ParentMap.Add(Next, Current);
			}
			else if (NewCost < CostMap[Next])
			{
				CostMap[Next] = NewCost;
				int32 Priority = NewCost + Heuristic(Next, GoalIndex);
				Frontier.Push(Next, Priority);
				ParentMap[Next] = Current;
			}
		}
	}
	
	TArray<int32> ReturnPath;

	// Return empty map if path was no found.
	if (!CostMap.Contains(GoalIndex))
		return ReturnPath;
	
	ReturnPath.Add(ParentMap[GoalIndex]);
	int32 Current;
	while ((Current = ReturnPath.Top()) != StartIndex)
	{
		ReturnPath.Add(ParentMap[Current]);
	}
	
	Algo::Reverse(ReturnPath);
	return ReturnPath;
}

