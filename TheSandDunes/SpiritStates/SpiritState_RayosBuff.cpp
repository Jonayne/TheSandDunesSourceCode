// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpiritState_RayosBuff.h"
#include "TimerManager.h"
#include "Diana.h"


ASpiritState_RayosBuff::ASpiritState_RayosBuff() 
{
    StateName = TEXT("RayosBuff");
    StateDuration = 10.f;
    StateProbability = 1.f;
    StateCooldown = 15.f;
    DodgeProbability = .7f;

    bIsABuff = true;
}

bool ASpiritState_RayosBuff::ApplyStateEffect() 
{
   bool bWillApply = Super::ApplyStateEffect();
   if(bWillApply)
   {
       if(Spirit && !Spirit->IsDead())
       {
           Spirit->DodgeProbabilityIfState += DodgeProbability;
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

void ASpiritState_RayosBuff::RemoveStateEffect() 
{
    Super::RemoveStateEffect();

    if(Spirit && !Spirit->IsDead())
    {   
        Spirit->DodgeProbabilityIfState -= DodgeProbability;
        GetWorldTimerManager().SetTimer(DestroyStateTimer, this, &ASpiritState::DestroySpiritState, 1.f, false);
    }
    else
    {
        Destroy();
    }
}
