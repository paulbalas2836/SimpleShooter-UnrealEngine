// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UMyBTTask_ClearBlackboardValue::UMyBTTask_ClearBlackboardValue()
{
	NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UMyBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

	return EBTNodeResult::Succeeded;
}