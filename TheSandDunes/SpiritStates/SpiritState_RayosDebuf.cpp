// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpiritState_RayosDebuf.h"
#include "TimerManager.h"
#include "Spirit.h"


ASpiritState_RayosDebuf::ASpiritState_RayosDebuf() 
{
    StateName = TEXT("RayosDebuf");
    StateDuration = 10.f;
    StateProbability = 1.f;
    StateCooldown = 15.f;
}

bool ASpiritState_RayosDebuf::ApplyStateEffect() 
{
   bool bWillApply = Super::ApplyStateEffect();
   if(bWillApply)
   {
       if(Spirit && !Spirit->IsDead())
       {
           StateDuration = Spirit->StunSpirit();
           GetWorldTimerManager().SetTimer(StopEffectTimer, this, &ASpiritState::RemoveStateEffect, StateDuration, false);
       }
       else
       {
           Destroy();
           return false;
       }
       
       return true;
   }

   return false;
}

void ASpiritState_RayosDebuf::RemoveStateEffect() 
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
