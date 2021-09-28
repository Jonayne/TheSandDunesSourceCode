// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "DianaShield.h"
#include "Diana.h"
#include "Spell.h"

void ADianaShield::BeginPlay() 
{
    Super::BeginPlay();

    if(Spirit)
    {
        Diana = Cast<ADiana>(Spirit); 
    }
}

void ADianaShield::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    if (OtherActor && OtherActor->IsA(ASpell::StaticClass()))
	{
		ASpell* OtherSpell = Cast<ASpell>(OtherActor);
		ASpirit* SpiritOwner = OtherSpell->GetOwnerSpirit();
		if(SpiritOwner != Diana)
		{
			OtherSpell->PlayHitEffects();

			if(Diana)
			{
				Diana->ModifyHealth(OtherSpell->GetSpellDamage() * .5f);
				Diana->ModifyStamina(OtherSpell->GetSpellDamage() * .5f);
			}
		}
		
	}
}
