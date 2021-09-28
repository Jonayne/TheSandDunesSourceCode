// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "BTTask_MoveEnemyToLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy.h"
#include "EnemyController.h"


EBTNodeResult::Type UBTTask_MoveEnemyToLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
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
        float TimeToWait = 0.f;

        if(Enemy->bMoveAroundMyself)
        {
            TimeToWait = Enemy->MoveEnemyToRandomLocation(Enemy);
        }
        else
        {
            TimeToWait = Enemy->MoveEnemyToRandomLocation(Enemy->GetTargetToHit());
        }
        
        OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("TimeToWaitKey"), TimeToWait);

        return EBTNodeResult::Succeeded;
    }
    
    return EBTNodeResult::Failed;
}

UBTTask_MoveEnemyToLocation::UBTTask_MoveEnemyToLocation() 
{
    NodeName = TEXT("MoveEnemyToLocation Node");
}
