// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(UObject* WorldContext, FString Message, FColor Color = FColor::White, float Duration = 5.0f)
{
	if(!ensure(WorldContext))
	{
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if(!ensure(World))
	{
		return;
	}

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
	}
}