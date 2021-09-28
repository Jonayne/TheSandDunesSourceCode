// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpiritState.generated.h"

UCLASS()
class THESANDDUNES_API ASpiritState : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpiritState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	FString StateName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	class UNiagaraComponent* StateNiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	class UParticleSystemComponent* StateParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	class UAudioComponent* StateAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	float StateDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	float StateProbability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	float StateCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	bool bIsEffectPerTick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	float StatePerTickTimer;

	class ASpirit* Spirit;

	class UStatesComponent* StateComponentOwner;

	FTimerHandle StopEffectTimer;
	FTimerHandle EffectPerTickTimer;
	FTimerHandle DestroyStateTimer;

	bool bIsABuff;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual bool ApplyStateEffect();

	UFUNCTION(BlueprintImplementableEvent, Category="State")
	void ShowStatusEffectWidget(const FString & StateNameToShow, bool bIsBuff, FVector SpiritLocation);

	virtual void RemoveStateEffect();

	virtual void ApplyTimerPerTick();

	bool WillApply();

	void DestroySpiritState();

	void SetStateComponentOwner(UStatesComponent* StateComponentOwnerToPut);

	float GetStateCooldown() const;

	FString GetTypeOfState();
};
