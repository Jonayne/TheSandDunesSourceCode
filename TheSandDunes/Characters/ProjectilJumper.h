// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Jumper.h"
#include "ProjectilJumper.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API AProjectilJumper : public AJumper
{
	GENERATED_BODY()
	
public:
	AProjectilJumper();

public:

	virtual void JumperAction() override;

	virtual FVector CalculateJumpVelocity() override;
	
};
