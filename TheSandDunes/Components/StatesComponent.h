// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THESANDDUNES_API UStatesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatesComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "States")
	TMap<FString, TSubclassOf<class ASpiritState>> SpiritStates;

	TMap<FString, bool> SpiritStatesApplied;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float ApplyEffect(FString TypeOfState, bool bIsDebuf);

	void ChangeStateStatus(FString StateName, bool bApplied);

	float GetBuffCooldown(FString StateName);

	bool IsInState(FString TypeOfState, bool bIsDebuf);

	TMap<FString, TSubclassOf<ASpiritState>> GetSpiritStates();

	TSubclassOf<ASpiritState> GetSpiritStateClass(FString StateName);
	
	void SetSpiritState(FString StateName, TSubclassOf<ASpiritState> NewState);
	
	void SetStatesList(TMap<FString, TSubclassOf<class ASpiritState>> NewList);
	
private:
	class ASpirit* SpiritOwner;
	
};
