// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "SpiritState.h"
#include "SpiritState_RayosBuff.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ASpiritState_RayosBuff : public ASpiritState
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASpiritState_RayosBuff();	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	float DodgeProbability;

public:
	
	virtual bool ApplyStateEffect() override;

	virtual void RemoveStateEffect() override;

};
