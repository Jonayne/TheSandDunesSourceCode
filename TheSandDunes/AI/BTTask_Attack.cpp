// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (!OwnerComp.GetAIOwner())
    {
        return EBTNodeResult::Failed;
    }

    AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
    if (!Enemy)
    {
        return EBTNodeResult::Failed;
    }
    if(!Enemy->bStunned)
    {
        float TimeToWait = Enemy->AttackAnimation();

        OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("TimeToWaitKey"), TimeToWait);

        return EBTNodeResult::Succeeded;
    }
    
    return EBTNodeResult::Failed;
}

UBTTask_Attack::UBTTask_Attack() 
{
    NodeName = TEXT("Attack Node");
}
