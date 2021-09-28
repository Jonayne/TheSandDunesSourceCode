// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#include "Spell.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "NiagaraComponent.h"
#include "Diana.h"
#include "Enemy.h"
#include "Perception/AISense_Hearing.h"


// Sets default values
ASpell::ASpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(FName("SpellSphere"));
	Sphere->SetCollisionProfileName(FName("Spells"));
	RootComponent = Sphere;

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(FName("SpellParticle"));
	Particle->SetupAttachment(GetRootComponent());

	Audio = CreateDefaultSubobject<UAudioComponent>(FName("SpellAudio"));
	Audio->SetupAttachment(GetRootComponent());

	HitParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("SpellHitParticle"));
	HitParticle->SetupAttachment(GetRootComponent());
	HitParticle->bAutoActivate = false;
	
	HitAudio = CreateDefaultSubobject<UAudioComponent>(FName("SpellHitAudio"));
	HitAudio->SetupAttachment(GetRootComponent());
	HitAudio->bAutoActivate = false;

	NiagaraParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("SpellNiagaraParticle"));
	NiagaraParticle->SetupAttachment(GetRootComponent());

	MuzzleParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("SpellMuzzleNiagaraParticle"));
	MuzzleParticle->SetupAttachment(GetRootComponent());

	NiagaraHitParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("SpellNiagaraHitParticle"));
	NiagaraHitParticle->SetupAttachment(GetRootComponent());
	NiagaraHitParticle->bAutoActivate = false;

	SpellSpeed = 5000.f;
	Damage = 15.f;
	SpellLifeSpan = 3.f;
	WaitBeforeDestroy = 1.f;
	NumberOfProjectilesPerSpell = 1;
	TimeBetweenProjectiles = 0.3f;
	HitPoint = FVector(0.f);
	SpellZigZagFrecuency = 10.f;
	SpellZigZagAmplitud = 3000.f;

	bShouldHitWorld = true;
	TimeShouldUseAutoAIM = 1.f;
}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerPawn = GetOwner<ASpirit>();
	
	Sphere->IgnoreActorWhenMoving(OwnerPawn, true);
	Sphere->IgnoreActorWhenMoving(this, true);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASpell::OnOverlapBegin);
	
	MaxSpellSpeed = SpellSpeed;
	
	Diameter = Sphere->GetScaledSphereRadius()*2.f;
	InitializeSpellRotation();

	if(bShouldHitWorld)
	{
		UpdateHitPoint();
	}

	SetLifeSpan(SpellLifeSpan);
}

void ASpell::InitializeSpellRotation() 
{
	if (OwnerPawn)
	{
		FinalTypeOfDamage = SpellType;

		if(OwnerPawn->IsA(ADiana::StaticClass()))
		{
			ADiana* Diana = Cast<ADiana>(OwnerPawn);
			Diana->UpdateDianaTargetToHit(30000.f);
			TargetLocationToHit = Diana->GetTargetToHitLocation();
			SetActorRotation((TargetLocationToHit - GetActorLocation()).Rotation());
		}
		else 
		{
			OwnerEnemy = Cast<AEnemy>(OwnerPawn);
			if(OwnerEnemy && OwnerEnemy->bShouldUseDamageByTouchNatureOnSpells)
			{
				FinalTypeOfDamage = OwnerEnemy->DamageByTouchNature;
			}
			
			Target = OwnerPawn->GetTargetToHit();
			if(Target)
			{
				TargetLocationToHit = GetPredictedPositionToAttack();
				SetActorRotation((TargetLocationToHit - GetActorLocation()).Rotation());
			}
			else
			{
				FVector OwnerForwardVector = OwnerPawn->GetActorForwardVector();
				TargetLocationToHit = OwnerForwardVector * 30000.f;
				SetActorRotation(OwnerForwardVector.Rotation());
			}
		}
	}
}

void ASpell::UpdateHitPoint() 
{
	if(!TargetLocationToHit.IsZero())
	{
		FHitResult Hit;
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerPawn);
		QueryParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), TargetLocationToHit, ECollisionChannel::ECC_WorldStatic, QueryParams);

		AActor* HitActor = Hit.GetActor();

		if (Hit.bBlockingHit && HitActor && 
					!HitActor->IsA(ASpell::StaticClass()) && !HitActor->IsA(ASpirit::StaticClass()) && HitActor != OwnerPawn)
		{
			HitPoint = Hit.ImpactPoint;
		}
		else
		{
		    HitPoint = FVector(0.f);
		}
	}
}

