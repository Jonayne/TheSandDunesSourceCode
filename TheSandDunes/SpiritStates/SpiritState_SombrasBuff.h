// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "SpiritState.h"
#include "SpiritState_SombrasBuff.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ASpiritState_SombrasBuff : public ASpiritState
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASpiritState_SombrasBuff();	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	float SpeedToIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	float SecundJumpIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	float ExtraDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	
	virtual bool ApplyStateEffect() override;

	virtual void RemoveStateEffect() override;

private:
	class ADiana* Diana;

};
