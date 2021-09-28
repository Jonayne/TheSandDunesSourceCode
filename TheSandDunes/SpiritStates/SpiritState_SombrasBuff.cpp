// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpiritState_SombrasBuff.h"
#include "TimerManager.h"
#include "Diana.h"

ASpiritState_SombrasBuff::ASpiritState_SombrasBuff() 
{
    StateName = TEXT("SombrasBuff");
    StateDuration = 8.f;
    StateProbability = 1.f;
    StateCooldown = 30.f;

    SpeedToIncrease = 500.f;
    SecundJumpIncrease = 1000.f;
    ExtraDamage = 50.f;

    bIsABuff = true;
}

void ASpiritState_SombrasBuff::BeginPlay() 
{
    Super::BeginPlay();

    if(Spirit) Diana = Cast<ADiana>(Spirit);
}

bool ASpiritState_SombrasBuff::ApplyStateEffect() 
{
   bool bWillApply = Super::ApplyStateEffect();
   if(bWillApply)
   {
        if(Diana)
        {
            Diana->AddHeightToSecondJump(SecundJumpIncrease);
            Diana->BaseDamage += ExtraDamage;
        }
        Spirit->ChangeSpiritSpeed(SpeedToIncrease);

        return true;
   }

   return false;
}

void ASpiritState_SombrasBuff::RemoveStateEffect() 
{
    Super::RemoveStateEffect();

    if(Spirit && !Spirit->IsDead())
    {   
        if(Diana)
        {
            Diana->AddHeightToSecondJump(-SecundJumpIncrease);
            Diana->BaseDamage -= ExtraDamage;
        }
        Spirit->ChangeSpiritSpeed(-SpeedToIncrease);

        GetWorldTimerManager().SetTimer(DestroyStateTimer, this, &ASpiritState::DestroySpiritState, 1.f, false); 
    }
    else
    {
        Destroy();
    }
}
