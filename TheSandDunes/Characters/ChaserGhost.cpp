// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "ChaserGhost.h"
#include "Spirit.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AChaserGhost::AChaserGhost() 
{
    SpiritMaxBuffSpeed = 2200.f;
	SpiritWalkSpeed = 1700.f;

    WaitBeforeDestroy = 5.f;

    ExplosionDamage = 300.f;
    TimeToExplote = 5.f;

    Tags.Add(FName("ChaserGhost"));

    ExplosionSound = CreateDefaultSubobject<UAudioComponent>(FName("ExplosionSound"));
	ExplosionSound->SetupAttachment(GetMesh());
	ExplosionSound->bAutoActivate = false;
	
	ExplosionEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("ExplosionEffect"));
	ExplosionEffect->SetupAttachment(GetMesh());
	ExplosionEffect->bAutoActivate = false;

    ExplosionSphere = CreateDefaultSubobject<USphereComponent>(FName("ExplosionSphere"));
	ExplosionSphere->SetupAttachment(GetRootComponent());
    ExplosionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ExplosionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ExplosionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

    MeleeCapsule = CreateDefaultSubobject<UCapsuleComponent>(FName("MeleeCapsule"));
	MeleeCapsule->SetupAttachment(GetRootComponent());
    MeleeCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeleeCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	MeleeCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

    ActionSound = CreateDefaultSubobject<UAudioComponent>(FName("ActionSound"));
	ActionSound->SetupAttachment(GetMesh());
	ActionSound->bAutoActivate = false;

    GetCharacterMovement()->MaxFlySpeed = 2000.f;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

    TimesToExploteBeforeDeath = 1;
    bCanExplote = true;
}

void AChaserGhost::BeginPlay() 
{
    Super::BeginPlay();

    MeleeCapsule->IgnoreActorWhenMoving(this, true);
    MeleeCapsule->OnComponentBeginOverlap.AddDynamic(this, &AChaserGhost::OnOverlapBegin);
}

void AChaserGhost::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    if (OtherActor && OtherActor->IsA(ASpirit::StaticClass()))
	{
		if(OtherActor != this && !OtherActor->IsA(AChaserGhost::StaticClass()))
		{
            Explote();
        }
    }
}

float AChaserGhost::SpiritTakeDamage(float DamageAmount, FString TypeOfDamage, AActor* DamageCauser, FVector DamageLocation, bool bIsABeam) 
{
    if(DamageCauser)
    {
        ASpirit* SpiritInstigator = Cast<ASpirit>(DamageCauser);
        if(SpiritInstigator && !SpiritInstigator->IsA(AChaserGhost::StaticClass()))
        {
            SpiritInstigator->SpiritTakeDamage(DamageAmount, TypeOfDamage, this, DamageCauser->GetActorLocation(), false);
        }
    }

    return 0.f;
}

void AChaserGhost::Explote() 
{
    if(bCanExplote)
    {
        if(ActionSound) ActionSound->Activate(true);
        if(ExplosionSound) ExplosionSound->Activate(true);
        if(ExplosionEffect) ExplosionEffect->Activate(true);

        TArray<AActor*> OverlappingActors;
        ExplosionSphere->GetOverlappingActors(OverlappingActors, ASpirit::StaticClass());
        
        for (auto& ActorHit : OverlappingActors)
        {
            ASpirit* EnemyIn = Cast<ASpirit>(ActorHit);
            if(EnemyIn)
            {
                if(EnemyIn != this && !EnemyIn->IsA(AChaserGhost::StaticClass()) && !EnemyIn->IsDead())
                {
                    EnemyIn->SpiritTakeDamage(ExplosionDamage, DamageByTouchNature, this, EnemyIn->GetActorLocation(), false);
                }
            }
        }
        TimesExploted++;
        
            
        if(TimesExploted < TimesToExploteBeforeDeath)
        {
            ACharacter::LaunchCharacter(FVector(600.f), true, true);
            GetWorldTimerManager().SetTimer(TimerToExploteHandler, this, &AChaserGhost::Explote, TimeToExplote+10.f, false);
        }
        else
        {
            bCanExplote = false;
            GetWorldTimerManager().ClearTimer(TimerToExploteHandler);
            MeleeCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            ExplosionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

            KillThisSpirit();
        }
    }
}

void AChaserGhost::OnHaveSeenAnEnemy() 
{
    Super::OnHaveSeenAnEnemy();
    if(bCanStartFight)
	{
        GetWorldTimerManager().SetTimer(TimerToExploteHandler, this, &AChaserGhost::Explote, TimeToExplote, false);
    }    
}
