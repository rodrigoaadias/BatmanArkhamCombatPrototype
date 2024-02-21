// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/SAbility.h"
#include "SCounterAttack.generated.h"

class USCombatComponent;
/**
 * 
 */
UCLASS()
class ARKHAMCOMBPROTOTYPE_API USCounterAttack : public USAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Counter Attack")
	UAnimMontage* CounterAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Counter Attack")
	float DistanceFromEnemy{90.0f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flow Attack")
	FName FlowAttackAbilityName{"Attack"};

	virtual void Setup(AActor* Owner) override;
	virtual void StartAbility_Implementation(AActor* InstigatorActor) override;
	virtual void StopAbility_Implementation(AActor* InstigatorActor) override;

	FTimerHandle Counter_TimerHandle{};

private:
	UPROPERTY()
	USCombatComponent* CombatComponent{};

	void SetWarpTarget(const AActor* Instigator) const;
};
