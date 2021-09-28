// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpiritState_NaturalezaBuff.h"
#include "TimerManager.h"
#include "Diana.h"

ASpiritState_NaturalezaBuff::ASpiritState_NaturalezaBuff() 
{
    StateName = TEXT("NaturalezaBuff");
    StateDuration = 8.f;
    StateProbability = 1.f;
    StateCooldown = 30.f;

    MaxStaminaToIncrease = 200.f;
    StaminaRegenerationRateToIncrease = 50.f;

    bIsABuff = true;
}

void ASpiritState_NaturalezaBuff::BeginPlay() 
{
    Super::BeginPlay();

    if(Spirit) Diana = Cast<ADiana>(Spirit);
}

bool ASpiritState_NaturalezaBuff::ApplyStateEffect() 
{
   bool bWillApply = Super::ApplyStateEffect();
   if(bWillApply && Spirit && !Spirit->IsDead())
   {
       if(Diana)
       {
           Diana->MaxStamina += MaxStaminaToIncrease;
           Diana->Stamina = Diana->MaxStamina;
           Diana->StaminaRegenerationRate += StaminaRegenerationRateToIncrease;
           Diana->UpdateMaxStamina();
           return true;
       }
   }
   else
   {
       Destroy();
   }

   return false;
}

void ASpiritState_NaturalezaBuff::RemoveStateEffect() 
{
    Super::RemoveStateEffect();

    if(Spirit && !Spirit->IsDead())
    {   
        if(Diana)
        {
            Diana->MaxStamina -= MaxStaminaToIncrease;
            Diana->StaminaRegenerationRate -= StaminaRegenerationRateToIncrease;
            Diana->Stamina = Diana->MaxStamina;
            Diana->UpdateMaxStamina();
        } 
        GetWorldTimerManager().SetTimer(DestroyStateTimer, this, &ASpiritState::DestroySpiritState, 1.f, false);
    }
    else
    {
        Destroy();
    }
}
