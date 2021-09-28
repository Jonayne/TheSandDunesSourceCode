// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "DianaPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "Diana.h"
#include "TheSandDunesSaveGame.h"
#include "TheSandDunesGameInstance.h"
#include "Pickup.h"
#include "Enemy.h"
#include "SavableActorInterface.h"


void ADianaPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    Diana = GetPawn<ADiana>();

    if(Diana)
    {
        Diana->DianaPlayerController = this;
    }
    
    if(HUDOverlayAsset)
    {
        HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
        HUDOverlay->AddToViewport();
    }

    if(HUDPauseMenuAsset)
    {
        HUDPauseMenu = CreateWidget<UUserWidget>(this, HUDPauseMenuAsset);
    }

    if(HUDStatsMenuAsset)
    {
        HUDStatsMenu = CreateWidget<UUserWidget>(this, HUDStatsMenuAsset);
    }

    OnSpellListChanged.AddDynamic(this, &ADianaPlayerController::SpellListHasChanged);

    UpdateWorld();

}

void ADianaPlayerController::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);
}

void ADianaPlayerController::UpdateWorld() 
{
    UWorld* World = GetWorld();
	UTheSandDunesGameInstance* GameInstance = World ? World->GetGameInstance<UTheSandDunesGameInstance>() : nullptr;
    
	if (GameInstance)
	{
        UTheSandDunesSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
        if(CurrentSaveGame)
        {
            // Actualizamos el estado de Diana y su UI.
            UpdateDianaState(CurrentSaveGame->DianaStats);
            UpdateDianaManaBar();
            UpdateDianaHealthBar();

            if(CurrentSaveGame->DianaStats.SpellsOrder.Num() == 0)
            {
                DianaSpellsOrder = ResetSpellList();
            }
            else
            {
				DianaSpellsOrder = CurrentSaveGame->DianaStats.SpellsOrder;
                
            }
			
			UpdateSpellListOrder();
            OnSpellListChanged.Broadcast();
            UpdateChoosenSpellByIndex(CurrentSaveGame->DianaStats.CurrentSpellIndex);
        }
	}
}

void ADianaPlayerController::SetChoosenSpell(FString SpellName)
{
    if(Diana)
    {
        if(!SpellName.IsEmpty()) Diana->ChangeCurrentSpell(SpellName);
    }
    return;
}

void ADianaPlayerController::SetDianaSpellsOrder(TMap<FString, int32> NewOrder) 
{
    DianaSpellsOrder = NewOrder;
    OnSpellListChanged.Broadcast();
}

void ADianaPlayerController::UpdateDianaState(FDianaStats DianaStats) 
{
    if(Diana)
	{
		Diana->BaseMaxHealth = DianaStats.MaxHealth > 0 ? DianaStats.MaxHealth : Diana->BaseMaxHealth;
        Diana->MaxHealth = DianaStats.MaxHealth > 0 ? DianaStats.MaxHealth : Diana->BaseMaxHealth;
		Diana->Health = DianaStats.Health > 0 ? DianaStats.Health : Diana->Health;
		Diana->BaseMaxStamina = DianaStats.MaxStamina > 0 ? DianaStats.MaxStamina : Diana->BaseMaxStamina;
        Diana->MaxStamina = DianaStats.MaxStamina > 0 ? DianaStats.MaxStamina : Diana->BaseMaxStamina;
		Diana->Stamina = DianaStats.Stamina > 0 ? DianaStats.Stamina : Diana->BaseMaxStamina;
        Diana->BaseMaxDamage = DianaStats.BaseDamage;
		Diana->BaseDamage = DianaStats.BaseDamage;
        Diana->CurrentSpellIndex = DianaStats.CurrentSpellIndex;
        Diana->RemainingSaves = DianaStats.RemainingSaves;

        if(DianaStats.DianaResistances.Num() > 0)
        {
            Diana->SpiritResistances = DianaStats.DianaResistances;
            Diana->DianaBaseResistances = DianaStats.DianaResistances;
        }
        else
        {
            Diana->DianaBaseResistances =  Diana->SpiritResistances;
        }

        if(DianaStats.DianaStates.Num() > 0)
        {
            Diana->SetStatesList(DianaStats.DianaStates);
        }

		if(DianaStats.DianaWeakSpells.Num() > 0)
		{
			Diana->SetWeakSpellList(DianaStats.DianaWeakSpells);
			Diana->SetStrongSpellList(DianaStats.DianaStrongSpells);
		}
        
		Diana->ChangeCurrentSpell(DianaStats.CurrentSpell);
        if(!DianaStats.Location.IsZero() && !DianaStats.bIsTPing)
        {
            Diana->SetActorLocation(DianaStats.Location);
		    Diana->SetActorRotation(DianaStats.Rotation);
        }
        else if(DianaStats.bIsTPing)
        {
            Diana->bIsTPing = false;
            Diana->SetActorLocation(DianaStats.NextMapLocation);
            Diana->SetActorRotation(DianaStats.NextMapRotation);
        }

        ChangeRemainingSaves(DianaStats.RemainingSaves);

        if(DianaStats.CorruptionLevel > 0)
        {
            for (int32 i = 0; i < DianaStats.CorruptionLevel; i++)
            {
                Diana->CorruptDiana();
            }
        }
        else if(DianaStats.CorruptionLevel < 0)
        {
            for (int32 i = 0; i > DianaStats.CorruptionLevel; i--)
            {
                Diana->DescorruptDiana();
            }
        }
	}
}

