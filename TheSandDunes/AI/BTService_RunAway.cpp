// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "BTService_RunAway.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "EnemyController.h"
#include "Enemy.h"

UBTService_RunAway::UBTService_RunAway() 
{
    this->NodeName = TEXT("Run Way Location");

    bNotifyBecomeRelevant = true;
}

void UBTService_RunAway::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) 
{
    
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    
    if (OwnerComp.GetAIOwner())
    {
        AEnemyController* EnemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
        
        if (EnemyController)
        {
            ASpirit* SpiritSeen = EnemyController->GetSeenPlayer();
            AEnemy* SpiritControlled = Cast<AEnemy>(EnemyController->GetSpiritControlled());
            // Hacemos que la IA huya de nosotros.
            if(SpiritSeen && !SpiritSeen->IsDead())
            {
                FVector SpiritSeenLocation = SpiritSeen->GetActorLocation();
                FVector SpiritControlledLocation = SpiritControlled->GetActorLocation();

                FVector SubResult = SpiritControlledLocation - SpiritSeenLocation;
                SubResult.Normalize();

                FVector OriginLocation = SubResult * 3000.f;
                SpiritControlled->SetRandomPointOnArea(OriginLocation);

                OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), SpiritControlled->NextLocation);
            }
            else
            {
                OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
            }
        }
    }
}
