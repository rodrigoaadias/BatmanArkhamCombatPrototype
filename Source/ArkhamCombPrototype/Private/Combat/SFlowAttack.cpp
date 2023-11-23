#include "Combat/SFlowAttack.h"
#include "SCharacter.h"
#include "SCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	
}

void USFlowAttack::StopAbility_Implementation(AActor* InstigatorActor)
{
	Super::StopAbility_Implementation(InstigatorActor);
}
