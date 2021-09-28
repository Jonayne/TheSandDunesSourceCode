// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "DianaSpell_NaturalezaWeak.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ADianaSpell_NaturalezaWeak : public ASpell
{
	GENERATED_BODY()

public:
	ADianaSpell_NaturalezaWeak();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Naturaleza Spell")
	float ThrustMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Naturaleza Spell")
	float DamageMinimum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Naturaleza Spell")
	float VelocityToReduceDamage;

public:	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void PlayHitEffects() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class ADiana* Diana;
	
	void ReduceDamage(float DeltaTime);
};
