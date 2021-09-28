// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Jumper.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API AJumper : public AEnemy
{
	GENERATED_BODY()
	
public:
	AJumper();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Jumper")
	float JumpDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Jumper")
	float TimeBetweenTryingJumps;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Jumper")
	float TimeBeforeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Jumper")
	class UAudioComponent* JumpSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Jumper")
	class UAudioComponent* ActionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Jumper")
	class UNiagaraComponent* ActionEffect;
	
	FTimerHandle TimerToJump;
	FTimerHandle TimerToAction;

	bool bExecutedOnce;
	bool HaveLanded;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Landed(const FHitResult& Hit) override;

public:

	UFUNCTION(BlueprintCallable, Category="Jumper")
	virtual void MakeJump();

	UFUNCTION(BlueprintCallable, Category="Jumper")
	virtual void JumperAction();

	virtual void KillThisSpirit() override;
	
	virtual void OnHaveSeenAnEnemy() override;
	
	virtual FVector CalculateJumpVelocity();
};
