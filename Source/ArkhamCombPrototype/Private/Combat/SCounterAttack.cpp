#include "Combat/SCounterAttack.h"
#include "SCharacterMovementComponent.h"
#include "SCombatComponent.h"
#include "AbilitySystem/SAbilityComponent.h"
#include "GameFramework/Character.h"

void USCounterAttack::Setup(AActor* Owner)
{
	Super::Setup(Owner);

	CombatComponent = Owner->GetComponentByClass<USCombatComponent>();
}

void USCounterAttack::StartAbility_Implementation(AActor* InstigatorActor)
{
	Super::StartAbility_Implementation(InstigatorActor);

	const float Duration = GetCharacterOwner()->PlayAnimMontage(CounterAttackMontage);

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "StopAbility", InstigatorActor);
	GetWorld()->GetTimerManager().SetTimer(Counter_TimerHandle, Delegate, Duration, false);

	GetCharacterOwner()->GetCharacterMovement()->SetMovementMode(MOVE_Custom, CMOVE_ATTACK);

	SetWarpTarget(InstigatorActor);
	GetCharacterOwner()->SetCanBeDamaged(false);
}

void USCounterAttack::StopAbility_Implementation(AActor* InstigatorActor)
{
	Super::StopAbility_Implementation(InstigatorActor);
	GetWorld()->GetTimerManager().ClearTimer(Counter_TimerHandle);
	GetCharacterOwner()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterOwner()->SetCanBeDamaged(true);

	CombatComponent->SetCurrentTarget(InstigatorActor);
	GetOwningComponent()->StartAbilityByTagName(InstigatorActor, FlowAttackAbilityName);
}

void USCounterAttack::SetWarpTarget(const AActor* Instigator) const
{
	const FVector TargetLoc = Instigator->GetActorLocation() + Instigator->GetActorForwardVector() * DistanceFromEnemy;
	const FRotator TargetRot = (-Instigator->GetActorForwardVector()).Rotation();

	CombatComponent->SetWarpTarget(TargetLoc, TargetRot);
}
