// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpellsComponent.h"
#include "Spell.h"
#include "AOE_Spell.h"
#include "Beam_Spell.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Diana.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
USpellsComponent::USpellsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USpellsComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = GetOwnerCharacter();

	if(OwnerCharacter && OwnerCharacter->IsA(ADiana::StaticClass()))
	{
		bIsDianaOwner = true;
	}

	StartSpellsComponent();

	if(bIsDianaOwner)
	{
		if(!bIsStrongSpellComponent)
		{
			SpellsSecundaryTimers.Add(TEXT("Arcanos"), GetWorld()->GetTimerManager().GenerateHandle(0));
			SpellsSecundaryTimers.Add(TEXT("Agua"), GetWorld()->GetTimerManager().GenerateHandle(1));
			SpellsSecundaryTimers.Add(TEXT("Rayos"), GetWorld()->GetTimerManager().GenerateHandle(2));
			SpellsSecundaryTimers.Add(TEXT("Naturaleza"), GetWorld()->GetTimerManager().GenerateHandle(3));
			SpellsSecundaryTimers.Add(TEXT("Sombras"), GetWorld()->GetTimerManager().GenerateHandle(4));
			SpellsSecundaryTimers.Add(TEXT("Sagrado"), GetWorld()->GetTimerManager().GenerateHandle(5));
		}

		SpellsTimers.Add(TEXT("Arcanos"), GetWorld()->GetTimerManager().GenerateHandle(6));
		SpellsTimers.Add(TEXT("Agua"), GetWorld()->GetTimerManager().GenerateHandle(7));
		SpellsTimers.Add(TEXT("Rayos"), GetWorld()->GetTimerManager().GenerateHandle(8));
		SpellsTimers.Add(TEXT("Naturaleza"), GetWorld()->GetTimerManager().GenerateHandle(9));
		SpellsTimers.Add(TEXT("Sombras"), GetWorld()->GetTimerManager().GenerateHandle(10));
		SpellsTimers.Add(TEXT("Sagrado"), GetWorld()->GetTimerManager().GenerateHandle(11));
	}
}

void USpellsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpellsComponent::ActivateSecundarySpell() 
{
	if(Spells.Num() > 0 && SpellsSecundaryTimers.Contains(CurrentSpellType) && Spells.Contains(CurrentSpellType) && OwnerCharacter)
	{
		float TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(SpellsSecundaryTimers[CurrentSpellType]);
		if(TimeRemaining <= 0.f)
		{
			float TimeToWait = OwnerCharacter->SetSpiritState(CurrentSpellType, false);
			GetWorld()->GetTimerManager().SetTimer(SpellsSecundaryTimers[CurrentSpellType], TimeToWait, false);
		}
	}
}

void USpellsComponent::ActivateSecundarySpell(FString SpecificSpellType) 
{
	if(Spells.Num() > 0 && SpellsSecundaryTimers.Contains(SpecificSpellType) && OwnerCharacter)
	{
		float TimeToWait = OwnerCharacter->SetSpiritState(SpecificSpellType, false);
		GetWorld()->GetTimerManager().SetTimer(SpellsSecundaryTimers[SpecificSpellType], TimeToWait, false);
	}
}

void USpellsComponent::ActivateSpell(FName SocketName)
{
	if(Spells.Num() > 0)
	{
		int32 SpellIndex = FMath::RandRange(0, Spells.Num()-1);
		TArray<FString> SpellsList;
		Spells.GenerateKeyArray(SpellsList);
		
		SetCurrentSpellByType(SpellsList[SpellIndex]);
		SocketNameForSpawn = SocketName;
		CastSpell();
	}
}

void USpellsComponent::ActivateSpell(FName SocketName, FString SpellType) 
{	
	if(Spells.Num() > 0)
	{
		SetCurrentSpellByType(SpellType);
		SocketNameForSpawn = SocketName;
		CastSpell();	
	}
}

void USpellsComponent::ActivateCurrentSpell(FName SocketName) 
{
	if(Spells.Num() > 0)
	{
		SocketNameForSpawn = SocketName;
		CastSpell();
	}
}

