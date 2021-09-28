// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TSDTypes.h"
#include "TheSandDunesSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API UTheSandDunesSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UTheSandDunesSaveGame();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Basic SaveGame Information")
	FDianaStats DianaStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Basic SaveGame Information")
	TMap<FString, FSavableActorMap> WorldState;

};
