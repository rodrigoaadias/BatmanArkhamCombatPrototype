// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/SAbility.h"
#include "SCounterAttackTimer.generated.h"

class USCombatComponent;
/**
 * 
 */
UCLASS()
class ARKHAMCOMBPROTOTYPE_API USCounterAttackTimer : public USAbility
{
	GENERATED_BODY()

protected:

	TObjectPtr<USCombatComponent> CombatComponent{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Counter Attack")
	float CounterAttackWindowDuration{1.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Counter Attack")
	FName CounterAttackAbilityName{"Counter Attack"};

	virtual void Setup(AActor* Owner) override;
	virtual void StartAbility_Implementation(AActor* InstigatorActor) override;
	virtual void StopAbility_Implementation(AActor* InstigatorActor) override;

	FTimerHandle Window_TimerHandle{};

private:

	UPROPERTY()
	TObjectPtr<ASCharacter> OwnerChar{};

	UPROPERTY()
	TObjectPtr<AActor> Instigator{};

	bool bCounterAttackPressed{false};
	
	UFUNCTION()
	void StartCounterAttack();
	
	UFUNCTION()
	void SetToCounter();
};
