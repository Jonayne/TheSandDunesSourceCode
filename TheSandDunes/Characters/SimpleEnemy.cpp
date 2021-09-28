// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SimpleEnemy.h"
#include "TimerManager.h"


ASimpleEnemy::ASimpleEnemy() 
{
    WaitBeforeDestroy = 5.f;

    TimeBetweenActions = 2.f;
    TimeBeforeStartingMin = 3.f;
    TimeBeforeStartingMax = 4.f;

    Tags.Add(FName("SimpleEnemy"));

    bCanBeTeleportedByOthersEnemies = true;
}

void ASimpleEnemy::MakeAnimation() 
{
    AttackAnimation();

    GetWorldTimerManager().SetTimer(TimerToMakeAnim, this, &ASimpleEnemy::MakeAnimation, TimeBetweenActions, false);
}

void ASimpleEnemy::KillThisSpirit() 
{
    GetWorldTimerManager().ClearTimer(TimerToMakeAnim);

    Super::KillThisSpirit();
}

void ASimpleEnemy::OnHaveSeenAnEnemy() 
{
    Super::OnHaveSeenAnEnemy();

    if(!bExecutedOnce && bCanStartFight)
    {
        bExecutedOnce = true;
        GetWorldTimerManager().SetTimer(TimerBeforeAction, this, &ASimpleEnemy::StartActions, FMath::FRandRange(TimeBeforeStartingMin, TimeBeforeStartingMax), false);
    }
}

void ASimpleEnemy::StartActions() 
{
    AttackAnimation();
    GetWorldTimerManager().SetTimer(TimerToMakeAnim, this, &ASimpleEnemy::MakeAnimation, TimeBetweenActions, false);
}
