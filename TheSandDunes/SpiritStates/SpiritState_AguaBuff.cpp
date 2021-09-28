// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpiritState_AguaBuff.h"
#include "TimerManager.h"
#include "Diana.h"


ASpiritState_AguaBuff::ASpiritState_AguaBuff() 
{
    StateName = TEXT("AguaBuff");
    StateDuration = 10.f;
    StateProbability = 1.f;
    StateCooldown = 15.f;
    bIsEffectPerTick = false;

    DianaLifeStealPercentage = .3f;
    bIsABuff = true;
}

void ASpiritState_AguaBuff::BeginPlay() 
{
    Super::BeginPlay();

    if(Spirit) Diana = Cast<ADiana>(Spirit);
}

bool ASpiritState_AguaBuff::ApplyStateEffect() 
{
    bool bWillApply = Super::ApplyStateEffect();

    if(bWillApply)
    {
        if(Diana && !Spirit->IsDead())
        {
            Diana->LifeStealPercentage = DianaLifeStealPercentage;
        }
        else
        {
            Destroy();
            return false;
        }
        
        return true;
    }

    return bWillApply;
}

void ASpiritState_AguaBuff::RemoveStateEffect() 
{
    Super::RemoveStateEffect();

    if(Diana && !Spirit->IsDead())
    {
        Diana->LifeStealPercentage = 0.f;
        GetWorldTimerManager().SetTimer(DestroyStateTimer, this, &ASpiritState::DestroySpiritState, 1.f, false); 
    }
    else
    {
        Destroy();
    }
}
