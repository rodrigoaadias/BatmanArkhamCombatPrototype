// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/SAbility.h"
#include "SEnemyAttack.generated.h"

class ASAICharacter;
/**
 * 
 */
UCLASS()
class ARKHAMCOMBPROTOTYPE_API USEnemyAttack : public USAbility
{
	GENERATED_BODY()

public:
	virtual void Setup(AActor* Owner) override;
	virtual void StartAbility_Implementation(AActor* InstigatorActor) override;
	virtual void StopAbility_Implementation(AActor* InstigatorActor) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Attack{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warp")
	FName PlayerBlackboardKeyName{"Player"};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warp")
	float MaxWarpDistance{150.0f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Counter Attack")
	FName CounterAttackTimerAbility {"CounterAttackTimer"};

	FTimerHandle Attack_TimerHandle{};

private:
	FVector PunchStartLocation{};

	UPROPERTY()
	AActor* TargetPlayer;

	UPROPERTY()
	ASAICharacter* AiCharacter;

	UFUNCTION()
	void SetupPunchTarget();

	UFUNCTION()
	void SendCounterAttackEvent();

	AActor* GetPlayerTarget() const;

	void StartCounterAttackAbility() const;
};
