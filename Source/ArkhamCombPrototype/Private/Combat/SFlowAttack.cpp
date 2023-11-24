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
	if(!Super::CanStart(InstigatorActor))
	{
		return false;
	}

	const FVector Direction = GetCharacterOwner()->GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal();
	Enemy = CombatComponent->GetEnemyOnDirection(Direction);

	if(Enemy)
	{
		DrawDebugSphere(GetWorld(), Enemy->GetActorLocation(), 30.0f,
			32, FColor::Green, false, 1.f);
	}

	return Enemy != nullptr;
}

void USFlowAttack::StartAbility_Implementation(AActor* InstigatorActor)
{
	Super::StartAbility_Implementation(InstigatorActor);

	const FVector AttackDirection = (Enemy->GetActorLocation() - GetCharacterOwner()->GetActorLocation()).GetSafeNormal2D();
	const FVector TargetLoc = Enemy->GetActorLocation() - AttackDirection * 100.0f; 
	const FRotator TargetRot = AttackDirection.Rotation();
	CombatComponent->SetWarpTarget(TargetLoc, TargetRot);
	
	const float Duration = GetCharacterOwner()->PlayAnimMontage(Attacks) - 0.15f;

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