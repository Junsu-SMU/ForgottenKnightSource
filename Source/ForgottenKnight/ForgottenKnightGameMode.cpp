// Copyright Epic Games, Inc. All Rights Reserved.

#include "ForgottenKnightGameMode.h"
#include "ForgottenKnightCharacter.h"
#include "UObject/ConstructorHelpers.h"

AForgottenKnightGameMode::AForgottenKnightGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