TMap<FString, int32> ADianaPlayerController::ResetSpellList() 
{
    if(Diana)
    {
        TMap<FString, int32> SpellsMapReset;

        TArray<FString> Spells = Diana->GetListOfSpellsNames();
        for (int32 Index = 0; Index < Spells.Num(); ++Index)
        {
            SpellsMapReset.Add(Spells[Index], Index+1);
        }

        return SpellsMapReset;
    }

    return TMap<FString, int32>();
}

void ADianaPlayerController::UpdateSpellOrderForNewSpell(FString SpellName) 
{
    // Si es un hechizo nuevo, no una mejora.
    if(!DianaSpellsOrder.Contains(SpellName))
    {
        // 6 es el maximo numero de hechizos y solo se pueden asignar esas teclas para cambiar
        // entre hechizos, so...
        for (int32 Index = 1; Index <= 6; ++Index)
        {
            if(!DianaSpellsValues.Contains(Index))
            {
                DianaSpellsOrder.Add(SpellName, Index);
                OnSpellListChanged.Broadcast();
                return;
            }
        }
    }
}

int32 ADianaPlayerController::GetNextValidSpellIndex(int32 CurrentIndex, bool bIndexUp) 
{
    int32 NextIndex = CurrentIndex;

    // Si aun no tiene todos los hechizos, pueden haber espacios vacios.
    if(DianaSpellsOrder.Num() < 6)
    {
        int32 CurrentIndexOfVal = DianaSpellsValues.IndexOfByKey(CurrentIndex);
        int32 IndexToAdd = bIndexUp ? 1 : -1;

        if(DianaSpellsValues.IsValidIndex(CurrentIndexOfVal+IndexToAdd))
        {
            NextIndex = DianaSpellsValues[CurrentIndexOfVal+IndexToAdd];
        }
    }
    else
    {
        NextIndex = bIndexUp ? FMath::Min(NextIndex+1, 6) : FMath::Max(NextIndex-1, 1);
    }
    
    return NextIndex;
}

