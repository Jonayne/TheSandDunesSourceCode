#pragma once

#include "TSDTypes.h"
#include "SavableActorInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USavableActorInterface : public UInterface
{
    GENERATED_BODY()
};

// Interfaz para los actores que necesitan guardar su estado entre sesiones de juego.
class ISavableActorInterface
{    
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="SavableActor")
    void ChangeActorState();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="SavableActor")
    FSavableActor GetSavableActorInfo() const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="SavableActor")
    void UpdateSavableInfo(const FSavableActor& NewActorInfo);
};
