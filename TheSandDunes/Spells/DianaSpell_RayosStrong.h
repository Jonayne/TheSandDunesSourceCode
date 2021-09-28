// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "DianaSpell_RayosStrong.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ADianaSpell_RayosStrong : public ASpell
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Spawn")
	TSubclassOf<class AShield> ShieldClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
