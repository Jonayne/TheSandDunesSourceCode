// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Pickup_Spell.h"
#include "Spell.h"
#include "Diana.h"
#include "SpiritState.h"


APickup_Spell::APickup_Spell() 
{
    bShouldMakeConstantSound = true;
}

void APickup_Spell::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    if (OtherActor && OtherActor->IsA(ADiana::StaticClass()) && !bPicked && bPickupActivated)
	{
        bPicked = true;
        ADiana* DianaOverlapped = Cast<ADiana>(OtherActor);
        if(DianaOverlapped)
        {
            // obtenemos el ultimo nivel que diana tiene de este hechizo...
            int32 ActualSpellLevel = DianaOverlapped->GetSpellTypeLevel(SpellName);
            
            if(ActualSpellLevel == 0)
            {
                bIsNewSpell = true;
                UnlockAchievementNewSpell();
            }
            else
            {
                bIsNewSpell = false;
                CheckMaxSpellAchievement(ActualSpellLevel+1);
            }

            int32 NextSpellLevel = ActualSpellLevel+1;
            if(WeakSpells.Contains(NextSpellLevel) && StrongSpells.Contains(NextSpellLevel) && BuffStates.Contains(NextSpellLevel))
            {
                DianaOverlapped->SetNewState(SpellName, BuffStates[NextSpellLevel]);
                DianaOverlapped->AddSpellToWeakComponentList(SpellName, WeakSpells[NextSpellLevel]);
                DianaOverlapped->AddSpellToStrongComponentList(SpellName, StrongSpells[NextSpellLevel]);
            }
        }
        
        PlayPickupEffects();
	}
}

void APickup_Spell::UnlockAchievementNewSpell() 
{
    FString AchievementAPI = TEXT("TSD_");
    AchievementAPI.Append(SpellName.ToUpper());
    AchievementAPI.Append("_NEW");

    SaveAchievementOnSteam(AchievementAPI);
}

void APickup_Spell::CheckMaxSpellAchievement(int32 NewLevel) 
{
    FString AchievementAPI = TEXT("");

    if(SpellName.Equals(TEXT("Arcanos")) || SpellName.Equals(TEXT("Rayos")) || 
        SpellName.Equals(TEXT("Agua")) || SpellName.Equals(TEXT("Naturaleza")))
    {
        if(NewLevel == 5)
        {
            AchievementAPI = TEXT("TSD_");
            AchievementAPI.Append(SpellName.ToUpper());
            AchievementAPI.Append("_MAX");

            SaveAchievementOnSteam(AchievementAPI);
        }
    }
    else if(SpellName.Equals(TEXT("Sagrado")) || SpellName.Equals(TEXT("Sombras")))
    {
        if(NewLevel == 4)
        {
            AchievementAPI = TEXT("TSD_");
            AchievementAPI.Append(SpellName.ToUpper());
            AchievementAPI.Append("_MAX");
            
            SaveAchievementOnSteam(AchievementAPI);
        }
    }
}
