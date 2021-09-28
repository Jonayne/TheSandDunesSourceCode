// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Spirit.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "SpellsComponent.h"
#include "Spell.h"
#include "StatesComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"

// Sets default values
ASpirit::ASpirit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 200.f;
	Health = MaxHealth;
	SpiritMaxBuffSpeed = 1500.f;
	SpiritWalkSpeed = 1000.f;
	DodgeProbabilityIfState = 0.f;
	DamageMultiplier = 1.f;
	
	SpiritStateComponent = CreateDefaultSubobject<UStatesComponent>(FName("StateComponent"));
	SpiritStateComponent->bAutoActivate = true;

	SpiritResistances.Add(TEXT("Arcanos"), 0.f);
	SpiritResistances.Add(TEXT("Agua"), 0.f);
	SpiritResistances.Add(TEXT("Rayos"), 0.f);
	SpiritResistances.Add(TEXT("Sagrado"), 0.f);
	SpiritResistances.Add(TEXT("Sombras"), 0.f);

	Tags.Add(FName("Spirit"));

	WaitBeforeDestroy = 5.f;

	SpellComponent = CreateDefaultSubobject<USpellsComponent>(FName("SpellComponent"));
	SpellComponent->bAutoActivate = true;

	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimulusSource"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Damage>());
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Hearing>());
	Stimulus->RegisterWithPerceptionSystem();

	StunnedTimeIfNotMontage = 2.5f;

	bSpellsCanMoveActor = true;
	SpiritMass = .2f;
}

// Called when the game starts or when spawned
void ASpirit::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpirit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpirit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ASpirit::SpiritTakeDamage(float DamageAmount, FString TypeOfDamage, AActor* DamageCauser, FVector DamageLocation, bool bIsABeam) 
{
	bool bCouldHit = !(UKismetMathLibrary::RandomBoolWithWeight(DodgeProbabilityIfState) || bInvincible);

	if(bCouldHit)
	{
		SetSpiritState(TypeOfDamage, true);
	}

	UAISense_Damage::ReportDamageEvent(this, this, DamageCauser, DamageAmount, DamageLocation, DamageLocation);
	UAISense_Hearing::ReportNoiseEvent(this, DamageLocation, 1.f, DamageCauser, 5000.f, FName("HearEvent"));

	if(!bCouldHit)
	{
		ShowDamage(0.f, TypeOfDamage, DamageLocation, bIsABeam);

		return 0.f;
	}
	
	float BaseDamagePlusSpellDamage = DamageAmount;
	ASpirit* DamageCauserSpirit = Cast<ASpirit>(DamageCauser);
	if(DamageCauserSpirit)
	{
		if(bIsABeam)
		{
			BaseDamagePlusSpellDamage = (DamageCauserSpirit->GetBaseDamage()*.03f + DamageAmount)*DamageMultiplier;
		}
		else
		{
			BaseDamagePlusSpellDamage = ((DamageCauserSpirit->GetBaseDamage()*FMath::FRandRange(.6f, 1.f)) + DamageAmount)*DamageMultiplier;
		}
		
		BaseDamagePlusSpellDamage = FMath::Max(0.f, BaseDamagePlusSpellDamage);
	}
	
	float DamageToApply = CalculateDamageRecieved(BaseDamagePlusSpellDamage, TypeOfDamage);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	ShowDamage(DamageToApply, TypeOfDamage, DamageLocation, bIsABeam);

	if (IsDead())
	{
		KillThisSpirit();
	}

	return DamageToApply;

}

UAnimMontage* ASpirit::GetSpiritMontage() 
{
	return SpiritAnimationMontage;
}

float ASpirit::CalculateDamageRecieved(float DamageAmount, FString TypeOfDamage) 
{
	if(SpiritResistances.Contains(TypeOfDamage))
	{
		float ResistanceToThisDamage = SpiritResistances[TypeOfDamage];
		return DamageAmount - (DamageAmount * ResistanceToThisDamage);
	}

	return DamageAmount;
}

