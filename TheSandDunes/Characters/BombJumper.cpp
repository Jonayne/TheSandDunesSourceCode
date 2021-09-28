// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "BombJumper.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"


ABombJumper::ABombJumper() 
{
    WaitBeforeDestroy = 5.f;

    TimeBetweenTryingJumps = 4.f;

    TimeBeforeAction = 1.f;

    Tags.Add(FName("BombJumper"));

    ActionSphere = CreateDefaultSubobject<USphereComponent>(FName("ActionSphere"));
	ActionSphere->SetupAttachment(GetRootComponent());
    ActionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ActionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ActionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

    ActionCollision = CreateDefaultSubobject<USphereComponent>(FName("ActionCollision"));
	ActionCollision->SetupAttachment(GetRootComponent());
    ActionCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ActionCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ActionCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

    bShouldPredictJump = false;
}

void ABombJumper::BeginPlay() 
{
    Super::BeginPlay();

    HaveExploted = true;
    
    ActionCollision->IgnoreActorWhenMoving(this, true);
    ActionCollision->OnComponentBeginOverlap.AddDynamic(this, &ABombJumper::OnOverlapBegin);
}

void ABombJumper::Landed(const FHitResult& Hit) 
{
    Super::Landed(Hit);

    if(!HaveExploted)
    {
        Explote();
    }
}

void ABombJumper::MakeJump()
{
    if(!bStunned && HaveLanded && HaveExploted) 
    {
        HaveLanded = false;
        HaveExploted = false;

        if(JumpSound) JumpSound->Activate(true);
        Jump();

        GetWorldTimerManager().SetTimer(TimerToAction, this, &ABombJumper::JumperAction, TimeBeforeAction, false);
    }
}

void ABombJumper::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    if (OtherActor && OtherActor->IsA(ASpirit::StaticClass()))
	{
		if(OtherActor != this && !HaveExploted && !HaveLanded)
		{
            Explote();
        }
    }
}

void ABombJumper::JumperAction() 
{
    Super::JumperAction();
    
    LaunchCharacter(CalculateJumpVelocity(), true, true);
}

void ABombJumper::KillThisSpirit() 
{
    ActionCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ActionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Super::KillThisSpirit();
}

void ABombJumper::Explote() 
{
    if(ActionSound) ActionSound->Activate(true);
    if(ActionEffect) ActionEffect->Activate(true);

    TArray<AActor*> OverlappingActors;
    ActionSphere->GetOverlappingActors(OverlappingActors, ASpirit::StaticClass());
    
    for (auto& ActorHit : OverlappingActors)
    {
        ASpirit* EnemyIn = Cast<ASpirit>(ActorHit);
        if(EnemyIn)
        {
            if(EnemyIn != this && !EnemyIn->IsDead())
            {
                if(!EnemyIn->IsA(AEnemy::StaticClass()) || (EnemyIn->IsA(AEnemy::StaticClass()) && bShouldDamageAllies))
                {
                    EnemyIn->SpiritTakeDamage(BaseDamage, DamageByTouchNature, this, EnemyIn->GetActorLocation(), false);
                }
            }
        }
    }

    HaveExploted = true;
}

FVector ABombJumper::CalculateJumpVelocity() 
{
    FVector StartLocation = GetActorLocation();
    FVector DestLocation = FVector();
    
    if(bShouldPredictJump)
    {
        DestLocation = GetPredictedTargetLocation();
    }
    else
    {
        SetRandomPointOnArea(GetTargetToHitLocation());
        DestLocation = NextLocation;
    }

    FVector VelocityVector = FVector();

    VelocityVector.X = (DestLocation.X - StartLocation.X)/JumpDuration;
    VelocityVector.Y = (DestLocation.Y - StartLocation.Y)/JumpDuration;
    VelocityVector.Z = (DestLocation.Z - (StartLocation.Z + (FMath::Square(JumpDuration) * -0.5f * 982.f)))/JumpDuration;

    return VelocityVector;
}

FVector ABombJumper::GetPredictedTargetLocation() 
{
    FVector PredictedPosition = FVector();

    if(GetTargetToHit())
    {
        FVector TargetPosition = GetTargetToHitLocation();
        FVector TargetVelocity = GetTargetToHit()->GetVelocity();

        PredictedPosition = TargetPosition + (TargetVelocity * JumpDuration);
    }

    return PredictedPosition;
}
