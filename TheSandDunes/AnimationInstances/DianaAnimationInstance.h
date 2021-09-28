// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DianaAnimationInstance.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API UDianaAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category="AnimationProperties")
	void UpdateAnimationVariables();

	// Para saber qué particulas spawnear segun el tipo de material que estemos pisando.
	UFUNCTION(BlueprintCallable, Category="Footsteps")
	void HandleDianaFootsteps(FName FootSocket);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Locomotion")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Locomotion")
	bool bInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Locomotion")
	bool bInAttackMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Locomotion")
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Locomotion")
	class ADiana* Diana;
};