float ASpirit::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) 
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return DamageToApply;
}

float ASpirit::PlayMontageByName(FName ActionName, UAnimMontage* AnimationMontage, float AnimationSpeed=1.f) 
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	float AnimationRemainingTime = 0.f;

	if (AnimInstance && AnimationMontage)
	{
		AnimInstance->Montage_Play(AnimationMontage, AnimationSpeed);
		AnimInstance->Montage_Pause(AnimationMontage);
		AnimInstance->Montage_JumpToSectionsEnd(ActionName, AnimationMontage);
		float EndOfAnimation = AnimInstance->Montage_GetPosition(AnimationMontage);
		AnimInstance->Montage_JumpToSection(ActionName, AnimationMontage);
		AnimationRemainingTime = EndOfAnimation - AnimInstance->Montage_GetPosition(AnimationMontage);
		AnimInstance->Montage_Resume(AnimationMontage);
	}

	return AnimationRemainingTime;
}

void ASpirit::AddSpell(FString NewSpellType, TSubclassOf<ASpell> NewSpellClass) 
{
	SpellComponent->AddSpellToComponent(NewSpellType, NewSpellClass);
}

void ASpirit::ModifyHealth(float Heal) 
{
	float NextHealth = FMath::Min(MaxHealth, Health+Heal);
	NextHealth = FMath::Max(1.f, NextHealth);

	Health = NextHealth;
}

int32 ASpirit::GetBaseDamage() 
{
	return BaseDamage;
}

void ASpirit::SetBaseDamage(float NewBaseDamage) 
{
	BaseDamage = NewBaseDamage;
}

void ASpirit::InvokeSpell(FString SpellType, FName SocketName) 
{
	if(!bStunned)
	{
		SpellComponent->ActivateSpell(SocketName, SpellType);
	}
}

void ASpirit::InvokeRandomSpell(FName SocketName) 
{
	if(!bStunned)
	{
		SpellComponent->ActivateSpell(SocketName);
	}
}

void ASpirit::InvokeCurrentSpell(FName SocketName) 
{
	if(!bStunned)
	{
		SpellComponent->ActivateCurrentSpell(SocketName);
	}
}

void ASpirit::ChangeCurrentSpell(FString SpellType) 
{
	if(!SpellType.Equals(SpellComponent->GetCurrentSpellType()))
	{
		SpellComponent->SetCurrentSpellByType(SpellType);
	} 
}

float ASpirit::StunSpirit() 
{
	float MontageTime = PlayMontageByName(FName("Stunned"), SpiritAnimationMontage, 1.f);
	bStunned = true;
	GetCharacterMovement()->StopMovementImmediately();

	if(MontageTime == 0.f)
	{
		GetWorldTimerManager().SetTimer(StunnedTimerHandler, this, &ASpirit::StopStun, StunnedTimeIfNotMontage, false);
		return StunnedTimeIfNotMontage;
	}
	else
	{
		GetWorldTimerManager().SetTimer(StunnedTimerHandler, this, &ASpirit::StopStun, MontageTime, false);
		return MontageTime;
	}
	
	return 0.f;
}

void ASpirit::StopStun() 
{
	GetWorldTimerManager().ClearTimer(StunnedTimerHandler);
	bStunned = false;
}

float ASpirit::GetSpellCastPercentageCompleted() 
{
	return SpellComponent->GetSpellPercentage();
}

void ASpirit::SetInvincibleMode(float Seconds) 
{
	bInvincible = true;
	GetWorldTimerManager().SetTimer(InvincibleStateHandler, this, &ASpirit::StopInvincibleMode, Seconds, false);
}

void ASpirit::StopInvincibleMode() 
{
	bInvincible = false;
	GetWorldTimerManager().ClearTimer(InvincibleStateHandler);
}

void ASpirit::SetTargetToHitLocation(FVector TargetLocation) 
{
	TargetToHitLocation = TargetLocation;
}

