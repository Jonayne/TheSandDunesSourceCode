// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpiritState_SagradoDebuf.h"
#include "TimerManager.h"
#include "Spirit.h"


ASpiritState_SagradoDebuf::ASpiritState_SagradoDebuf() 
{
    StateName = TEXT("SagradoDebuf");
    StateDuration = 10.f;
    StateProbability = 1.f;
    StateCooldown = 15.f;

    DamageMultiplierReduction = .7f;
}

bool ASpiritState_SagradoDebuf::ApplyStateEffect() 
{
   bool bWillApply = Super::ApplyStateEffect();
   if(bWillApply && Spirit && !Spirit->IsDead())
   {
       Spirit->DamageMultiplier = DamageMultiplierReduction;

       return true;
   }
   else
   {
       Destroy();
   }

   return false;
}

void ASpiritState_SagradoDebuf::RemoveStateEffect() 
{
    Super::RemoveStateEffect();

    if(Spirit && !Spirit->IsDead())
    {   
        Spirit->DamageMultiplier = 1.f;
        GetWorldTimerManager().SetTimer(DestroyStateTimer, this, &ASpiritState::DestroySpiritState, 1.f, false); 
    }
    else
    {
        Destroy();
    }
}
