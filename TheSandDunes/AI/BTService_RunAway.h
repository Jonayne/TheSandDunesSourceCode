// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_RunAway.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API UBTService_RunAway : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_RunAway();

protected:
	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
	
};
