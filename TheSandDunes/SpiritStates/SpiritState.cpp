// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpiritState.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "NiagaraComponent.h"
#include "Diana.h"
#include "StatesComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpiritState::ASpiritState()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StateAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("StateAudio"));
	RootComponent = StateAudioComponent;

	StateParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("StateParticle"));
	StateParticle->SetupAttachment(GetRootComponent());

	StateNiagaraParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("StateNiagaraParticle"));
	StateNiagaraParticle->SetupAttachment(GetRootComponent());

	StatePerTickTimer = .5f;
	bIsEffectPerTick = false;
}

// Called when the game starts or when spawned
void ASpiritState::BeginPlay()
{
	Super::BeginPlay();
	
	Spirit = Cast<ASpirit>(GetOwner());
}

// Called every frame
void ASpiritState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASpiritState::ApplyStateEffect() 
{
	if(Spirit && WillApply() && StateComponentOwner)
    {
		if(!bIsABuff)
		{
			ShowStatusEffectWidget(StateName, bIsABuff, Spirit->GetMesh()->GetSocketLocation(FName("HeadTopSocket")));
		}
        
        GetWorldTimerManager().SetTimer(StopEffectTimer, this, &ASpiritState::RemoveStateEffect, StateDuration, false);

		if(bIsEffectPerTick)
		{
			GetWorldTimerManager().SetTimer(EffectPerTickTimer, this, &ASpiritState::ApplyTimerPerTick, StatePerTickTimer, true);
		}
		return true;
    }
	
	return false;
}

void ASpiritState::RemoveStateEffect() 
{	
	if(StateComponentOwner && Spirit)
	{
		StateComponentOwner->ChangeStateStatus(StateName, false);
		GetWorldTimerManager().ClearTimer(EffectPerTickTimer);
		GetWorldTimerManager().ClearTimer(StopEffectTimer);
		if(StateParticle) StateParticle->Deactivate();
		if(StateAudioComponent) StateAudioComponent->Deactivate();
		if(StateNiagaraParticle) StateNiagaraParticle->Deactivate();
	}
}

void ASpiritState::ApplyTimerPerTick() 
{	
}

bool ASpiritState::WillApply() 
{
	if(!bIsABuff)
	{	
		FString StateType = GetTypeOfState();
		float FinalProbability = 1.f;

		if(StateType.Equals("Rayos") || StateType.Equals("Naturaleza"))
		{
			ADiana* Diana = Cast<ADiana>(Spirit);

			if(Diana && Diana->bSlippingDiana)
			{
				return false;
			}
			else
			{
				FinalProbability = StateProbability - (StateProbability * Spirit->GetResistStat(StateType));
			}
		}
		else
		{
			FinalProbability = StateProbability - (StateProbability * Spirit->GetResistStat(StateType));
		}
		
		return UKismetMathLibrary::RandomBoolWithWeight(FinalProbability);
	}
	
	return true;
}

void ASpiritState::DestroySpiritState() 
{
	Destroy();
}

void ASpiritState::SetStateComponentOwner(UStatesComponent* StateComponentOwnerToPut) 
{
	StateComponentOwner = StateComponentOwnerToPut;
}

float ASpiritState::GetStateCooldown() const
{
	return StateCooldown + StateDuration;
}

FString ASpiritState::GetTypeOfState() 
{
	FString Empty = TEXT("");
	FString Buff = TEXT("Buff");
	FString Debuff = TEXT("Debuf");

	FString StateType = StateName.Replace(*Buff, *Empty);
	return StateType.Replace(*Debuff, *Empty);
}