void ASpirit::SetTargetToHit(ASpirit* NewTargetToHit) 
{
	if(NewTargetToHit && !NewTargetToHit->IsDead())
	{
		TargetToHit = NewTargetToHit;
	}
	else
	{
		TargetToHit = nullptr;
	}
}

FVector ASpirit::GetTargetToHitLocation() 
{
	if(TargetToHit) return TargetToHit->GetActorLocation();

	return TargetToHitLocation;
}

ASpirit* ASpirit::GetTargetToHit() 
{
	return TargetToHit;
}

bool ASpirit::IsInAir() 
{
	return GetCharacterMovement()->IsFalling();
}

float ASpirit::GetSpiritSpeed() 
{
	return GetVelocity().Size2D();
}

void ASpirit::ModifyResistance(FString StatName, float ResistanceToModify, bool bModifyPermanently=false) 
{
	if(SpiritResistances.Contains(StatName))
	{
		float Resistance = SpiritResistances[StatName];
		float NextResistance = FMath::Min(1.f, Resistance+ResistanceToModify);
		
		SpiritResistances[StatName] = NextResistance;
	}
}

TMap<FString, float> ASpirit::GetSpiritResistances() 
{
	return SpiritResistances;
}

void ASpirit::SetStunned(bool NewState) 
{
	bStunned = NewState;
}

void ASpirit::SetResistance(FString StatName, float NewResistance) 
{
	if(SpiritResistances.Contains(StatName))
	{
		SpiritResistances[StatName] = NewResistance;
	}
}

float ASpirit::GetResistStat(FString StatName) 
{
	if(SpiritResistances.Contains(StatName))
	{
		return SpiritResistances[StatName];
	}

	return 0.f;
}

int32 ASpirit::GetSpellLevelByType(FString SpellType) 
{
	return SpellComponent->GetSpellTypeLevel(SpellType);
}

FString ASpirit::GetCurrentSpell() 
{
	FString Current = SpellComponent->GetCurrentSpellType();
	return Current;
}

void ASpirit::InvokeSecundarySpell() 
{
	SpellComponent->ActivateSecundarySpell();
}

void ASpirit::InvokeSpecificSecundarySpell(FString SpecificSpellType) 
{
	SpellComponent->ActivateSecundarySpell(SpecificSpellType);
}

void ASpirit::KillThisSpirit() 
{
	DetachFromControllerPendingDestroy();
	
	GetMesh()->bPauseAnims = true;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetWorldTimerManager().SetTimer(DestroyActorTimerHandler, this, &ASpirit::CallDestroyFunction, WaitBeforeDestroy, false);
}

float ASpirit::GetCurrentHealth() 
{
	if(MaxHealth) return Health/MaxHealth;
	
	return 0.f;
}

int32 ASpirit::GetMaxHealth() 
{
	return FMath::FloorToInt(MaxHealth);
}

int32 ASpirit::GetHealth() 
{
	return FMath::FloorToInt(Health);
}

void ASpirit::ChangeSpiritSpeed(float SpeedToChange) 
{
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed += SpeedToChange;
		GetCharacterMovement()->MaxFlySpeed += SpeedToChange;
	} 
}

void ASpirit::ChangeSpiritAcceleration(float AccelerationToChange) 
{
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->MaxAcceleration += AccelerationToChange;
	}
}

void ASpirit::ChangeSpiritGravity(float NewGravityScale) 
{
	if(GetCharacterMovement()) GetCharacterMovement()->GravityScale += NewGravityScale;
}

bool ASpirit::IsInState(FString StateName, bool bIsDebuf) 
{
	return SpiritStateComponent->IsInState(StateName, bIsDebuf);
}

float ASpirit::SetSpiritState(FString StateName, bool bIsDebuf) 
{
	return SpiritStateComponent->ApplyEffect(StateName, bIsDebuf);
}

float ASpirit::GetBuffCooldown(FString StateName) 
{
	return SpiritStateComponent->GetBuffCooldown(StateName);
}

void ASpirit::CallDestroyFunction() 
{
	Destroy();
}
