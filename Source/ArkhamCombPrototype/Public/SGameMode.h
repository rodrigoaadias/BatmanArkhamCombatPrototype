// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionDelegate);

UCLASS()
class ARKHAMCOMBPROTOTYPE_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool bLastEnemyRemaining {false};
	
public:

	void OnActorKilled(AActor* VictimActor, AActor* KillerActor);

	UFUNCTION()
	void RestartLevel();
	
	void NotifyLastEnemyRemaining();
	void ResetEnemyRemaining();
	FORCEINLINE bool IsLastEnemyRemaining() const { return bLastEnemyRemaining; }

	UPROPERTY(BlueprintAssignable)
	FActionDelegate OnFinalSequenceStarted{};

	UPROPERTY(BlueprintAssignable)
	FActionDelegate OnFinalSequenceEnded{};
};
