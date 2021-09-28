// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AmitAnimationInstance.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API UAmitAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category="AnimationProperties")
	void UpdateAnimationVariables();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Locomotion")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Locomotion")
	bool bInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Locomotion")
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Locomotion")
	class AAmit* Amit;
};
