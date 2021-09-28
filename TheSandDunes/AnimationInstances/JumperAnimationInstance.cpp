// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "JumperAnimationInstance.h"
#include "GameFramework/Pawn.h"
#include "Jumper.h"

void UJumperAnimationInstance::NativeInitializeAnimation() 
{
  if (!Pawn) 
  {
      Pawn = TryGetPawnOwner();
      if(Pawn)
      {
          Jumper = Cast<AJumper>(Pawn);
      }
  }
}

void UJumperAnimationInstance::UpdateAnimationVariables() 
{
    if (!Pawn) 
    {
      Pawn = TryGetPawnOwner();
    }

    if (Pawn)
    {
        Jumper = Cast<AJumper>(Pawn);
        if(Jumper)
        {
          bInAir = Jumper->IsInAir();
        }
    }
}
