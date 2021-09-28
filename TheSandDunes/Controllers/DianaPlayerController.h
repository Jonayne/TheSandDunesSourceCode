// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TSDTypes.h"
#include "DianaPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpellListHasChanged);

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ADianaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Reference to UMG asset in editor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	// Variable to hold the wisget after creating it.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widgets")
	UUserWidget* HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widgets")
	TSubclassOf<UUserWidget> HUDPauseMenuAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widgets")
	UUserWidget* HUDPauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widgets")
	TSubclassOf<UUserWidget> BloodOverlayAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widgets")
	UUserWidget* BloodOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widgets")
	TSubclassOf<UUserWidget> HUDStatsMenuAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widgets")
	UUserWidget* HUDStatsMenu;

	UPROPERTY(BlueprintReadOnly, Category="HUD")
	TMap<FString, int32> DianaSpellsOrder;
	
	UPROPERTY(BlueprintReadOnly, Category="HUD")
	TArray<int32> DianaSpellsValues;

	UPROPERTY(BlueprintAssignable)
	FSpellListHasChanged OnSpellListChanged;

public:
	void ShowEnemyHealthBar();
	void HideEnemyHealthBar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="HUD")
	void ShowPauseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="HUD")
	void HideStatsMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="HUD")
	void ShowStatsMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="HUD")
	void HidePauseMenu();

	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void ShowBloodOverlay();

	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void HideBloodOverlay();

	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void ChangeRemainingSaves(int32 NumberOfSaves);

	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void DianaGotCorrupted();

	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void DianaIsNotCorrupted();

	/* Cambia el Spell Actual. */
	UFUNCTION(BlueprintCallable, Category="Spell")
	void SetChoosenSpell(FString SpellName);

	UFUNCTION(BlueprintCallable, Category="Spell")
	void SetDianaSpellsOrder(TMap<FString, int32> NewOrder);

	UFUNCTION(BlueprintImplementableEvent, Category= "Death Menu")
	void ShowDeathMenu();

	UFUNCTION(BlueprintImplementableEvent, Category= "Spell")
	void ShowNoManaMessage();

	UFUNCTION(BlueprintImplementableEvent, Category="Diana HUD")
	void UpdateDianaHealthBar();

	UFUNCTION(BlueprintImplementableEvent, Category="Diana HUD")
	void UpdateDianaManaBar();

	UFUNCTION(BlueprintImplementableEvent, Category="Diana HUD")
	void UpdateAbilityBars();

	UFUNCTION(BlueprintImplementableEvent, Category="Diana HUD")
	void UpdateDianaDashBar();

	UFUNCTION(BlueprintImplementableEvent, Category="Diana HUD")
	void ShowDianaMovementModeIndicator();

	UFUNCTION(BlueprintImplementableEvent, Category="Diana HUD")
	void HideDianaMovementModeIndicator();

	UFUNCTION(BlueprintImplementableEvent, Category="Diana HUD")
	void MovementModeChanged(bool bIsUsing);

	UFUNCTION(BlueprintImplementableEvent, Category="Diana HUD")
	void UpdateDianaShieldBar();

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void UpdateDianaState(FDianaStats DianaStats);

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	TMap<FString, int32> ResetSpellList();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category= "Save Game")
	void UpdateSpellListOrder();
	
	UFUNCTION(BlueprintCallable, Category="Spell")
	void UpdateSpellOrderForNewSpell(FString SpellName);

	UFUNCTION(BlueprintImplementableEvent, Category= "Diana HUD")
	void UpdateChoosenSpell(const FString& NewSpellIcon);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category= "Diana HUD")
	void UpdateChoosenSpellByIndex(int32 SpellIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Save Game")
	void ShowSavedIcon();

	UFUNCTION(BlueprintCallable, Category ="Diana HUD")
	int32 GetNextValidSpellIndex(int32 CurrentIndex, bool bIndexUp);

	UFUNCTION(BlueprintCallable, Category ="Save Game")
	bool SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	bool LoadGame();

	UFUNCTION()
	void SpellListHasChanged();

	void HideSaveIconNow();

	bool ExistSpellIndex(int32 Index);

	bool bGamePaused;
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	class ADiana* Diana;

	void UpdateWorld();
};
