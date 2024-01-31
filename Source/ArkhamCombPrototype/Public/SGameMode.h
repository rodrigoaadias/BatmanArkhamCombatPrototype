// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARKHAMCOMBPROTOTYPE_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	void OnActorKilled(AActor* VictimActor, AActor* KillerActor);

	UFUNCTION()
	void RestartLevel();
};
