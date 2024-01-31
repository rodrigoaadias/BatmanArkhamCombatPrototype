#include "Combat/SHitReactionAbility.h"

#include "GameFramework/Character.h"

void USHitReactionAbility::StartAbility_Implementation(AActor* InstigatorActor)
{
	Super::StartAbility_Implementation(InstigatorActor);

	const float Duration = GetCharacterOwner()->PlayAnimMontage(HitReactionMontage);

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "StopAbility", InstigatorActor);
	GetWorld()->GetTimerManager().SetTimer(Hit_TimerHandle, Delegate, Duration, false);
}

void USHitReactionAbility::StopAbility_Implementation(AActor* InstigatorActor)
{
	Super::StopAbility_Implementation(InstigatorActor);
	GetWorld()->GetTimerManager().ClearTimer(Hit_TimerHandle);
}
