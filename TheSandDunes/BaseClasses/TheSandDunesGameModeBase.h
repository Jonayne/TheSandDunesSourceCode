// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheSandDunesGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ATheSandDunesGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category="SavableActors")
	void UpdateLevelActorsState(FString CurrentLevel);
};