void USpellsComponent::SpawnSpell() 
{
	if (OwnerCharacter && CurrentProjectileSpawned < NumberOfProjectiles && Spells.Contains(CurrentSpellType))
	{
		CurrentProjectileSpawned += 1;

		FVector SpawnLocation;
		FRotator SpawnRotator;

		// Si tenemos que spawnear un Spell en un socket especifico, o no.
		if(SocketNameForSpawn.IsNone())
		{
			SpawnLocation = OwnerCharacter->GetActorLocation();
			SpawnRotator = OwnerCharacter->GetActorRotation();

			if(!bIsAOE && !bIsBeam)
			{
				SpawnLocation.Z += 40.f;
			}
		}
		else
		{
			SpawnLocation = OwnerCharacter->GetMesh()->GetSocketLocation(SocketNameForSpawn);
			SpawnRotator = OwnerCharacter->GetMesh()->GetSocketRotation(SocketNameForSpawn);
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerCharacter;
		
		if(bIsBeam)
		{
			ASpell* SpawnedActor = GetWorld()->SpawnActor<ASpell>(Spells[CurrentSpellType], SpawnParams);
			SpawnedActor->AttachToActor(OwnerCharacter, FAttachmentTransformRules::KeepRelativeTransform);
		}
		else
		{
			ASpell* SpawnedActor = GetWorld()->SpawnActor<ASpell>(Spells[CurrentSpellType], SpawnLocation, SpawnRotator, SpawnParams);
			OwnerCharacter->GetCapsuleComponent()->IgnoreActorWhenMoving(SpawnedActor, true);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimeBetweenSpellsHandler);
	}
}

void USpellsComponent::ReleaseSpell() 
{
	CurrentProjectileSpawned = 0;
	SpawnSpell();
	if(NumberOfProjectiles > 1)
	{
		GetWorld()->GetTimerManager().SetTimer(TimeBetweenSpellsHandler, this, &USpellsComponent::SpawnSpell, TimeBetweenSpellProjectiles, true); 
	}
}

void USpellsComponent::CastSpell() 
{
	if (OwnerCharacter && Spells.Contains(CurrentSpellType))
	{	
		if(bIsDianaOwner)
		{
			if(SpellsTimers.Contains(CurrentSpellType))
			{
				float TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(SpellsTimers[CurrentSpellType]);
				if(TimeRemaining <= 0.f)
				{
					ReleaseSpell();
					GetWorld()->GetTimerManager().SetTimer(SpellsTimers[CurrentSpellType], SpellCooldown, false);
				}
			}
		}
		else
		{
			ReleaseSpell();
		}
	}
}

float USpellsComponent::GetSpellPercentage() const
{
	if(SpellsTimers.Contains(CurrentSpellType) && SpellCooldown)
	{
		if(GetWorld()->GetTimerManager().IsTimerActive(SpellsTimers[CurrentSpellType]))
		{
			float TimeElapsed = GetWorld()->GetTimerManager().GetTimerElapsed(SpellsTimers[CurrentSpellType]);
			return TimeElapsed/SpellCooldown;
		}
	}
		
	return 1.f;
}

float USpellsComponent::GetSecundarySpellPercentage() const
{
	if(SpellsSecundaryTimers.Contains(CurrentSpellType) && SecundarySpellCooldown)
	{
		if(GetWorld()->GetTimerManager().IsTimerActive(SpellsSecundaryTimers[CurrentSpellType]))
		{
			float TimeElapsed = GetWorld()->GetTimerManager().GetTimerElapsed(SpellsSecundaryTimers[CurrentSpellType]);

			return TimeElapsed/SecundarySpellCooldown;
		}
	}
		
	return 1.f;
}

float USpellsComponent::GetActiveSpellPercentage(FString SpellType) const
{
	if(SpellsSecundaryTimers.Contains(SpellType) && OwnerCharacter)
	{
		if(GetWorld()->GetTimerManager().IsTimerActive(SpellsSecundaryTimers[SpellType]))
		{
			float TimeElapsed = GetWorld()->GetTimerManager().GetTimerElapsed(SpellsSecundaryTimers[SpellType]);

			return TimeElapsed/OwnerCharacter->GetBuffCooldown(SpellType);
		}
	}
		
	return 1.f;
}

float USpellsComponent::GetSpellPercentage(FString SpellType) const
{
	if(SpellsTimers.Contains(SpellType))
	{
		if(GetWorld()->GetTimerManager().IsTimerActive(SpellsTimers[SpellType]))
		{
			float TimeElapsed = GetWorld()->GetTimerManager().GetTimerElapsed(SpellsTimers[SpellType]);

			return TimeElapsed/GetSpellCooldownByType(SpellType);
		}
	}
		
	return 1.f;
}

float USpellsComponent::GetSpellCooldownByType(FString SpellType) const
{
	if(Spells.Contains(SpellType))
	{
		const ASpell* SpellToCast = GetDefault<ASpell>(Spells[SpellType]);
		return SpellToCast->GetSpellCooldown();
	}
	
	return 1.f;
}

void USpellsComponent::SetCurrentSpellByType(FString SpellType) 
{
	if(!OwnerCharacter) OwnerCharacter = GetOwnerCharacter();
	
	if(Spells.Contains(SpellType) && !CurrentSpellType.Equals(SpellType) && OwnerCharacter)
	{
		CurrentSpellType = SpellType;

		UpdateCurrentSpellValues(Spells[CurrentSpellType]);
	}
}

TMap<FString, FText> USpellsComponent::GetSpellDescription(FString SpellType) 
{
	if (Spells.Contains(SpellType))
	{
		TSubclassOf<ASpell> ClassToSpawn = Spells[SpellType];
		const ASpell* SpellToCast = GetDefault<ASpell>(ClassToSpawn);

		return SpellToCast->GetSpellDescription();
	}

	return TMap<FString, FText>();
}

void USpellsComponent::UpdateCurrentSpellValues(TSubclassOf<ASpell> ClassToCaptureValues) 
{
	if(ClassToCaptureValues)
	{
		const ASpell* SpellToCast = GetDefault<ASpell>(ClassToCaptureValues);

		SpellCooldown = SpellToCast->GetSpellCooldown();
		bIsAOE = SpellToCast->IsA(AAOE_Spell::StaticClass());
		bIsBeam = SpellToCast->IsA(ABeam_Spell::StaticClass());
		NumberOfProjectiles = SpellToCast->GetNumberOfProjectilesPerSpell();
		TimeBetweenSpellProjectiles = SpellToCast->GetTimeBetweenProjectiles();
		StaminaCost = SpellToCast->GetStaminaCost();
		SecundarySpellCooldown = OwnerCharacter->GetBuffCooldown(CurrentSpellType);
	}
}

int32 USpellsComponent::GetSpellTypeLevel(FString SpellType) 
{
	if (Spells.Contains(SpellType))
	{
		TSubclassOf<ASpell> ClassToSpawn = Spells[SpellType];
		const ASpell* SpellToCast = GetDefault<ASpell>(ClassToSpawn);

		return SpellToCast->GetSpellLevel();
	}

	return 0;
}

FString USpellsComponent::GetCurrentSpellType() 
{
	return CurrentSpellType;
}

TArray<FString> USpellsComponent::GetSpells() 
{
	TArray<FString> SpellsNames;
	Spells.GenerateKeyArray(SpellsNames);
	return SpellsNames;
}

float USpellsComponent::GetCurrentSpellStaminaCost() 
{
	return StaminaCost;
}

void USpellsComponent::SetSpellComponentAsStrong() 
{
	bIsStrongSpellComponent = true;
}

void USpellsComponent::SetSpellComponentAsWeak() 
{
	bIsStrongSpellComponent = false;
}

float USpellsComponent::GetSpellCD() 
{
	return SpellCooldown;
}

bool USpellsComponent::ValidSpell(FString SpellType) 
{
	return Spells.Contains(SpellType);
}

ASpirit* USpellsComponent::GetOwnerCharacter() const
{
	AActor* ActorOwner = GetOwner();
	if(ActorOwner) return Cast<ASpirit>(ActorOwner);

	return nullptr;
}

bool USpellsComponent::CanShotSpell() 
{
	return GetSpellPercentage() == 1.f;
}

TMap<FString, TSubclassOf<ASpell>> USpellsComponent::GetSpellsList() 
{
	return Spells;
}

void USpellsComponent::SetSpellsList(TMap<FString, TSubclassOf<ASpell>> NewSpellList) 
{
	Spells = NewSpellList;
}

void USpellsComponent::StartSpellsComponent() 
{
	if(Spells.Num() > 0 && !Spells.Contains(CurrentSpellType))
	{	
		TArray<FString> SpellsNames;
		Spells.GenerateKeyArray(SpellsNames);
		CurrentSpellType = SpellsNames[0];
	}
	
	if (OwnerCharacter && Spells.Contains(CurrentSpellType))
	{
		UpdateCurrentSpellValues(Spells[CurrentSpellType]);
	}
}

void USpellsComponent::AddSpellToComponent(FString NewSpellType, TSubclassOf<ASpell> NewSpellClass) 
{
	if(Spells.Contains(NewSpellType))
	{
		Spells.Remove(NewSpellType);
	}

	Spells.Add(NewSpellType, NewSpellClass);
	
	if(CurrentSpellType.Equals(NewSpellType))
	{
		UpdateCurrentSpellValues(Spells[NewSpellType]);
	}
}
