// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Pickup_Stat.h"
#include "Diana.h"


void APickup_Stat::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    if (OtherActor && OtherActor->IsA(ADiana::StaticClass()) && !bPicked && bPickupActivated)
	{
        bPicked = true;
        ADiana* DianaOverlapped = Cast<ADiana>(OtherActor);
        if(DianaOverlapped)
        {
            if(HealthToIncrement > 0.f) DianaOverlapped->SetNewMaxHealth(HealthToIncrement);
            if(StaminaToIncrement > 0.f) DianaOverlapped->SetNewMaxStamina(StaminaToIncrement);
            if(BaseDamageToIncrement > 0.f) DianaOverlapped->SetNewBaseDamage(BaseDamageToIncrement);   
        }
        
        PlayPickupEffects();
	}
}
