// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SAISquadController.generated.h"

class ASAICharacter;

UCLASS()
class ARKHAMCOMBPROTOTYPE_API ASAISquadController : public AActor
{
	GENERATED_BODY()

public:
	ASAISquadController();

protected:
	UPROPERTY(EditInstanceOnly, Category = "Squad")
	TArray<ASAICharacter*> SquadMembers{};

	UPROPERTY(EditDefaultsOnly, Category = "Squad")
	FName BlackboardAttackKey;

	UPROPERTY(EditDefaultsOnly, Category = "Squad")
	float AttackInterval {5.0f};
	
	virtual void BeginPlay() override;

private:
	int CurrentMemberIndex{-1};
	TObjectPtr<ASAICharacter> CurrentAttackingChar;

	void GoNextEnemy();
	void SelectEnemyToAttack();
	void ResetAttacking(const ASAICharacter* Enemy) const;

	FTimerHandle Attack_TimerHandle{};

	UFUNCTION()
	void HandleMemberDied(ASAICharacter* EnemyDied);
};
