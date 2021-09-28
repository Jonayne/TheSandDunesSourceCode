// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdatePlayerIfSeen.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API UBTService_UpdatePlayerIfSeen : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_UpdatePlayerIfSeen();

protected:
	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
	
};
