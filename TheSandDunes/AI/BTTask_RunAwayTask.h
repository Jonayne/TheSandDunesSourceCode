// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RunAwayTask.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API UBTTask_RunAwayTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RunAwayTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
	
};
