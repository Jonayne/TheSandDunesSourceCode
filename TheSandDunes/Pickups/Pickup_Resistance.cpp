// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Pickup_Resistance.h"
#include "Diana.h"
#include "SpiritState.h"


void APickup_Resistance::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    if (OtherActor && OtherActor->IsA(ADiana::StaticClass()) && !bPicked && bPickupActivated)
	{
        bPicked = true;
        ADiana* DianaOverlapped = Cast<ADiana>(OtherActor);
        if(DianaOverlapped)
        {
            DianaOverlapped->ModifyResistance(StatName, StatToIncrement, true);
        }
        
        PlayPickupEffects();
	}
}