// Called every frame
void ASpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!HitPoint.IsZero())
	{
		if(FVector::PointsAreNear(GetActorLocation(), HitPoint, Diameter))
		{
			PlayHitEffects();
		}
	}
}

void ASpell::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{	
	// SI CHOCAMOS CON UN ESPIRITU.
	if (OtherActor && OtherActor->IsA(ASpirit::StaticClass()))
	{
		if(OtherActor != OwnerPawn)
		{	
			if(OwnerEnemy && OtherActor->IsA(AEnemy::StaticClass()) && !OwnerEnemy->bShouldDamageAllies)
			{
				return;
			}

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

void ASpell::MoveStraightLine(float DeltaSeconds) 
{
	FVector SpellForwardVector = GetActorForwardVector();
	FVector ActorLocation = GetActorLocation();

	FVector NewSpellLocation = ((SpellForwardVector * DeltaSeconds) * SpellSpeed) + ActorLocation;
	SetActorLocation(NewSpellLocation);
}

void ASpell::MoveZigZagLine(float DeltaSeconds) 
{
	MoveWithAutoAIM(DeltaSeconds);

	float Amplitud = GetGameTimeSinceCreation() * SpellZigZagFrecuency;
	float Cos = FMath::Cos(Amplitud);
	float CosDotAmplitud = Cos * SpellZigZagAmplitud;

	FVector NewActorLocalSet = FVector(0.f, CosDotAmplitud, 0.f) * DeltaSeconds;
	AddActorLocalOffset(NewActorLocalSet);
}

void ASpell::MoveZigZagLineXAxis(float DeltaSeconds) 
{
	MoveStraightLine(DeltaSeconds);

	float Amplitud = GetGameTimeSinceCreation() * SpellZigZagFrecuency;
	float Cos = FMath::Cos(Amplitud);
	float CosDotAmplitud = Cos * SpellZigZagAmplitud;

	FVector NewActorLocalSet = FVector(CosDotAmplitud, 0.f, 0.f) * DeltaSeconds;
	AddActorLocalOffset(NewActorLocalSet);
}

void ASpell::MoveZigZagLineZAxis(float DeltaSeconds) 
{
	MoveWithAutoAIM(DeltaSeconds);

	float Amplitud = GetGameTimeSinceCreation() * SpellZigZagFrecuency;
	float Cos = FMath::Cos(Amplitud);
	float CosDotAmplitud = Cos * SpellZigZagAmplitud;

	FVector NewActorLocalSet = FVector(0.f, 0.f, CosDotAmplitud) * DeltaSeconds;
	AddActorLocalOffset(NewActorLocalSet);
}

void ASpell::MoveZigZagLineAllAxis(float DeltaSeconds) 
{
	MoveWithAutoAIM(DeltaSeconds);

	float Amplitud = GetGameTimeSinceCreation() * SpellZigZagFrecuency;
	float Cos = FMath::Cos(Amplitud);
	float CosDotAmplitud = Cos * SpellZigZagAmplitud;

	FVector NewActorLocalSet = FVector(CosDotAmplitud, CosDotAmplitud, CosDotAmplitud) * DeltaSeconds;
	AddActorLocalOffset(NewActorLocalSet);
}


void ASpell::AddSpellRotation(FRotator AddRotator) 
{
	SetActorRotation(GetActorRotation() + AddRotator);
}

void ASpell::MoveStraightLineWithAceleration(float DeltaSeconds) 
{
	FVector SpellForwardVector = GetActorForwardVector();
	FVector ActorLocation = GetActorLocation();

	float Aceleration = DeltaSeconds * SpellAcelerationRate;

	FVector NewSpellLocation = (SpellForwardVector * Aceleration) + ActorLocation;
	SetActorLocation(NewSpellLocation);
}

void ASpell::MoveWithAutoAIM(float DeltaSeconds) 
{
	AutoAimTimeAux = AutoAimTimeAux + DeltaSeconds;
	if(Target && !Target->IsDead() && AutoAimTimeAux <= TimeShouldUseAutoAIM)
    {
        FVector LocationToReach = Target->GetActorLocation();
		SetActorRotation((LocationToReach - GetActorLocation()).Rotation());

        FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), LocationToReach, DeltaSeconds, SpellSpeed);
        SetActorLocation(NewLocation);
    }
    else
    {
        MoveStraightLine(DeltaSeconds);
    }
}

void ASpell::PlayHitEffects() 
{
	if(Particle) Particle->Deactivate();
	if(NiagaraParticle) NiagaraParticle->Deactivate();
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (HitParticle) HitParticle->Activate(true);
	if (NiagaraHitParticle) NiagaraHitParticle->Activate(true);
	if (HitAudio) HitAudio->Activate(true);
	
	if(OwnerPawn)
	{
		UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 1.f, OwnerPawn, 5000.f, FName("HearEvent"));
	}
	
	// para que deje de moverse
	SetActorTickEnabled(false);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASpell::DestroySpell, WaitBeforeDestroy, false);
}

