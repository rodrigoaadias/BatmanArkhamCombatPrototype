// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArkhamCombPrototypeGameMode.h"
#include "ArkhamCombPrototypeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArkhamCombPrototypeGameMode::AArkhamCombPrototypeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
