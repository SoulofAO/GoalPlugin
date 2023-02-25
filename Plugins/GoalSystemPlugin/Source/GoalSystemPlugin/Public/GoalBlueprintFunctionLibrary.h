// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GoalBlueprintFunctionLibrary.generated.h"

/**
 * 
 */

class UGoalObject;
class UInfoActorComponent;

UCLASS()
class GOALSYSTEMPLUGIN_API UGoalBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static UPlunObject* GetOptimalPlunInstance(UInfoActorComponent* Component, TSubclassOf<UGoalObject> GoalClass);

};
