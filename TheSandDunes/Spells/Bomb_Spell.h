// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "Bomb_Spell.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ABomb_Spell : public ASpell
{
	GENERATED_BODY()

public:

	ABomb_Spell();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Explosion")
	float TimeToWaitForExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Explosion")
	float ExplosionTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Explosion")
	class USphereComponent* ExplosionSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Explosion")
	class UParticleSystemComponent* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Explosion")
	class UNiagaraComponent* ExplosionNiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Explosion")
	class UAudioComponent* ExplosionAudio;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	void TurnOffSpell();

	virtual void PlayHitEffects() override;

	FTimerHandle SpawnExplosionTimerHandle;
	FTimerHandle ExplosionTimerHandle;

	bool bAlreadyExploded;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
