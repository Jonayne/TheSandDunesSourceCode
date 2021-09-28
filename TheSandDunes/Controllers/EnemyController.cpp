// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "EnemyController.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Diana.h"
#include "SpiritHelper.h"
#include "Kismet/GameplayStatics.h"


AEnemyController::AEnemyController() 
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
    if (SightConfig && EnemyPerceptionComponent)
	{
		SetPerceptionComponent(*EnemyPerceptionComponent);
		SightConfig->SightRadius = 5000.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 5000.f;
		SightConfig->PeripheralVisionAngleDegrees = 180.f;
		SightConfig->SetMaxAge(0.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 10000.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
        SightConfig->PointOfViewBackwardOffset = 1000.f;
        SightConfig->NearClippingRadius = 1000.f;

		// Agregar sight component a Perception component.
		EnemyPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
		EnemyPerceptionComponent->ConfigureSense(*SightConfig);
	}

    DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
    if (DamageConfig)
	{
		DamageConfig->SetMaxAge(0.f);
		EnemyPerceptionComponent->ConfigureSense(*DamageConfig);
	}

    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
    if (HearingConfig)
	{
		HearingConfig->SetMaxAge(0.f);
        HearingConfig->HearingRange = 5000.f;
        HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
        HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;

		EnemyPerceptionComponent->ConfigureSense(*HearingConfig);
	}

    EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AEnemyController::OnActorUpdated);

    // Team 1. Para diferenciar entre nuestro personaje y el enemigo.
    SetGenericTeamId(FGenericTeamId(1));
}

void AEnemyController::OnPossess(APawn* InPawn) 
{
    Super::OnPossess(InPawn);

    ControlledEnemy = Cast<AEnemy>(InPawn);
    if(ControlledEnemy && ControlledEnemy->bEnemyByEvent)
    {
        EnemyPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
    }
    if(ControlledEnemy && !ControlledEnemy->bShouldHear)
    {
        EnemyPerceptionComponent->SetSenseEnabled(UAISense_Hearing::StaticClass(), false);
    }
}

void AEnemyController::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);
}

bool AEnemyController::IsDead() const
{
    AEnemy* ControlledCharacter = GetPawn<AEnemy>();
    if (ControlledCharacter)
    {
        return ControlledCharacter->IsDead();
    }

    return true;
}

void AEnemyController::OnActorUpdated(AActor* Actor, FAIStimulus Stimulus) 
{
    if(ControlledEnemy && Actor != SeenPlayer)
    {
        // Podemos atacar a nuestros aliados.
        if(bAttackEveryone)
        {
            ASpirit* SensedSpirit = Cast<ASpirit>(Actor);
            if(SensedSpirit)
            {
                if(Stimulus.WasSuccessfullySensed())
                {
                    if(ControlledEnemy->bEnemyByEvent && SensedSpirit->IsA(ADiana::StaticClass()))
                    {
                        if(ControlledEnemy->bCanStartFight)
                        {
                            ControlledEnemy->StartBossFight();
                        }
                    }
                    else
                    {
                        SeenPlayer = SensedSpirit;
                        ControlledEnemy->bSeeingEnemy = true;
                        ControlledEnemy->SetTargetToHit(SensedSpirit);
                        SetFocus(SensedSpirit);

                    }
                    ControlledEnemy->OnSeenEnemy.Broadcast();
                } 
            }
        }
        else if(Actor->IsA(ADiana::StaticClass()) || Actor->IsA(ASpiritHelper::StaticClass()))
        {
            ADiana* Diana = Cast<ADiana>(UGameplayStatics::GetPlayerPawn(this, 0));
            if(Diana)
            {
                if(Stimulus.WasSuccessfullySensed())
                {
                    if(ControlledEnemy->bEnemyByEvent)
                    {
                        if(ControlledEnemy->bCanStartFight)
                        {
                            ControlledEnemy->StartBossFight();
                        }
                    }
                    else
                    {
                        SeenPlayer = Diana;
                        ControlledEnemy->bSeeingEnemy = true;
                        ControlledEnemy->SetTargetToHit(Diana);
                        SetFocus(Diana);
                    }
                    ControlledEnemy->OnSeenEnemy.Broadcast();
                }
                EnemyPerceptionComponent->OnTargetPerceptionUpdated.Clear();
            }
            
        }
    }
}

void AEnemyController::StartEnemyBehaviorTree() 
{
    if (AIBehaviorTree)
    {
        RunBehaviorTree(AIBehaviorTree);
    }
}

ASpirit* AEnemyController::GetSeenPlayer() 
{
    if(SeenPlayer)
    {
        if(!SeenPlayer->IsDead())
        {
            return SeenPlayer;
        }
        else
        {
            SeenPlayer = nullptr;
            return SeenPlayer;
        }   
    }
    
    return nullptr;
}

bool AEnemyController::IsSeeingPlayer()
{
    if(ControlledEnemy)
    {
        return ControlledEnemy->bSeeingEnemy;
    }

    return false;
}

ASpirit* AEnemyController::GetSpiritControlled() 
{
    return GetPawn<ASpirit>();
}
