// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FGAI_2/Grid/FGGridActor.h"
#include "GameFramework/Actor.h"
#include "Pathfinding.generated.h"

class AFGGridActor;

UCLASS()
class FGAI_2_API APathfinding : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APathfinding();

	UPROPERTY(EditAnywhere)
	AFGGridActor* Grid;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float Heuristic(int32 CurrentIndex, int32 GoalIndex);

public:
	UFUNCTION(BlueprintCallable)
	virtual TArray<int32> FindPath(int32 StartIndex, int32 GoalIndex);
};
