// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalBlueprintFunctionLibrary.h"
#include "GoalObject.h"
#include "InfoActorComponent.h"

UPlunObject* UGoalBlueprintFunctionLibrary::GetOptimalPlunInstance(UInfoActorComponent* Component, TSubclassOf<UGoalObject> GoalClass)
{
	UGoalObject* MainGoalObject = NewObject<UGoalObject>(Component,GoalClass);
	return MainGoalObject->GetOptimalPlunInstance(Component);
}
