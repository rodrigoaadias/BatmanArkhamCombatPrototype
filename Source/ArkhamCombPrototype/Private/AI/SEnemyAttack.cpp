#include "AI/SEnemyAttack.h"
#include "AIController.h"
#include "SCombatComponent.h"
#include "AbilitySystem/SAbilityComponent.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

void USEnemyAttack::Setup(AActor* Owner)
{
	Super::Setup(Owner);
	
	AiCharacter = Cast<ASAICharacter>(GetCharacterOwner());
}

void USEnemyAttack::StartAbility_Implementation(AActor* InstigatorActor)
{
	Super::StartAbility_Implementation(InstigatorActor);

	const float Duration = GetCharacterOwner()->PlayAnimMontage(Attack);
	TargetPlayer = GetPlayerTarget();

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "StopAbility", GetCharacterOwner());
	GetWorld()->GetTimerManager().SetTimer(Attack_TimerHandle, Delegate, Duration, false);

	ensure(AiCharacter);
	AiCharacter->OnPunchStarted.AddDynamic(this, &USEnemyAttack::SetupPunchTarget);
	AiCharacter->OnCounterAttackStart.AddDynamic(this, &USEnemyAttack::SendCounterAttackEvent);
	StartCounterAttackAbility();
}

void USEnemyAttack::StopAbility_Implementation(AActor* InstigatorActor)
{
	Super::StopAbility_Implementation(InstigatorActor);
	GetWorld()->GetTimerManager().ClearTimer(Attack_TimerHandle);
	AiCharacter->OnPunchStarted.RemoveDynamic(this, &USEnemyAttack::SetupPunchTarget);
	AiCharacter->OnCounterAttackStart.RemoveDynamic(this, &USEnemyAttack::SendCounterAttackEvent);
}

void USEnemyAttack::SetupPunchTarget()
{
	PunchStartLocation = GetCharacterOwner()->GetActorLocation();	

	FVector TargetWarpLocation = TargetPlayer->GetActorLocation() - GetCharacterOwner()->GetActorForwardVector() * 90.0f;
	const float Distance = FVector::Dist2D(GetCharacterOwner()->GetActorLocation(), TargetWarpLocation);
	if(Distance > MaxWarpDistance)
	{
		TargetWarpLocation = GetCharacterOwner()->GetActorLocation() + GetCharacterOwner()->GetActorForwardVector() * MaxWarpDistance;
	}

	AiCharacter->OnUpdateWarpTarget.Broadcast(TargetWarpLocation);
}

void USEnemyAttack::SendCounterAttackEvent()
{
	const USCombatComponent* OtherCombatComp = TargetPlayer->GetComponentByClass<USCombatComponent>();
	if(OtherCombatComp)
	{
		OtherCombatComp->StartCounterAttack.Broadcast();
	}
}

AActor* USEnemyAttack::GetPlayerTarget() const
{
	const AAIController* AiController = Cast<AAIController>(GetCharacterOwner()->GetController());
	if(!AiController)
	{
		return nullptr;
	}

	AActor* Player = Cast<AActor>(AiController->GetBlackboardComponent()->GetValueAsObject(PlayerBlackboardKeyName));
	if(!ensure(Player))
	{
		UE_LOG(LogTemp, Error, TEXT("NO TARGET PLAYER SET IN BLACKBOARD COMP"));
	}

	return Player;
}

void USEnemyAttack::StartCounterAttackAbility() const
{
	USAbilityComponent* AbilityComponent = TargetPlayer->GetComponentByClass<USAbilityComponent>();
	if(AbilityComponent)
	{
		AbilityComponent->StartAbilityByTagName(GetCharacterOwner(), CounterAttackTimerAbility);
	}
}
