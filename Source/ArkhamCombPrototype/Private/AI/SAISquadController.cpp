// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAISquadController.h"

#include "SGameMode.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"

ASAISquadController::ASAISquadController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASAISquadController::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(Attack_TimerHandle, this, &ASAISquadController::GoNextEnemy,
		AttackInterval, true, 1.0f);

	for (ASAICharacter* Enemy : SquadMembers)
	{
		Enemy->OnCharacterDied.AddDynamic(this, &ASAISquadController::HandleMemberDied);
	}
}

void ASAISquadController::GoNextEnemy()
{
	if(SquadMembers.Num() == 0)
	{
		GetWorldTimerManager().ClearTimer(Attack_TimerHandle);
		return;
	}

	CurrentMemberIndex++;
	CurrentMemberIndex = CurrentMemberIndex % SquadMembers.Num();
	SelectEnemyToAttack();
}

void ASAISquadController::SelectEnemyToAttack()
{
	if(CurrentAttackingChar != nullptr)
	{
		ResetAttacking(CurrentAttackingChar);
	}

	CurrentAttackingChar = SquadMembers[CurrentMemberIndex];
	if(CurrentAttackingChar->IsDead())
	{
		SquadMembers.RemoveAt(CurrentMemberIndex);
		GoNextEnemy();
		return;
	}

	AAIController* Controller = Cast<AAIController>(CurrentAttackingChar->GetController());
	Controller->GetBlackboardComponent()->SetValueAsBool(BlackboardAttackKey, true);
}

void ASAISquadController::ResetAttacking(const ASAICharacter* Enemy) const
{
	AAIController* Controller = Cast<AAIController>(Enemy->GetController());
	Controller->GetBlackboardComponent()->SetValueAsBool(BlackboardAttackKey, false);
}

void ASAISquadController::HandleMemberDied(ASAICharacter* EnemyDied)
{
	SquadMembers.Remove(EnemyDied);
	if(SquadMembers.Num() == 1)
	{
		ASGameMode* GM =  GetWorld()->GetAuthGameMode<ASGameMode>();
		if(GM)
		{
			GM->NotifyLastEnemyRemaining();
		}
	}
}
