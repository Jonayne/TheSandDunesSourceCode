// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpiritState_SombrasDebuf.h"
#include "TimerManager.h"
#include "Spirit.h"
#include "Diana.h"

ASpiritState_SombrasDebuf::ASpiritState_SombrasDebuf() 
{
    StateName = TEXT("SombrasDebuf");
    StateDuration = 10.f;
    StateProbability = 1.f;
    StateCooldown = 15.f;

    StatePerTickTimer = 1.f;
	bIsEffectPerTick = true;

    HealthToDecreasePerTick = -30.f;
    ManaToDecreasePerTick = -20.f;
}

void ASpiritState_SombrasDebuf::BeginPlay() 
{
    Super::BeginPlay();

    if(Spirit) Diana = Cast<ADiana>(Spirit);
}

bool ASpiritState_SombrasDebuf::ApplyStateEffect() 
{
    bool bWillApply = Super::ApplyStateEffect();
    if(bWillApply)
    {
        if(Diana)
        {
            Diana->ModifyStamina(-(Diana->MaxStamina*.5f));
        }
    }
    return bWillApply;
}

void ASpiritState_SombrasDebuf::RemoveStateEffect() 
{
    Super::RemoveStateEffect();

    if(Spirit && !Spirit->IsDead())
    {
        GetWorldTimerManager().SetTimer(DestroyStateTimer, this, &ASpiritState::DestroySpiritState, 1.f, false); 
    }
    else
    {
        Destroy();
    }
}

void ASpiritState_SombrasDebuf::ApplyTimerPerTick() 
{
    if(Spirit && !Spirit->IsDead())
    {
        Spirit->ModifyHealth(HealthToDecreasePerTick);
        if(Diana) 
        {
            Diana->ModifyStamina(ManaToDecreasePerTick);
        }
    }
    else
    {
        Destroy();
    }
}
