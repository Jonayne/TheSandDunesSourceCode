// Copyright Epic Games, Inc. All Rights Reserved.


#include "TheSandDunesGameModeBase.h"
#include "TheSandDunesGameInstance.h"
#include "SavableActorInterface.h"
#include "Kismet/GameplayStatics.h"


void ATheSandDunesGameModeBase::UpdateLevelActorsState(FString CurrentLevel) 
{
    UWorld* World = GetWorld();
	UTheSandDunesGameInstance* GameInstance = World ? World->GetGameInstance<UTheSandDunesGameInstance>() : nullptr;
    
	if (GameInstance)
	{
        // Lista de actores que se pueden salvar en este lvl.
        TArray<AActor*> SavableActorsList;
        UGameplayStatics::GetAllActorsWithInterface(this, USavableActorInterface::StaticClass(), SavableActorsList);

        // lista de info del save game acerca de los actores salvables de este lvl.
        TMap<FName, FSavableActor> LevelActorsInfo = GameInstance->GetActorsByLevel(CurrentLevel);
        
        // los actualizamos todos segun nuestros valores del save game.
        for (auto& SavableActor : SavableActorsList)
        {
            if(LevelActorsInfo.Contains(SavableActor->GetFName()))
            {
                ISavableActorInterface::Execute_UpdateSavableInfo(SavableActor, LevelActorsInfo[SavableActor->GetFName()]);
            }
        }
	}
}
