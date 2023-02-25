// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalObject.h"

void UGoalObject::BaseTaskFunction_Implementation(UInfoActorComponent* Component)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Black, "NoImplementEventInGoal");
}


UPlunObject* UGoalObject::GetOptimalPlunInstance(UInfoActorComponent* Component)
{
	UPlunObject* PlunObject = NewObject<UPlunObject>(Component);
	GetOptimalGoals(Component);
	return PlunObject;
}

TArray<UGoalObject*> UGoalObject::GetOptimalGoals(UInfoActorComponent* Component)
{
	TArray<UGoalObject*> GoalObjects;
	if (OperationType == EOperationType::And)
	{
		for (FChildGoalStruct ChildGoalStruct : ChildGoalStructs)
		{
			UGoalObject* LocalGoalObject = NewObject<UGoalObject>(Component, ChildGoalStruct.TaskObject);
			GoalObjects.Append(LocalGoalObject->GetOptimalGoals(Component));
			GoalObjects.Add(this);
			
		}
	}
	else
	{
		float Cost = 100000;
		FChildGoalStruct OptimalChildGoalStruct;

		for (FChildGoalStruct ChildGoalStruct : ChildGoalStructs)
		{
			float LocalCost = ChildGoalStruct.CalculateCostObject.GetDefaultObject()->CalculateCostTask(Component);

			if (LocalCost < Cost)
			{
				OptimalChildGoalStruct = ChildGoalStruct;
				Cost = LocalCost;
			}
		}
		UGoalObject* LocalGoalObject = NewObject<UGoalObject>(Component, OptimalChildGoalStruct.TaskObject);
		GoalObjects.Append(LocalGoalObject->GetOptimalGoals(Component));
		GoalObjects.Add(this);
	}

	return GoalObjects;
}

void UPlunObject::PlayGoal(UInfoActorComponent* InfoActorComponent)
{
	PlunInfoActorComponent = InfoActorComponent;
	IndexComplete = 0;
	GoalObjects[IndexComplete]->OnGoalComplete.AddDynamic(this, &UPlunObject::GoalComplete);
	GoalObjects[IndexComplete]->BaseTaskFunction(InfoActorComponent);
}

void UPlunObject::GoalComplete(bool Result)
{
	if (Result)
	{
		GoalObjects[IndexComplete]->OnGoalComplete.RemoveDynamic(this, &UPlunObject::GoalComplete);
		IndexComplete = IndexComplete + 1;
		if (GoalObjects.IsValidIndex(IndexComplete))
		{
			GoalObjects[IndexComplete]->OnGoalComplete.AddDynamic(this, &UPlunObject::GoalComplete);
			GoalObjects[IndexComplete]->BaseTaskFunction(PlunInfoActorComponent);
		}
		else
		{
			PlunResultDelegate.Broadcast(true);
		}
	}
	else
	{
		PlunResultDelegate.Broadcast(false);
	}
}



