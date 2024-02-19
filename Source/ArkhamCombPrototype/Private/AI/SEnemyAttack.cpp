#include "AI/SEnemyAttack.h"
#include "AIController.h"
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

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "StopAbility", GetCharacterOwner());
	GetWorld()->GetTimerManager().SetTimer(Attack_TimerHandle, Delegate, Duration, false);

	ensure(AiCharacter);
	AiCharacter->OnPunchStarted.AddDynamic(this, &USEnemyAttack::SetupPunchTarget);
}

void USEnemyAttack::StopAbility_Implementation(AActor* InstigatorActor)
{
	Super::StopAbility_Implementation(InstigatorActor);
	GetWorld()->GetTimerManager().ClearTimer(Attack_TimerHandle);
	AiCharacter->OnPunchStarted.RemoveDynamic(this, &USEnemyAttack::SetupPunchTarget);
}

void USEnemyAttack::SetupPunchTarget()
{
	PunchStartLocation = GetCharacterOwner()->GetActorLocation();
	const AAIController* AiController = Cast<AAIController>(GetCharacterOwner()->GetController());
	if(!AiController)
	{
		return;
	}

	TargetPlayer = Cast<AActor>(AiController->GetBlackboardComponent()->GetValueAsObject(PlayerBlackboardKeyName));
	if(!ensure(TargetPlayer))
	{
		UE_LOG(LogTemp, Error, TEXT("NO TARGET PLAYER SET IN BLACKBOARD COMP"));
	}

	FVector TargetWarpLocation = TargetPlayer->GetActorLocation() - GetCharacterOwner()->GetActorForwardVector() * 90.0f;
	const float Distance = FVector::Dist2D(GetCharacterOwner()->GetActorLocation(), TargetWarpLocation);
	if(Distance > MaxWarpDistance)
	{
		TargetWarpLocation = GetCharacterOwner()->GetActorLocation() + GetCharacterOwner()->GetActorForwardVector() * MaxWarpDistance;
	}

	AiCharacter->OnUpdateWarpTarget.Broadcast(TargetWarpLocation);
}
