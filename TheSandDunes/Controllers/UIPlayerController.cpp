// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "UIPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TheSandDunesSaveGame.h"
#include "TheSandDunesGameInstance.h"

bool AUIPlayerController::LoadGame() 
{
	UWorld* World = GetWorld();
	UTheSandDunesGameInstance* GameInstance = World ? World->GetGameInstance<UTheSandDunesGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	UTheSandDunesSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if (CurrentSaveGame)
	{
        FName LoadLevel = FName(CurrentSaveGame->DianaStats.CurrentLevelName);
        UGameplayStatics::OpenLevel(this, LoadLevel, false);
        
		return true;
	}

	return false;
}
