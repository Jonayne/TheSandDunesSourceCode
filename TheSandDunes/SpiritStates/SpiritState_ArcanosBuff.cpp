// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpiritState_ArcanosBuff.h"
#include "TimerManager.h"
#include "Diana.h"


ASpiritState_ArcanosBuff::ASpiritState_ArcanosBuff() 
{
    StateName = TEXT("ArcanosBuff");
    StateDuration = 10.f;
    StateProbability = 1.f;
    StateCooldown = 15.f;
    PercentageToReduce = .6f;
    
    bIsABuff = true;
}

void ASpiritState_ArcanosBuff::BeginPlay() 
{
    Super::BeginPlay();

    if(Spirit) Diana = Cast<ADiana>(Spirit);
}

bool ASpiritState_ArcanosBuff::ApplyStateEffect() 
{
   bool bWillApply = Super::ApplyStateEffect();
   if(bWillApply && Spirit && !Spirit->IsDead())
   {
       if(Diana)
       {
            CDToModify = Diana->DashCooldown * PercentageToReduce;
            Diana->DashCooldown -= CDToModify;
            Diana->bNoLimitDash = true;
            Diana->EnableDash();
            return true;
       }
   }

   return false;
}

void ASpiritState_ArcanosBuff::RemoveStateEffect() 
{
    Super::RemoveStateEffect();

    if(Spirit && !Spirit->IsDead())
    {   
        if(Diana)
        {
            Diana->DashCooldown += CDToModify;
            Diana->bNoLimitDash = false;
        }
        GetWorldTimerManager().SetTimer(DestroyStateTimer, this, &ASpiritState::DestroySpiritState, 1.f, false); 
    }
    else
    {
        Destroy();
    }

}
