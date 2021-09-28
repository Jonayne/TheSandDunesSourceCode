// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Dasher.h"
#include "ProjectilDasher.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API AProjectilDasher : public ADasher
{
	GENERATED_BODY()
	
public:
	AProjectilDasher();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dasher")
	float DashDistance;

public:

	virtual void DasherAction() override;

	virtual FVector CalculateDashVelocity() override;
	
};
