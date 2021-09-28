// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealingArea.generated.h"

UCLASS()
class THESANDDUNES_API AHealingArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealingArea();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealingArea")
	class USphereComponent* SphereArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingArea")
	class UNiagaraComponent* HealingAreaNiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingArea")
	class UParticleSystemComponent* HealingAreaParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingArea")
	class UAudioComponent* HealingAreaAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingArea")
	FString HealingTypeDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingArea")
	float HealingAreaDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingArea")
	float WaitBeforeDestroy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingArea")
	float HealQuantity;

	FTimerHandle HealingAreaTimerHandler;
	FTimerHandle HealTickTimerHandler;
	FTimerHandle HealingAreaDestroyTimerHandler;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyHealingInArea();

	void StopHealingArea();

	void DestroyHealingArea();

private:
	class ASpirit* SpiritOwner;
};
