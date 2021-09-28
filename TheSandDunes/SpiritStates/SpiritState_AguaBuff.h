// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "SpiritState.h"
#include "SpiritState_AguaBuff.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ASpiritState_AguaBuff : public ASpiritState
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASpiritState_AguaBuff();	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	float DianaLifeStealPercentage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	
	virtual bool ApplyStateEffect() override;

	virtual void RemoveStateEffect() override;

private:
	class ADiana* Diana;
};
