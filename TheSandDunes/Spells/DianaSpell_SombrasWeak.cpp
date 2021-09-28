// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "DianaSpell_SombrasWeak.h"
#include "Diana.h"
#include "Components/SphereComponent.h"
#include "Enemy.h"


ADianaSpell_SombrasWeak::ADianaSpell_SombrasWeak() 
{
    EnemiesDetectionSphere = CreateDefaultSubobject<USphereComponent>(FName("SpellDetectionSphere"));
    EnemiesDetectionSphere->SetupAttachment(GetRootComponent());
    EnemiesDetectionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EnemiesDetectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	EnemiesDetectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
    
    DamageToTake = 120.f;
}

void ADianaSpell_SombrasWeak::BeginPlay() 
{
    Super::BeginPlay();

    if(OwnerPawn)
    {
        Diana = Cast<ADiana>(OwnerPawn);
        if(Diana) Diana->SpiritTakeDamage(DamageToTake, TEXT("Arcanos"), Diana, Diana->GetActorLocation(), false);
    }

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
		EnemiesDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ADianaSpell_SombrasWeak::OnDetectionOverlapBegin);
	}
}

void ADianaSpell_SombrasWeak::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if (OtherActor && OtherActor->IsA(ASpirit::StaticClass()) && OtherActor != OwnerPawn)
	{
		PlayHitEffects();
	}
}

void ADianaSpell_SombrasWeak::OnDetectionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    if (OtherActor && OtherActor->IsA(ASpirit::StaticClass()) && OtherActor != OwnerPawn)
    {
        if(!EnemyTargeted && OtherActor->ActorHasTag(FName("Enemy")))
        {
            EnemyTargeted = Cast<ASpirit>(OtherActor);
        }
    }
}

void ADianaSpell_SombrasWeak::MoveSpellToTarget(float DeltaSeconds) 
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
