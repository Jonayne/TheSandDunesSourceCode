// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "DianaSpell_Sagrado.h"
#include "Diana.h"
#include "Components/SphereComponent.h"
#include "Enemy.h"


ADianaSpell_Sagrado::ADianaSpell_Sagrado() 
{
    EnemiesDetectionSphere = CreateDefaultSubobject<USphereComponent>(FName("SpellDetectionSphere"));
    EnemiesDetectionSphere->SetupAttachment(GetRootComponent());
	EnemiesDetectionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EnemiesDetectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	EnemiesDetectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
}

void ADianaSpell_Sagrado::BeginPlay() 
{
    Super::BeginPlay();

	EnemiesDetectionSphere->IgnoreActorWhenMoving(OwnerPawn, true);
	EnemiesDetectionSphere->IgnoreActorWhenMoving(this, true);

	TArray<AActor*> OverlappingActors;
    EnemiesDetectionSphere->GetOverlappingActors(OverlappingActors, AEnemy::StaticClass());
	bool bHaveTarget = false;
	
    for (auto& ActorHit : OverlappingActors)
    {
        AEnemy* EnemyIn = Cast<AEnemy>(ActorHit);
        if(EnemyIn)
        {
            if(!EnemyIn->IsDead() && EnemyIn->ActorHasTag(FName("Enemy")))
            {
                EnemyTargeted = EnemyIn;
				bHaveTarget = true;
				break;
            }
        }
    }

	if(!bHaveTarget)
	{
		EnemiesDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ADianaSpell_Sagrado::OnDetectionOverlapBegin);
	}
}

void ADianaSpell_Sagrado::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
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
				}
			}
		}	
	}
}

void ADianaSpell_Sagrado::OnDetectionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    if (OtherActor && OtherActor->IsA(ASpirit::StaticClass()) && OtherActor != OwnerPawn)
    {
        if(!EnemyTargeted && OtherActor->ActorHasTag(FName("Enemy")))
        {
            EnemyTargeted = Cast<ASpirit>(OtherActor);
        }
    }
}

void ADianaSpell_Sagrado::MoveSpellToTarget(float DeltaSeconds) 
{
    if(EnemyTargeted && !EnemyTargeted->IsDead())
    {
        FVector LocationToReach = EnemyTargeted->GetActorLocation();
		SetActorRotation((LocationToReach - GetActorLocation()).Rotation());

        FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), LocationToReach, DeltaSeconds, 8000.f);
        SetActorLocation(NewLocation);
    }
    else
    {   
        EnemyTargeted = nullptr;
        MoveStraightLine(DeltaSeconds);
    }
}

void ADianaSpell_Sagrado::PlayHitEffects() 
{
	Super::PlayHitEffects();

	EnemiesDetectionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
