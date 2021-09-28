// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Pickup_Helper.h"
#include "Diana.h"


void APickup_Helper::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    if (OtherActor && OtherActor->IsA(ADiana::StaticClass()) && !bPicked && bPickupActivated)
	{
        bPicked = true;
        ADiana* DianaOverlapped = Cast<ADiana>(OtherActor);
        if(DianaOverlapped)
        {
            if(HealthToIncrement > 0.f) DianaOverlapped->ModifyHealth(HealthToIncrement);
            if(StaminaToIncrement > 0.f) DianaOverlapped->ModifyStamina(StaminaToIncrement);
        }
        
        PlayPickupEffects();
	}
}
