// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "StatesComponent.h"
#include "SpiritState.h"
#include "Spirit.h"

// Sets default values for this component's properties
UStatesComponent::UStatesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Lista de Debuffos.
	SpiritStates.Add(TEXT("RayosDebuf"), TSubclassOf<ASpiritState>()); //Stun, rayo
	SpiritStates.Add(TEXT("NaturalezaDebuf"), TSubclassOf<ASpiritState>()); //Slow, naturaleza
	SpiritStates.Add(TEXT("SombrasDebuf"), TSubclassOf<ASpiritState>()); //Dark, sombras
	SpiritStates.Add(TEXT("SagradoDebuf"), TSubclassOf<ASpiritState>()); //Dark, sombras
	SpiritStatesApplied.Add(TEXT("RayosDebuf"), false);
	SpiritStatesApplied.Add(TEXT("NaturalezaDebuf"), false);
	SpiritStatesApplied.Add(TEXT("SombrasDebuf"), false);
	SpiritStatesApplied.Add(TEXT("SagradoDebuf"), false);

	// Lista de Buffs.
	SpiritStates.Add(TEXT("ArcanosBuff"), TSubclassOf<ASpiritState>());
	SpiritStates.Add(TEXT("SagradoBuff"), TSubclassOf<ASpiritState>());
	SpiritStates.Add(TEXT("AguaBuff"), TSubclassOf<ASpiritState>());
	SpiritStates.Add(TEXT("RayosBuff"), TSubclassOf<ASpiritState>());
    SpiritStates.Add(TEXT("NaturalezaBuff"), TSubclassOf<ASpiritState>());
    SpiritStates.Add(TEXT("SombrasBuff"), TSubclassOf<ASpiritState>());
	SpiritStatesApplied.Add(TEXT("ArcanosBuff"), false);
	SpiritStatesApplied.Add(TEXT("SagradoBuff"), false);
	SpiritStatesApplied.Add(TEXT("AguaBuff"), false);
	SpiritStatesApplied.Add(TEXT("RayosBuff"), false);
    SpiritStatesApplied.Add(TEXT("NaturalezaBuff"), false);
	SpiritStatesApplied.Add(TEXT("SombrasBuff"), false);
}


// Called when the game starts
void UStatesComponent::BeginPlay()
{
	Super::BeginPlay();

	SpiritOwner = Cast<ASpirit>(GetOwner());
}


// Called every frame
void UStatesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UStatesComponent::ApplyEffect(FString TypeOfState, bool bIsDebuf) 
{
	FString StateName = bIsDebuf ? TypeOfState.Append("Debuf") : TypeOfState.Append("Buff");

	// Revisamos que el estado exista, que tenga una clase valida (si no la tiene es porque el actor en cuestion
	// es invulnerable o porque se me olvido crear el blueprint xD) y que no lo tenga ya aplicado
	if(SpiritStates.Contains(StateName) && SpiritStates[StateName] && !SpiritStatesApplied[StateName])
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = SpiritOwner;

		ASpiritState* SpiritStateToApply = GetWorld()->SpawnActor<ASpiritState>(SpiritStates[StateName], SpawnParams);
		SpiritStateToApply->AttachToActor(SpiritOwner, FAttachmentTransformRules::KeepRelativeTransform);
		SpiritStateToApply->SetStateComponentOwner(this);

		if(SpiritStateToApply && SpiritStateToApply->ApplyStateEffect())
		{
			ChangeStateStatus(StateName, true);
			return SpiritStateToApply->GetStateCooldown();
		}
		else if (SpiritStateToApply)
		{
			SpiritStateToApply->Destroy();
		}
		
	}

	return 0.f;
}

void UStatesComponent::ChangeStateStatus(FString StateName, bool bApplied) 
{
	if(SpiritStatesApplied.Contains(StateName))
	{
		// FString Change = bApplied ? TEXT("TRUE") : TEXT("FALSE");
		// UE_LOG(LogTemp, Warning, TEXT("Se cambio el estado %s a %s"), *StateName, *Change);
		SpiritStatesApplied[StateName] = bApplied;
	}
}

float UStatesComponent::GetBuffCooldown(FString StateName) 
{
	FString BuffName = StateName.Append("Buff");

	if(SpiritStates.Contains(BuffName) && SpiritStates[BuffName])
	{
		const ASpiritState* SpiritStateDefault = GetDefault<ASpiritState>(SpiritStates[BuffName]);
		return SpiritStateDefault->GetStateCooldown();
	}	

	return 0.f;
}

bool UStatesComponent::IsInState(FString TypeOfState, bool bIsDebuf) 
{
	FString StateName = bIsDebuf ? TypeOfState.Append("Debuf") : TypeOfState.Append("Buff");

	if(SpiritStatesApplied.Contains(StateName))
	{
		return SpiritStatesApplied[StateName];
	}

	return false;
}

TMap<FString, TSubclassOf<ASpiritState>> UStatesComponent::GetSpiritStates() 
{
	return SpiritStates;
}

TSubclassOf<ASpiritState> UStatesComponent::GetSpiritStateClass(FString StateName) 
{
	FString StateFinalName = StateName.Append("Buff");

	if(SpiritStates.Contains(StateFinalName))
	{
		return SpiritStates[StateFinalName];
	}

	return TSubclassOf<ASpiritState>();
}

void UStatesComponent::SetSpiritState(FString StateName, TSubclassOf<ASpiritState> NewState) 
{
	FString StateFinalName = StateName.Append("Buff");

	if(SpiritStates.Contains(StateFinalName))
	{
		SpiritStates.Remove(StateFinalName);
	}

	SpiritStates.Add(StateFinalName, NewState);	
}

void UStatesComponent::SetStatesList(TMap<FString, TSubclassOf<class ASpiritState>> NewList) 
{
	SpiritStates = NewList;
}
