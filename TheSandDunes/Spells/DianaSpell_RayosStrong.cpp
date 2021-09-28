// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "DianaSpell_RayosStrong.h"
#include "Shield.h"
#include "Spirit.h"


void ADianaSpell_RayosStrong::BeginPlay() 
{
    Super::BeginPlay();

    if(ShieldClass)
    {
        FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerPawn;
        
        if(OwnerPawn)
        {
            GetWorld()->SpawnActor<AShield>(ShieldClass, OwnerPawn->GetActorLocation(), OwnerPawn->GetActorRotation(), SpawnParams);
        }
    }
}
