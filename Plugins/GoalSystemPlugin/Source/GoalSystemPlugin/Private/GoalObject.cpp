// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalObject.h"

void UGoalObject::StartTaskFunction_Implementation(UInfoActorComponent* Component)
{
	OnGoalComplete.Broadcast(true);
}


void UGoalObject::EndTaskFunction_Implementation(UInfoActorComponent* Component)
{
	OnGoalComplete.Broadcast(true);
}

UWorld* UGoalObject::GetWorld() const
{
	if (GIsEditor && !GIsPlayInEditorWorld) return nullptr;
	else if (GetOuter()) return GetOuter()->GetWorld();
	else return nullptr;
}

UPlunObject* UGoalObject::GetOptimalPlunInstance(UInfoActorComponent* Component)
{
	UPlunObject* PlunObject = NewObject<UPlunObject>(Component);
	PlunObject->GoalObjects = GetOptimalGoals(Component);
	return PlunObject;
}

TArray<UGoalObject*> UGoalObject::GetOptimalGoals(UInfoActorComponent* Component)
{
	TArray<UGoalObject*> GoalObjects;
	if (OperationType == EOperationType::And)
	{
		GoalObjects.Add(this);
		for (FChildGoalStruct ChildGoalStruct : ChildGoalStructs)
		{
			UGoalObject* LocalGoalObject = NewObject<UGoalObject>(Component, ChildGoalStruct.TaskObject);
			GoalObjects.Append(LocalGoalObject->GetOptimalGoals(Component));
		}
		GoalObjects.Add(this);
	}
	else
	{
		float Cost = 100000;
		FChildGoalStruct OptimalChildGoalStruct;

		for (FChildGoalStruct ChildGoalStruct : ChildGoalStructs)
		{
			float LocalCost = 0;

			for (TSubclassOf <UCalculateCostObject> CalculateObject : ChildGoalStruct.CalculateCostObject)
			{
				LocalCost = LocalCost + CalculateObject.GetDefaultObject()->CalculateCostTask(Component);
			}

			if (LocalCost < Cost)
			{
				OptimalChildGoalStruct = ChildGoalStruct;
				Cost = LocalCost;
			}
		}
		
		GoalObjects.Add(this);
		if (OptimalChildGoalStruct.TaskObject)
		{
			UGoalObject* LocalGoalObject = NewObject<UGoalObject>(Component, OptimalChildGoalStruct.TaskObject);
			GoalObjects.Append(LocalGoalObject->GetOptimalGoals(Component));
		}
		GoalObjects.Add(this);
	}

	return GoalObjects;
}

void UPlunObject::PlayGoal(UInfoActorComponent* InfoActorComponent)
{
	PlunInfoActorComponent = InfoActorComponent;
	IndexComplete = 0;
	GoalObjects[IndexComplete]->OnGoalComplete.AddDynamic(this, &UPlunObject::GoalComplete);
	GoalObjects[IndexComplete]->StartTaskFunction(InfoActorComponent);
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
			if (GoalObjects[IndexComplete]->StartEvent)
			{
				GoalObjects[IndexComplete]->StartEvent = false;
				GoalObjects[IndexComplete]->StartTaskFunction(PlunInfoActorComponent);
			}
			else
			{
				GoalObjects[IndexComplete]->EndTaskFunction(PlunInfoActorComponent);
			}
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



