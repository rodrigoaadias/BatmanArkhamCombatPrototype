#include "Combat/SFlowAttack.h"
#include "SCharacter.h"
#include "SCombatComponent.h"
#include "SCharacterMovementComponent.h"
#include "SGameMode.h"

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

	const FVector Direction = Cast<ASCharacter>(GetCharacterOwner())->GetInputDirection();
	Enemy = CombatComponent->GetEnemyOnDirection(Direction);

	return Enemy != nullptr;
}

void USFlowAttack::UpdateWarpTarget() const
{
	const FVector AttackDirection = (Enemy->GetActorLocation() - GetCharacterOwner()->GetActorLocation()).GetSafeNormal2D();
	const FVector TargetLoc = Enemy->GetActorLocation() - AttackDirection * 100.0f; 
	const FRotator TargetRot = AttackDirection.Rotation();
	CombatComponent->SetWarpTarget(TargetLoc, TargetRot);
}

void USFlowAttack::StartAbility_Implementation(AActor* InstigatorActor)
{
	Super::StartAbility_Implementation(InstigatorActor);
	bInFinalSequence = false;

	UpdateWarpTarget();
	CombatComponent->SetCurrentTarget(Enemy);

	UAnimMontage* SelectedMontage = GetRandomAttack();
	ASGameMode* GM = GetWorld()->GetAuthGameMode<ASGameMode>();
	if(GM && GM->IsLastEnemyRemaining())
	{
		SelectedMontage = FinalAttack;
		GM->OnFinalSequenceStarted.Broadcast();
		bInFinalSequence = true;
	}
	
	const float Duration = GetCharacterOwner()->PlayAnimMontage(SelectedMontage) - 0.15f;
	
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "StopAbility", GetCharacterOwner());
	GetWorld()->GetTimerManager().SetTimer(Attack_TimerHandle, Delegate, Duration, false);

	GetCharacterOwner()->GetCharacterMovement()->SetMovementMode(MOVE_Custom, CMOVE_ATTACK);
}

void USFlowAttack::TickAbility(float DeltaTime)
{
	Super::TickAbility(DeltaTime);
	UpdateWarpTarget();
}

void USFlowAttack::StopAbility_Implementation(AActor* InstigatorActor)
{
	Super::StopAbility_Implementation(InstigatorActor);
	GetWorld()->GetTimerManager().ClearTimer(Attack_TimerHandle);
	GetCharacterOwner()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	CombatComponent->SetCurrentTarget(nullptr);

	if(bInFinalSequence)
	{
		ASGameMode* GM = GetWorld()->GetAuthGameMode<ASGameMode>();
		if(GM)
		{
			GM->OnFinalSequenceEnded.Broadcast();
			bInFinalSequence = false;
		}
	}
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