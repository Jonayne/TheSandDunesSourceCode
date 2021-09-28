// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "DianaSpell_SombrasStrong.h"
#include "Diana.h"


void ADianaSpell_SombrasStrong::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    if (OtherActor && OtherActor->IsA(ASpirit::StaticClass()))
	{
		if(OtherActor != OwnerPawn)
		{	
			ASpirit* SpiritHit = Cast<ASpirit>(OtherActor);
            if(SpiritHit)
            {
				if(!SpiritHit->IsDead())
				{
					SpiritHit->SpiritTakeDamage(Damage, FinalTypeOfDamage, OwnerPawn, SpiritHit->GetActorLocation(), false);
				}
                TeleportToTarget(SpiritHit);
            }
			PlayHitEffects();
		}	
	}
}

void ADianaSpell_SombrasStrong::TeleportToTarget(ASpirit* TargetToTeleport) 
{
    if(OwnerPawn)
    {
        FVector TargetLocation = TargetToTeleport->GetActorLocation();
		if(OwnerPawn->IsA(ADiana::StaticClass()))
		{
			ADiana* Diana = Cast<ADiana>(OwnerPawn);
			Diana->EnableShield();
			Diana->Shield();
			Diana->EnableShield();
		}
        OwnerPawn->TeleportTo(TargetLocation, TargetToTeleport->GetActorRotation());
		OwnerPawn->SetInvincibleMode(.5f);
    }
}
