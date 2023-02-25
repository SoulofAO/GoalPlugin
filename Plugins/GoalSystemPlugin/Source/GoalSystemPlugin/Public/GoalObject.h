// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InfoActorComponent.h"
#include "GoalObject.generated.h"

/**
 * 
 */

class UGoalObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGoalCompleteDelegate, bool, Result);

UCLASS(Blueprintable)
class GOALSYSTEMPLUGIN_API UCalculateCostObject : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	float CalculateCostTask(UInfoActorComponent* InfoComponent);
};

UCLASS(Blueprintable)
class GOALSYSTEMPLUGIN_API UCheckActualTaskObject : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void CheckActualTask(UInfoActorComponent* InfoComponent);
};

USTRUCT(Blueprintable)
struct FChildGoalStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGoalObject> TaskObject;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCalculateCostObject> CalculateCostObject;

};

UENUM(Blueprintable)
enum class EOperationType : uint8
{
	And,
	Or
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlunResultDelegate, bool, Result);

UCLASS(Blueprintable)
class GOALSYSTEMPLUGIN_API UPlunObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	TArray<UGoalObject*> GoalObjects;

	UPROPERTY(BlueprintReadOnly)
	int IndexComplete = 0;

	UPROPERTY(BlueprintReadOnly)
	UInfoActorComponent* PlunInfoActorComponent;

	UPROPERTY(BlueprintReadWrite)
	FPlunResultDelegate PlunResultDelegate;

	UFUNCTION(BlueprintCallable)
	void PlayGoal(UInfoActorComponent* InfoActorComponent);

	void GoalComplete(bool Result);
};

UCLASS(Blueprintable)
class GOALSYSTEMPLUGIN_API UGoalObject : public UObject
{
	GENERATED_BODY()
public:
//Core
	UPROPERTY(EditDefaultsOnly)
	TArray<FChildGoalStruct> ChildGoalStructs;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCheckActualTaskObject> CheckActualObject;

	UFUNCTION(BlueprintNativeEvent)
	void BaseTaskFunction(UInfoActorComponent* Component);

	UPROPERTY(EditDefaultsOnly)
	EOperationType OperationType;

	UPROPERTY(BlueprintAssignable)
	FGoalCompleteDelegate OnGoalComplete;

//Control

	UPlunObject* GetOptimalPlunInstance(UInfoActorComponent* Component);

	TArray<UGoalObject*> GetOptimalGoals(UInfoActorComponent* Component);
};

