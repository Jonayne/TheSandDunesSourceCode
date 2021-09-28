// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "DianaSpell_NaturalezaWeak.h"
#include "Diana.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"


ADianaSpell_NaturalezaWeak::ADianaSpell_NaturalezaWeak() 
{
	ThrustMultiplier = 2500.f;
	DamageMinimum = 50.f;
	VelocityToReduceDamage = 400.f;
}


void ADianaSpell_NaturalezaWeak::BeginPlay() 
{
    Super::BeginPlay();
	if(OwnerPawn)
	{
		Diana = Cast<ADiana>(OwnerPawn);
	}
}

void ADianaSpell_NaturalezaWeak::ReduceDamage(float DeltaTime) 
{
	Damage = FMath::FInterpConstantTo(Damage, DamageMinimum, DeltaTime, VelocityToReduceDamage);
}

void ADianaSpell_NaturalezaWeak::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if (OtherActor && OtherActor->IsA(ASpirit::StaticClass()))
	{
		if(OtherActor != OwnerPawn)
		{	
			ASpirit* SpiritHit = Cast<ASpirit>(OtherActor);
			PlayHitEffects();

			if (OwnerPawn)
			{
				if(!SpiritHit->IsDead())
				{					
					if(bFromSweep)
					{
						SpiritHit->SpiritTakeDamage(Damage, FinalTypeOfDamage, OwnerPawn, SweepResult.ImpactPoint, false);
					}
					else
					{
						SpiritHit->SpiritTakeDamage(Damage, FinalTypeOfDamage, OwnerPawn, SpiritHit->GetActorLocation(), false);
					}

					if(SpiritHit->bSpellsCanMoveActor)
					{
						FVector SpellDirection = UKismetMathLibrary::GetDirectionUnitVector(OwnerPawn->GetActorLocation(), SpiritHit->GetActorLocation());
						FVector ThrustVelocity = SpellDirection * (ThrustMultiplier - (ThrustMultiplier * SpiritHit->SpiritMass));
						if(!ThrustVelocity.IsZero())
						{
							SpiritHit->LaunchCharacter(ThrustVelocity, false, false);
						}
					}
				}
			}
		}	
	}
}


void ADianaSpell_NaturalezaWeak::PlayHitEffects() 
{
	Super::PlayHitEffects();

	if(NiagaraParticle) NiagaraParticle->SetHiddenInGame(true);
}

void ADianaSpell_NaturalezaWeak::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	if(Damage != DamageMinimum)
	{
		ReduceDamage(DeltaTime);
	}
}
