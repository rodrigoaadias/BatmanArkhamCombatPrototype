#include "AI/SEnemyAttack.h"
#include "GameFramework/Character.h"

void USEnemyAttack::StartAbility_Implementation(AActor* InstigatorActor)
{
	Super::StartAbility_Implementation(InstigatorActor);

	const float Duration = GetCharacterOwner()->PlayAnimMontage(Attack);

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "StopAbility", GetCharacterOwner());
	GetWorld()->GetTimerManager().SetTimer(Attack_TimerHandle, Delegate, Duration, false);
}

void USEnemyAttack::StopAbility_Implementation(AActor* InstigatorActor)
{
	Super::StopAbility_Implementation(InstigatorActor);
	GetWorld()->GetTimerManager().ClearTimer(Attack_TimerHandle);
}