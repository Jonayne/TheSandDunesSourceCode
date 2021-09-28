// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "ChaserGhost.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API AChaserGhost : public AEnemy
{
	GENERATED_BODY()
	
public:
	AChaserGhost();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ghost")
	class UAudioComponent* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ghost")
	class UNiagaraComponent* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell Explosion")
	class USphereComponent* ExplosionSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell Explosion")
	class UCapsuleComponent* MeleeCapsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Jumper")
	class UAudioComponent* ActionSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ghost")
	int32 TimesToExploteBeforeDeath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ghost")
	float TimeToExplote;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ghost")
	float ExplosionDamage;

	FTimerHandle TimerToExploteHandler;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual float SpiritTakeDamage(float DamageAmount, FString TypeOfDamage, AActor* DamageCauser, FVector DamageLocation, bool bIsABeam) override;

	void Explote();

	virtual void OnHaveSeenAnEnemy() override;

private:
	int32 TimesExploted;
	bool bCanExplote;
};
