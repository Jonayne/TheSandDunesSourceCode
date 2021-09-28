// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Dasher.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"


ADasher::ADasher() 
{
    WaitBeforeDestroy = 5.f;
    DashDuration = 2.f;
    TimeBefweenDashes = 2.f;
    Tags.Add(FName("Dasher"));

    DashSound = CreateDefaultSubobject<UAudioComponent>(FName("DashSound"));
	DashSound->SetupAttachment(GetMesh());
	DashSound->bAutoActivate = false;
    
    ActionSound = CreateDefaultSubobject<UAudioComponent>(FName("ActionSound"));
	ActionSound->SetupAttachment(GetMesh());
	ActionSound->bAutoActivate = false;

	DashEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("DashEffect"));
	DashEffect->SetupAttachment(GetMesh());
	DashEffect->bAutoActivate = false;

    ActionEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("ActionEffect"));
	ActionEffect->SetupAttachment(GetMesh());
	ActionEffect->bAutoActivate = false;

    ActionParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(FName("ActionParticleSystem"));
	ActionParticleSystem->SetupAttachment(GetMesh());
	ActionParticleSystem->bAutoActivate = false;

    TouchCapsule = CreateDefaultSubobject<UCapsuleComponent>(FName("SpellDetectionCapsule"));
    TouchCapsule->SetupAttachment(GetRootComponent());
	TouchCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TouchCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	TouchCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

    DashDirection = CreateDefaultSubobject<UArrowComponent>(FName("DashDirection"));
	DashDirection->SetupAttachment(GetMesh());
}

void ADasher::BeginPlay() 
{
    Super::BeginPlay();

    TouchCapsule->IgnoreActorWhenMoving(this, true);

    TimeBeforeDashAux = TimeBefweenDashes + DashDuration;
    TouchCapsule->OnComponentBeginOverlap.AddDynamic(this, &ADasher::OnTouchedAnEnemy);
}

void ADasher::MakeDash()
{
    if(!bStunned) 
    {
        if(DashSound) DashSound->Activate(true);

        GetCharacterMovement()->GroundFriction = 0.f;
        GetCharacterMovement()->BrakingDecelerationWalking = 0.f;

        if(DashEffect) DashEffect->Activate();
        if(ActionEffect) ActionEffect->Activate(true);
        if(ActionParticleSystem) ActionParticleSystem->Activate(true);
        LaunchCharacter(CalculateDashVelocity(), true, true);
        GetWorldTimerManager().SetTimer(DashDurationTimer, this, &ADasher::StopDash, DashDuration, false);
        GetWorldTimerManager().SetTimer(TimerToAction, this, &ADasher::DasherAction, TimeBeforeAction, false);
    }
}

void ADasher::DasherAction() 
{
}

void ADasher::StopDash() 
{
    if(DashEffect) DashEffect->Deactivate();
    if(ActionEffect) ActionEffect->Deactivate();
    if(ActionParticleSystem) ActionParticleSystem->Deactivate();

    GetCharacterMovement()->GroundFriction = 8.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2048.f;
}

void ADasher::OnTouchedAnEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    if (OtherActor && OtherActor->IsA(ASpirit::StaticClass()))
	{
        ASpirit* SpiritHit = Cast<ASpirit>(OtherActor);
    
        if(!SpiritHit->IsDead())
        {					
            if(ActionSound) ActionSound->Activate(true);
            if(bFromSweep)
            {
                SpiritHit->SpiritTakeDamage(DamageByTouch, DamageByTouchNature, this, SweepResult.ImpactPoint, false);
            }
            else
            {
                SpiritHit->SpiritTakeDamage(DamageByTouch, DamageByTouchNature, this, SpiritHit->GetActorLocation(), false);
            }
        }	
	}
}

FVector ADasher::CalculateDashVelocity() 
{
    FVector StartLocation = GetActorLocation();
    FVector DestLocation = GetTargetToHitLocation();
    
    DashDirection->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(StartLocation, DestLocation)); 

    FVector VelocityVector = DashDirection->GetForwardVector() * FVector::Dist(StartLocation, DestLocation);

    return VelocityVector;
}

void ADasher::KillThisSpirit() 
{
    GetWorldTimerManager().ClearTimer(TimerToAction);
    GetWorldTimerManager().ClearTimer(TimerToDash);

    TouchCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TouchCapsule->OnComponentBeginOverlap.Clear();
    
    Super::KillThisSpirit();
}

void ADasher::OnHaveSeenAnEnemy() 
{
    Super::OnHaveSeenAnEnemy();
    if(!bExecutedOnce && bCanStartFight)
    {
        bExecutedOnce = true;
        MakeDash();
        GetWorldTimerManager().SetTimer(TimerToDash, this, &ADasher::MakeDash, TimeBeforeDashAux, true);
    }
}