bool ADianaPlayerController::SaveGame() 
{
    UWorld* World = GetWorld();
	UTheSandDunesGameInstance* GameInstance = World ? World->GetGameInstance<UTheSandDunesGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	UTheSandDunesSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if (CurrentSaveGame && Diana && !Diana->IsDead() && !Diana->bInBossFight)
	{
        CurrentSaveGame->DianaStats.MaxHealth = Diana->BaseMaxHealth;
        CurrentSaveGame->DianaStats.Health = FMath::Min(Diana->Health, Diana->BaseMaxHealth);
        CurrentSaveGame->DianaStats.MaxStamina = Diana->BaseMaxStamina;
        CurrentSaveGame->DianaStats.Stamina = FMath::Min(Diana->Stamina, Diana->BaseMaxStamina);
        CurrentSaveGame->DianaStats.Location = Diana->GetActorLocation();
        CurrentSaveGame->DianaStats.Rotation = Diana->GetActorRotation();
        CurrentSaveGame->DianaStats.bIsTPing = Diana->bIsTPing;
        CurrentSaveGame->DianaStats.NextMapLocation = Diana->DianaNextTPLocation;
        CurrentSaveGame->DianaStats.NextMapRotation = Diana->DianaNextTPRotation;
        CurrentSaveGame->DianaStats.BaseDamage = Diana->BaseMaxDamage;
        CurrentSaveGame->DianaStats.DianaResistances = Diana->DianaBaseResistances;
        CurrentSaveGame->DianaStats.DianaWeakSpells = Diana->GetWeakSpellsList();
        CurrentSaveGame->DianaStats.DianaStrongSpells = Diana->GetStrongSpellsList();
        CurrentSaveGame->DianaStats.DianaStates = Diana->GetDianaStatesList();
        CurrentSaveGame->DianaStats.CurrentSpell = Diana->GetCurrentSpell();
        CurrentSaveGame->DianaStats.SpellsOrder = DianaSpellsOrder;
        CurrentSaveGame->DianaStats.CurrentSpellIndex = Diana->CurrentSpellIndex;
        CurrentSaveGame->DianaStats.RemainingSaves = Diana->RemainingSaves;
        CurrentSaveGame->DianaStats.CorruptionLevel = Diana->CorruptionLevel;

        if(Diana->bIsTPing)
        {
            CurrentSaveGame->DianaStats.CurrentLevelName = Diana->DianaNextMap;
        }
        else
        {
            CurrentSaveGame->DianaStats.CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this, true); 
        }     

        CurrentSaveGame->WorldState = GameInstance->GetCurrentWorldState();
        
        GameInstance->SetCurrentSaveGame(CurrentSaveGame);
		GameInstance->WriteSaveGame();
        
		return true;
	}
	return false;
}

bool ADianaPlayerController::LoadGame() 
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
		if(Diana)
        {
            FName LoadLevel = FName(CurrentSaveGame->DianaStats.CurrentLevelName);
            UGameplayStatics::OpenLevel(this, LoadLevel, false);
        }
		return true;
	}

	return false;
}

void ADianaPlayerController::SpellListHasChanged() 
{
    DianaSpellsOrder.ValueSort([](int32 A, int32 B) {
        return A < B;
    });

    DianaSpellsOrder.GenerateValueArray(DianaSpellsValues);
}

bool ADianaPlayerController::ExistSpellIndex(int32 Index) 
{
    if(DianaSpellsValues.Contains(Index))
    {
        return true;
    }

    return false;
}

void ADianaPlayerController::ShowStatsMenu_Implementation() 
{
    if(HUDStatsMenu && !HUDStatsMenu->IsInViewport())
    {
        if(HUDOverlay) HUDOverlay->SetVisibility(ESlateVisibility::Collapsed);
        bGamePaused = true;
        
        FInputModeUIOnly FInputMode;
        SetInputMode(FInputMode);
        bShowMouseCursor = true;
        SetPause(true);
        HUDStatsMenu->AddToViewport();  
    }   
}

void ADianaPlayerController::HideStatsMenu_Implementation() 
{
    if(HUDStatsMenu && HUDStatsMenu->IsInViewport())
    {
        if(HUDOverlay) HUDOverlay->SetVisibility(ESlateVisibility::Visible);
        bGamePaused = false;

        FInputModeGameOnly FInputMode;

        SetInputMode(FInputMode);
        bShowMouseCursor = false;

        SetPause(false);
        HUDStatsMenu->RemoveFromParent();
    }
}

void ADianaPlayerController::ShowPauseMenu_Implementation() 
{
    if(HUDPauseMenu && !HUDPauseMenu->IsInViewport())
    {
        if(HUDOverlay) HUDOverlay->SetVisibility(ESlateVisibility::Collapsed);
        bGamePaused = true;
        
        FInputModeUIOnly FInputMode;

        SetInputMode(FInputMode);
        bShowMouseCursor = true;
        SetPause(true);
        HUDPauseMenu->AddToViewport();  
    }  
}

void ADianaPlayerController::HidePauseMenu_Implementation() 
{
    if(HUDPauseMenu && HUDPauseMenu->IsInViewport())
    {
        if(HUDOverlay) HUDOverlay->SetVisibility(ESlateVisibility::Visible);
        bGamePaused = false;

        FInputModeGameOnly FInputMode;

        SetInputMode(FInputMode);
        bShowMouseCursor = false;

        SetPause(false);
        HUDPauseMenu->RemoveFromParent();
    }
}
