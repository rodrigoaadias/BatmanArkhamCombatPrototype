// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/SAbility.h"
#include "SFlowAttack.generated.h"

class USCombatComponent;
/**
 * 
 */
UCLASS()
class ARKHAMCOMBPROTOTYPE_API USFlowAttack : public USAbility
{
	GENERATED_BODY()

public:
	void Setup(AActor* Owner) override;
	bool CanStart(AActor* InstigatorActor) override;
	void StartAbility_Implementation(AActor* InstigatorActor) override;
	void StopAbility_Implementation(AActor* InstigatorActor) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> Attacks{};
	UAnimMontage* LastAttack{};

	FTimerHandle Attack_TimerHandle{};
	
	TObjectPtr<USCombatComponent> CombatComponent{};
	TObjectPtr<AActor> Enemy{};

	UAnimMontage* GetRandomAttack();
};
