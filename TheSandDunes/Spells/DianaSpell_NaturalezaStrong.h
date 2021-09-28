// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "DianaSpell_NaturalezaStrong.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ADianaSpell_NaturalezaStrong : public ASpell
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Spawn")
	TSubclassOf<class ATornado> TornadoClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
