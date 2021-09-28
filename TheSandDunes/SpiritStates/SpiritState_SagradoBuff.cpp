// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpiritState_SagradoBuff.h"
#include "TimerManager.h"
#include "Diana.h"


ASpiritState_SagradoBuff::ASpiritState_SagradoBuff() 
{
    StateName = TEXT("SagradoBuff");
    StateDuration = 10.f;
    StateProbability = 1.f;
    StateCooldown = 50.f;

    HealthToIncrease = 300.f;

    bIsABuff = true;

    StatsToIncrease.Add(TEXT("Arcanos"), .2f);
    StatsToIncrease.Add(TEXT("Agua"), .1f);
    StatsToIncrease.Add(TEXT("Rayos"), .2f);
    StatsToIncrease.Add(TEXT("Sombras"), .3f);
    StatsToIncrease.Add(TEXT("Sagrado"), .3f);
}

void ASpiritState_SagradoBuff::BeginPlay() 
{
    Super::BeginPlay();

    if(Spirit) Diana = Cast<ADiana>(Spirit);
}

bool ASpiritState_SagradoBuff::ApplyStateEffect() 
{
   bool bWillApply = Super::ApplyStateEffect();
   if(bWillApply)
   {
        if(Spirit && !Spirit->IsDead())
        {
            Spirit->MaxHealth += HealthToIncrease;
            
            Spirit->ModifyResistance(TEXT("Arcanos"), StatsToIncrease[TEXT("Arcanos")], false);
            Spirit->ModifyResistance(TEXT("Agua"), StatsToIncrease[TEXT("Agua")], false);
            Spirit->ModifyResistance(TEXT("Rayos"), StatsToIncrease[TEXT("Rayos")], false);
            Spirit->ModifyResistance(TEXT("Sombras"), StatsToIncrease[TEXT("Sombras")], false);
            Spirit->ModifyResistance(TEXT("Sagrado"), StatsToIncrease[TEXT("Sagrado")], false);

            if(Diana)
            {
                Diana->UpdateMaxHealth();
                Diana->ExtraHealth = HealthToIncrease;
            }
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

void ASpiritState_SagradoBuff::RemoveStateEffect() 
{
    Super::RemoveStateEffect();

    if(Spirit && !Spirit->IsDead())
    {   
        Spirit->MaxHealth -= HealthToIncrease;
        Spirit->ModifyResistance(TEXT("Arcanos"), -StatsToIncrease[TEXT("Arcanos")], false);
        Spirit->ModifyResistance(TEXT("Agua"), -StatsToIncrease[TEXT("Agua")], false);
        Spirit->ModifyResistance(TEXT("Rayos"), -StatsToIncrease[TEXT("Rayos")], false);
        Spirit->ModifyResistance(TEXT("Sombras"), -StatsToIncrease[TEXT("Sombras")], false);
        Spirit->ModifyResistance(TEXT("Sagrado"), -StatsToIncrease[TEXT("Sagrado")], false);

        if(Diana)
        {
            Diana->UpdateMaxHealth();
            Diana->ExtraHealth = 0.f;
        }

        GetWorldTimerManager().SetTimer(DestroyStateTimer, this, &ASpiritState::DestroySpiritState, 1.f, false);
    }
    else
    {
        Destroy();
    }
}
