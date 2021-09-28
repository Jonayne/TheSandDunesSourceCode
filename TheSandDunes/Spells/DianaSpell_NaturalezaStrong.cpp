// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "DianaSpell_NaturalezaStrong.h"
#include "Tornado.h"


void ADianaSpell_NaturalezaStrong::BeginPlay() 
{
    Super::BeginPlay();
    
    if(TornadoClass)
    {
        FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
        ATornado* TornadoToSpawn;
		TornadoToSpawn = GetWorld()->SpawnActor<ATornado>(TornadoClass, SpawnParams);
		TornadoToSpawn->AttachToActor(OwnerPawn, FAttachmentTransformRules::KeepRelativeTransform);
    }
}
