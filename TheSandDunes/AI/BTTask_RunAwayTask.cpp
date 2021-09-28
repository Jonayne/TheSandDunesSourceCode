// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "BTTask_RunAwayTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy.h"
#include "EnemyController.h"
#include "Kismet/KismetMathLibrary.h"


EBTNodeResult::Type UBTTask_RunAwayTask::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
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
        AEnemyController* EnemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
        
        if (EnemyController)
        {
            ASpirit* SpiritSeen = EnemyController->GetSeenPlayer();
            if(SpiritSeen && !SpiritSeen->IsDead())
            {
                FVector EnemyLoc = Enemy->GetActorLocation();
                FVector SpiritSeenLocation = SpiritSeen->GetActorLocation();

                if(FVector::Dist(EnemyLoc, SpiritSeenLocation) <= 3000.f)
                {
                    float RandomRotation = FMath::RandRange(-90.f, 90.f);
                    FRotator EnemyRotation = Enemy->GetActorRotation();

                    EnemyRotation.Yaw = EnemyRotation.Yaw + RandomRotation;

                    FVector RotForwardVector = UKismetMathLibrary::GetForwardVector(EnemyRotation);
                    RotForwardVector = RotForwardVector * -1.f;

                    FVector OriginLocation = EnemyLoc + (RotForwardVector * 8000.f);
                    Enemy->SetRandomPointOnArea(OriginLocation);
                }
                else
                {
                    Enemy->SetRandomPointOnArea(SpiritSeenLocation);
                }   

				OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("AwayFromPlayerPoint"), Enemy->NextLocation);
            }
            else
            {
                OwnerComp.GetBlackboardComponent()->ClearValue(FName("AwayFromPlayerPoint"));
            }

            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}

UBTTask_RunAwayTask::UBTTask_RunAwayTask() 
{
    NodeName = TEXT("Run Away Node");
}
