// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Dasher.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ADasher : public AEnemy
{
	GENERATED_BODY()
	
public:
	ADasher();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dasher")
	float DashDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dasher")
	float TimeBeforeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dasher")
	float TimeBefweenDashes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Auto AIM")
	class UCapsuleComponent* TouchCapsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dasher")
	class UArrowComponent* DashDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dasher")
	class UAudioComponent* DashSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dasher")
	class UAudioComponent* ActionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dasher")
	class UNiagaraComponent* ActionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dasher")
	class UParticleSystemComponent* ActionParticleSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dasher")
	class UNiagaraComponent* DashEffect;
	
	FTimerHandle TimerToDash;
	FTimerHandle TimerToAction;
	FTimerHandle DashDurationTimer;

	bool bExecutedOnce;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float TimeBeforeDashAux;

public:

	UFUNCTION(BlueprintCallable, Category="Dasher")
	virtual void MakeDash();

	UFUNCTION(BlueprintCallable, Category="Dasher")
	virtual void DasherAction();

	virtual void KillThisSpirit() override;
	
	virtual void OnHaveSeenAnEnemy() override;
	
	void StopDash();
	
	UFUNCTION()
	virtual void OnTouchedAnEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual FVector CalculateDashVelocity();
};
