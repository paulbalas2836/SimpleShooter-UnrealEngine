// Copyright Epic Games, Inc. All Rights Reserved.


#include "SimpleShooterGameModeBase.h"

void ASimpleShooterGameModeBase::PawnKilled(APawn* PawnKilled)
{
	UE_LOG(LogTemp, Warning, TEXT("A Pawn was killed"));
}