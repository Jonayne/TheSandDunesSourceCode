// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "BTTask_Teleport.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy.h"
#include "EnemyController.h"

UBTTask_Teleport::UBTTask_Teleport() 
{
    NodeName = TEXT("Teleport Node");
}

EBTNodeResult::Type UBTTask_Teleport::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (!OwnerComp.GetAIOwner())
    {
        return EBTNodeResult::Failed;
    }

    AEnemyController* EnemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
    
    if (EnemyController)
    {
        AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
        ASpirit* SpiritSeen = EnemyController->GetSeenPlayer();
        if(SpiritSeen)
        {
            if(Enemy && !Enemy->bStunned)
            {
                if(Enemy->bMoveAroundMyself)
                {
                    Enemy->SetRandomPointOnArea(Enemy->GetActorLocation());
                }
                else
                {
                    Enemy->SetRandomPointOnArea(SpiritSeen->GetActorLocation());
                }
                
                float TimeToWait = Enemy->TeleportAnimation();
                OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("TimeToWaitKey"), TimeToWait);

                return EBTNodeResult::Succeeded;
            }
        }
    }   
    
    return EBTNodeResult::Failed;

}
