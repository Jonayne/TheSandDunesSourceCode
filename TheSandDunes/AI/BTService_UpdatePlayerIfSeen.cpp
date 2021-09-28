// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "BTService_UpdatePlayerIfSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "EnemyController.h"
#include "Spirit.h"
#include "Enemy.h"

UBTService_UpdatePlayerIfSeen::UBTService_UpdatePlayerIfSeen() 
{
    this->NodeName = TEXT("Update Player If Seen");

    // por si cambian los observers... tambien esta bnotifyceaserelevant
    bNotifyBecomeRelevant = true;
}

void UBTService_UpdatePlayerIfSeen::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) 
{
    
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    
    if (OwnerComp.GetAIOwner())
    {
        AEnemyController* EnemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
        
        if (EnemyController)
        {
            // Enemigo que hemos (o no) visto.
            ASpirit* Player = Cast<ASpirit>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
            ASpirit* SpiritSeen = EnemyController->GetSeenPlayer();
            if(SpiritSeen && !SpiritSeen->IsDead())
            {
                if(!Player || Player != SpiritSeen)
                {
                    OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), SpiritSeen);
                    AEnemy* SpiritControlled = Cast<AEnemy>(EnemyController->GetSpiritControlled());
                    if(SpiritControlled)
                    {
                        SpiritControlled->SetTargetToHit(SpiritSeen);
                    }

                }
            }
            else
            {
                if(Player)
                {
                    AEnemy* SpiritControlled = Cast<AEnemy>(EnemyController->GetSpiritControlled());
                    EnemyController->SetFocus(nullptr);
                    if(SpiritControlled)
                    {
                        SpiritControlled->SetTargetToHit(nullptr);
                    }
                    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
                }
            }
        }
    }
}
