// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TSDTypes.h"
#include "TheSandDunesGameInstance.generated.h"

class UTheSandDunesSaveGame;

/**
 * Clase base del Game Instance.
 */
UCLASS()
class THESANDDUNES_API UTheSandDunesGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTheSandDunesGameInstance();

	/** Nombre del slot usado para guardar. */
	UPROPERTY(BlueprintReadWrite, Category = Save)
	FString SaveSlot;

	/** User index. */
	UPROPERTY(BlueprintReadWrite, Category = Save)
	int32 SaveUserIndex;

	/** Si deberíamos mostrar el modal (o no) de save game ingame.. */
	UPROPERTY(BlueprintReadWrite, Category = MapsChange)
	bool bShouldShowModal;

	/** Guarda el estado de los mundos. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Save)
	TMap<FString, FSavableActorMap> WorldGameInstanceState;

	/** Nos da el Save Game actual. Los cambios nos e hacen hasta llamar WriteSaveGame */
	UFUNCTION(BlueprintCallable, Category = Save)
	UTheSandDunesSaveGame* GetCurrentSaveGame();

	/** Nos da el estado actual de los mapas del juego. */
	UFUNCTION(BlueprintCallable, Category = Save)
	TMap<FString, FSavableActorMap> GetCurrentWorldState();

	/** Actualiza los valores que se guardarna la proxima vez que se llame la funcion WriteSaveGame. */
	UFUNCTION(BlueprintCallable, Category = Save)
	void SetCurrentSaveGame(UTheSandDunesSaveGame* NewSave);

	/** Actualiza el estado del mundo. */
	UFUNCTION(BlueprintCallable, Category = Save)
	void SetCurrentWorldState(TMap<FString, FSavableActorMap> NewWorldState);

	/** Carga un save game síncronamente. Crea uno nuevo si falla y regresa false, si no, regresa true. */
	UFUNCTION(BlueprintCallable, Category = Save)
	bool LoadOrCreateSaveGame();

	/** Nos da los actores de este nivel que son guardables */
	UFUNCTION(BlueprintCallable, Category = Save)
	TMap<FName, FSavableActor> GetActorsByLevel(FString LevelName);

	/** Acciones finales después de cargar un USaveGame usando la func AsyncLoadGameFromSlot. True si cargó, false si no y creo uno.*/
	UFUNCTION(BlueprintCallable, Category = Save)
	bool HandleSaveGameLoaded(USaveGame* SaveGameObject);

	/** Nos da el SlotName y el Index de Diana y el estado del mundo. */
	UFUNCTION(BlueprintCallable, Category = Save)
	void GetSaveSlotInfo(FString& SlotName, int32& UserIndex) const;

	/** Escribe el save game actual en el disco. Ocurre en otro hilo. */
	UFUNCTION(BlueprintCallable, Category = Save)
	bool WriteSaveGame();

	/** Reinicia el save game a su default, borrando los datos. No se salva en disco hasta la sig. llamada a WriteSaveGame */
	UFUNCTION(BlueprintCallable, Category = Save)
	void ResetSaveGame();

	/** Cambia el estado del Actor State de IsAlive */
	UFUNCTION(BlueprintCallable, Category = Save)
	void ChangeSavableActorState(FString LevelName, FSavableActor ActorUpdated);

	/** Se llama cuando ya se hizo el SaveGame. */
	UFUNCTION(BlueprintImplementableEvent, Category=Save)
	void SaveGameHasFinished();

	/** Intenta cargar el estado del mundo mas reciente, si existe. Si no, crea el default. */
	UFUNCTION(BlueprintCallable, Category = Save)
	bool LoadOrCreateWorldState(FString LevelName);
	
protected:
	/** El objeto de save game. */
	UPROPERTY()
	UTheSandDunesSaveGame* CurrentSaveGame;

	class ADiana* Diana;

	UPROPERTY()
	bool bCurrentlySaving;

	UPROPERTY()
	bool bPendingSaveRequested;

	/** Cuando el guardado ocurre, se llama esta func. */
	virtual void HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess);

private:
	void CreateDefaultStateForThisLevel(FString LevelName);
};
