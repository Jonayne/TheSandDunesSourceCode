// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "SpiritState.h"
#include "SpiritState_SagradoDebuf.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ASpiritState_SagradoDebuf : public ASpiritState
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASpiritState_SagradoDebuf();	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	float DamageMultiplierReduction;

public:
	
	virtual bool ApplyStateEffect() override;

	virtual void RemoveStateEffect() override;

};
