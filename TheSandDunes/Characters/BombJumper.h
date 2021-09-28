// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Jumper.h"
#include "BombJumper.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ABombJumper : public AJumper
{
	GENERATED_BODY()
	
public:
	ABombJumper();
	
	// Util para zonas planas. Si no, desactivarlo.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BombJumper")
	bool bShouldPredictJump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BombJumper")
	class USphereComponent* ActionSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell Action")
	USphereComponent* ActionCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Landed(const FHitResult& Hit) override;

public:

	virtual void MakeJump() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	virtual void JumperAction() override;

	virtual void KillThisSpirit() override;

	virtual FVector CalculateJumpVelocity() override;

private:
	bool HaveExploted;

	void Explote();
	
	FVector GetPredictedTargetLocation();
};
