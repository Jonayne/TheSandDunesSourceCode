// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "TheSandDunesGameInstance.h"
#include "TheSandDunesSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Diana.h"
#include "SavableActorInterface.h"


UTheSandDunesGameInstance::UTheSandDunesGameInstance() 
{
    SaveSlot = TEXT("SaveGame");
	SaveUserIndex = 0;
	bShouldShowModal = true;
}

UTheSandDunesSaveGame* UTheSandDunesGameInstance::GetCurrentSaveGame() 
{
    return CurrentSaveGame;
}

TMap<FString, FSavableActorMap> UTheSandDunesGameInstance::GetCurrentWorldState() 
{
	return WorldGameInstanceState;
}

void UTheSandDunesGameInstance::SetCurrentSaveGame(UTheSandDunesSaveGame* NewSave) 
{
	CurrentSaveGame = NewSave;
}

void UTheSandDunesGameInstance::SetCurrentWorldState(TMap<FString, FSavableActorMap> NewWorldState) 
{
	WorldGameInstanceState = NewWorldState;
}

bool UTheSandDunesGameInstance::LoadOrCreateSaveGame() 
{
    UTheSandDunesSaveGame* LoadedSave = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex))
	{
		LoadedSave = Cast<UTheSandDunesSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
	}

	return HandleSaveGameLoaded(LoadedSave);
}

TMap<FName, FSavableActor> UTheSandDunesGameInstance::GetActorsByLevel(FString LevelName) 
{
	if(WorldGameInstanceState.Contains(LevelName))
	{
		return WorldGameInstanceState[LevelName].SavableMap;
	}

	return TMap<FName, FSavableActor>();	
}

bool UTheSandDunesGameInstance::HandleSaveGameLoaded(USaveGame* SaveGameObject) 
{
    bool bLoaded = false;

	// Reemplazamos el objeto de guardado. El otro será recolectado por el GC.
	CurrentSaveGame = Cast<UTheSandDunesSaveGame>(SaveGameObject);

	if (CurrentSaveGame)
	{
		bLoaded = true;
	}
	else
	{
		// Lo creamos en demanda.
		CurrentSaveGame = Cast<UTheSandDunesSaveGame>(UGameplayStatics::CreateSaveGameObject(UTheSandDunesSaveGame::StaticClass()));
	}

	return bLoaded;
}

void UTheSandDunesGameInstance::GetSaveSlotInfo(FString& SlotName, int32& UserIndex) const
{
    SlotName = SaveSlot;
	UserIndex = SaveUserIndex;
}

bool UTheSandDunesGameInstance::WriteSaveGame() 
{
    if (bCurrentlySaving)
    {
        bPendingSaveRequested = true;
        return true;
    }

    bCurrentlySaving = true;

    UGameplayStatics::AsyncSaveGameToSlot(GetCurrentSaveGame(), SaveSlot, SaveUserIndex, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &UTheSandDunesGameInstance::HandleAsyncSave));
    return true;
}

void UTheSandDunesGameInstance::ResetSaveGame() 
{
	HandleSaveGameLoaded(nullptr);
}

void UTheSandDunesGameInstance::ChangeSavableActorState(FString LevelName, FSavableActor ActorUpdated) 
{
	if(WorldGameInstanceState.Contains(LevelName) && 
		WorldGameInstanceState[LevelName].SavableMap.Contains(ActorUpdated.SavableId))
	{
		WorldGameInstanceState[LevelName].SavableMap[ActorUpdated.SavableId] = ActorUpdated;
	}
}

bool UTheSandDunesGameInstance::LoadOrCreateWorldState(FString LevelName) 
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex))
	{
		if(CurrentSaveGame)
		{
			WorldGameInstanceState = CurrentSaveGame->WorldState;
			
			// cuando entramos a nuevo nivel, aunque exista el save game, no se ha creado la
			// lista de actores de este nivel en el world state.
			if(!WorldGameInstanceState.Contains(LevelName))
			{
				CreateDefaultStateForThisLevel(LevelName);
				CurrentSaveGame->WorldState = WorldGameInstanceState;
				return false;
			}
			return true;
		}
	}
	else
	{
		CreateDefaultStateForThisLevel(LevelName);
		CurrentSaveGame->WorldState = WorldGameInstanceState;
		return false;
	}
	
	return false;
}

void UTheSandDunesGameInstance::CreateDefaultStateForThisLevel(FString LevelName) 
{
	TArray<AActor*> SavableActors;
	UGameplayStatics::GetAllActorsWithInterface(this, USavableActorInterface::StaticClass(), SavableActors);

	TMap<FName, FSavableActor> ActorsInfo;

	for (auto& ActorToSave : SavableActors)
	{
		FSavableActor ActorInfo = ISavableActorInterface::Execute_GetSavableActorInfo(ActorToSave);
		ActorInfo.SavableId = ActorToSave->GetFName();
		ISavableActorInterface::Execute_UpdateSavableInfo(ActorToSave, ActorInfo);
		ActorsInfo.Add(ActorInfo.SavableId, ActorInfo);
	}

	WorldGameInstanceState.Add(LevelName, FSavableActorMap(ActorsInfo));
}

void UTheSandDunesGameInstance::HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess) 
{
    ensure(bCurrentlySaving);
	bCurrentlySaving = false;

	if (bPendingSaveRequested)
	{
		bPendingSaveRequested = false;
		WriteSaveGame();
	}
	else
	{
		if(bShouldShowModal)
		{
			SaveGameHasFinished();
		}
		else
		{
			bShouldShowModal = true;
		}
	}
}
