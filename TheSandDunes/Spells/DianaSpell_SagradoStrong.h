// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "DianaSpell_SagradoStrong.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ADianaSpell_SagradoStrong : public ASpell
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Spawn")
	TSubclassOf<class ASummonDianaHelp> SummonClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
