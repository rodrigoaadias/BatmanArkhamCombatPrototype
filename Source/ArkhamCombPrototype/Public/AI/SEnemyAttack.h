// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/SAbility.h"
#include "SEnemyAttack.generated.h"

/**
 * 
 */
UCLASS()
class ARKHAMCOMBPROTOTYPE_API USEnemyAttack : public USAbility
{
	GENERATED_BODY()

public:
	virtual void StartAbility_Implementation(AActor* InstigatorActor) override;
	virtual void StopAbility_Implementation(AActor* InstigatorActor) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Attack;

	FTimerHandle Attack_TimerHandle{};
};
