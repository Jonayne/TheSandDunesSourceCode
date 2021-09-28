// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveEnemyToLocation.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API UBTTask_MoveEnemyToLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveEnemyToLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
	
};
