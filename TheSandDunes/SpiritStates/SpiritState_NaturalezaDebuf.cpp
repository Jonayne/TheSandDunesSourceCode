// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpiritState_NaturalezaDebuf.h"
#include "TimerManager.h"
#include "Diana.h"


ASpiritState_NaturalezaDebuf::ASpiritState_NaturalezaDebuf() 
{
    StateName = TEXT("NaturalezaDebuf");
    StateDuration = 10.f;
    StateProbability = 1.f;
    StateCooldown = 15.f;

    SlowToApply = -500.f;
    StaminaRegenerationToDecrease = -50.f;
}

void ASpiritState_NaturalezaDebuf::BeginPlay() 
{
    Super::BeginPlay();

    if(Spirit) Diana = Cast<ADiana>(Spirit);
}

bool ASpiritState_NaturalezaDebuf::ApplyStateEffect() 
{
   bool bWillApply = Super::ApplyStateEffect();
   if(bWillApply && Spirit && !Spirit->IsDead())
   {
       Spirit->ChangeSpiritSpeed(SlowToApply);
       if(Diana)
       {
           Diana->StaminaRegenerationRate += StaminaRegenerationToDecrease;
       }
       return true;
   }
   else
   {
       Destroy();
   }

   return false;
}

void ASpiritState_NaturalezaDebuf::RemoveStateEffect() 
{
    Super::RemoveStateEffect();

    if(Spirit && !Spirit->IsDead())
    {   
        Spirit->ChangeSpiritSpeed((-1) * SlowToApply);
        if(Diana)
        {
            Diana->StaminaRegenerationRate += ((-1) * StaminaRegenerationToDecrease);
        }
        GetWorldTimerManager().SetTimer(DestroyStateTimer, this, &ASpiritState::DestroySpiritState, 1.f, false); 
    }
    else
    {
        Destroy();
    }
    
}
