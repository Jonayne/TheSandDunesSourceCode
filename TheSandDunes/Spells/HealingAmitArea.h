// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealingAmitArea.generated.h"

UCLASS()
class THESANDDUNES_API AHealingAmitArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealingAmitArea();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealingAmitArea")
	class USphereComponent* SphereArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingAmitArea")
	class UNiagaraComponent* HealingAmitAreaNiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingAmitArea")
	class UParticleSystemComponent* HealingAmitAreaParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingAmitArea")
	class UAudioComponent* HealingAmitAreaAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingAmitArea")
	FString HealingTypeDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingAmitArea")
	float HealingAmitAreaDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingAmitArea")
	float WaitBeforeDestroy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealingAmitArea")
	float HealQuantity;

	FTimerHandle HealingAmitAreaTimerHandler;
	FTimerHandle HealTickTimerHandler;
	FTimerHandle HealingAmitAreaDestroyTimerHandler;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyHealingInArea();

	void StopHealingAmitArea();

	void DestroyHealingAmitArea();

private:
	class ASpirit* SpiritOwner;
};
