#include "Combat/SFlowAttack.h"
#include "SCharacter.h"
#include "SCombatComponent.h"
#include "SCharacterMovementComponent.h"

void USFlowAttack::Setup(AActor* Owner)
{
	Super::Setup(Owner);
	CombatComponent = Owner->GetComponentByClass<USCombatComponent>();
}

bool USFlowAttack::CanStart(AActor* InstigatorActor)
{
	if(!Super::CanStart(InstigatorActor) || Attacks.Num() == 0)
	{
		return false;
	}

	const FVector Direction = GetCharacterOwner()->GetInputDirection();
	Enemy = CombatComponent->GetEnemyOnDirection(Direction);

	return Enemy != nullptr;
}

void USFlowAttack::StartAbility_Implementation(AActor* InstigatorActor)
{
	Super::StartAbility_Implementation(InstigatorActor);

	const FVector AttackDirection = (Enemy->GetActorLocation() - GetCharacterOwner()->GetActorLocation()).GetSafeNormal2D();
	const FVector TargetLoc = Enemy->GetActorLocation() - AttackDirection * 100.0f; 
	const FRotator TargetRot = AttackDirection.Rotation();
	CombatComponent->SetWarpTarget(TargetLoc, TargetRot);

	const float Duration = GetCharacterOwner()->PlayAnimMontage(GetRandomAttack()) - 0.15f;

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "StopAbility", GetCharacterOwner());
	GetWorld()->GetTimerManager().SetTimer(Attack_TimerHandle, Delegate, Duration, false);

	GetCharacterOwner()->GetCharacterMovement()->SetMovementMode(MOVE_Custom, CMOVE_ATTACK);
}

void USFlowAttack::StopAbility_Implementation(AActor* InstigatorActor)
{
	Super::StopAbility_Implementation(InstigatorActor);
	GetWorld()->GetTimerManager().ClearTimer(Attack_TimerHandle);
	GetCharacterOwner()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

UAnimMontage* USFlowAttack::GetRandomAttack()
{
	if(Attacks.Num() == 1)
	{
		return Attacks[0];
	}

	int32 index = FMath::RandRange(0, Attacks.Num() - 1);
	while(Attacks.Num() > 1 && Attacks[index] == LastAttack)
	{
		index = FMath::RandRange(0, Attacks.Num()-1);
	}

	LastAttack = Attacks[index];
	return Attacks[index];
}
