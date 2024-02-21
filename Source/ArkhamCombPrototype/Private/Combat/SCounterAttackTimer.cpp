#include "Combat/SCounterAttackTimer.h"
#include "SCharacter.h"
#include "SCombatComponent.h"
#include "AbilitySystem/SAbilityComponent.h"

void USCounterAttackTimer::Setup(AActor* Owner)
{
	Super::Setup(Owner);	
	CombatComponent = Owner->GetComponentByClass<USCombatComponent>();
	OwnerChar = Cast<ASCharacter>(GetCharacterOwner());
}

void USCounterAttackTimer::StartAbility_Implementation(AActor* InstigatorActor)
{
	Super::StartAbility_Implementation(InstigatorActor);
	bCounterAttackPressed = false;
	Instigator = InstigatorActor;

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "StopAbility", InstigatorActor);
	GetWorld()->GetTimerManager().SetTimer(Window_TimerHandle, Delegate, CounterAttackWindowDuration, false);

	CombatComponent->StartCounterAttack.AddDynamic(this, &USCounterAttackTimer::StartCounterAttack);
	OwnerChar->OnCounterAttackPressed.AddDynamic(this, &USCounterAttackTimer::SetToCounter);
}

void USCounterAttackTimer::StopAbility_Implementation(AActor* InstigatorActor)
{
	Super::StopAbility_Implementation(InstigatorActor);
	Instigator = nullptr;
	bCounterAttackPressed = false;
	OwnerChar->OnCounterAttackPressed.RemoveDynamic(this, &USCounterAttackTimer::SetToCounter);
}

void USCounterAttackTimer::StartCounterAttack()
{
	if(bCounterAttackPressed && IsRunning())
	{
		GetOwningComponent()->StartAbilityByTagName(Instigator, CounterAttackAbilityName);
	}
}

void USCounterAttackTimer::SetToCounter()
{
	bCounterAttackPressed = true;
}