void ASpell::PlayHitEffectsAtLocation(ASpirit* SpiritHit) 
{
	if (HitParticle)
	{
		HitParticle->SetWorldLocation(SpiritHit->GetActorLocation());
		HitParticle->Activate(true);
	} 

	if (NiagaraHitParticle) 
	{
		NiagaraHitParticle->SetWorldLocation(SpiritHit->GetActorLocation());
		NiagaraHitParticle->Activate(true);
	}

	if (HitAudio)
	{
		HitAudio->SetWorldLocation(SpiritHit->GetActorLocation());
		HitAudio->Activate(true);
	} 

	if(OwnerPawn)
	{
		UAISense_Hearing::ReportNoiseEvent(this, SpiritHit->GetActorLocation(), 1.f, OwnerPawn, 5000.f, FName("HearEvent"));
	}
}

void ASpell::PlayHitEffectsAtLocation(FVector SpiritHitLocation) 
{
	if (HitParticle)
	{
		HitParticle->SetWorldLocation(SpiritHitLocation);
		HitParticle->Activate(true);
	} 

	if (NiagaraHitParticle) 
	{
		NiagaraHitParticle->SetWorldLocation(SpiritHitLocation);
		NiagaraHitParticle->Activate(true);
	}

	if (HitAudio)
	{
		HitAudio->SetWorldLocation(SpiritHitLocation);
		HitAudio->Activate(true);
	} 
}

void ASpell::DestroySpell() 
{
	Destroy();
}

float ASpell::GetSpellCooldown() const
{
	return SpellCooldown;
}

float ASpell::GetTimeBetweenProjectiles() const
{
	return TimeBetweenProjectiles;
}

float ASpell::GetStaminaCost() const
{
	return SpellStaminaCost;
}

int32 ASpell::GetNumberOfProjectilesPerSpell() const
{
	return NumberOfProjectilesPerSpell;
}

FString ASpell::GetSpellType() 
{
	return SpellType;
}

TMap<FString, FText> ASpell::GetSpellDescription() const
{
	TMap<FString, FText> Descriptions;
	Descriptions.Add(TEXT("Weak"), SpellWeakDescription);
	Descriptions.Add(TEXT("Strong"), SpellStrongDescription);
	Descriptions.Add(TEXT("Secundary"), SpellSecundaryDescription);

	return Descriptions;
}

int32 ASpell::GetSpellLevel() const
{
	return SpellLevel;
}

float ASpell::GetSpellDamage() 
{
	return Damage;
}

ASpirit* ASpell::GetOwnerSpirit() 
{
	if(OwnerPawn)
	{
		return Cast<ASpirit>(OwnerPawn);
	}
	
	return nullptr;
}

FVector ASpell::GetPredictedPositionToAttack() 
{
	if(Target)
	{
		FVector TargetPosition = Target->GetActorLocation();
		FVector FiringPosition = GetActorLocation();
		FVector TargetVelocity = Target->GetVelocity();

		FVector DistanceVector = TargetPosition - FiringPosition;
		float Distance = DistanceVector.Size();
		float Time = Distance/SpellSpeed;

		FVector PredictedPosition = TargetPosition + (TargetVelocity * Time);

		return PredictedPosition;
	}
	
	return FVector();
}
