#include "SCombatComponent.h"

#include "SGameMode.h"
#include "ArkhamCombPrototype/SGameplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"

TAutoConsoleVariable CVar_DebugCombatFinding(TEXT("a.DebugCombat"), true, TEXT("Show spheres on enemyies finding to attack"), ECVF_Cheat);

USCombatComponent::USCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

AActor* USCombatComponent::GetEnemyOnDirection(const FVector Direction) const
{
	TArray<FOverlapResult> OverlapResults;
	const FVector StartSweep = GetOwner()->GetActorLocation();

	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(EnemyObjectType);

	FCollisionShape SphereShape;
	SphereShape.SetSphere(MaxDistanceToFindEnemy);

	if(GetWorld()->OverlapMultiByObjectType(OverlapResults, StartSweep,
		FQuat::Identity,QueryParams, SphereShape))
	{
		float BestDotResult = -10.f;
		AActor* SelectedEnemy = nullptr;
		for (FOverlapResult Overlap : OverlapResults)
		{
			AActor* OverlappedActor = Overlap.GetActor();
			if(OverlappedActor == GetOwner())
			{
				continue;
			}
			
			FVector HitDirection = OverlappedActor->GetActorLocation() - StartSweep;
			const float CurrentDot = FVector::DotProduct(Direction, HitDirection.GetSafeNormal2D());
			if(CurrentDot > 0.0f && CurrentDot > BestDotResult)
			{
				BestDotResult = CurrentDot;
				SelectedEnemy = OverlappedActor;
			}
			
			if(CVar_DebugCombatFinding.GetValueOnGameThread())
			{
				DrawDebugSphere(GetWorld(), OverlappedActor->GetActorLocation() + FVector::UpVector * 90.0f, 30.0f,
					32, FColor::Yellow, false, 1.f);
			}
		}

		if(SelectedEnemy && CVar_DebugCombatFinding.GetValueOnGameThread())
		{
			DrawDebugSphere(GetWorld(), SelectedEnemy->GetActorLocation() + FVector::UpVector * 90.0f, 30.0f,
					32, FColor::Green, false, 1.f, 0, 3.f);
		}

		return SelectedEnemy;
	}
	
	return nullptr;
}

void USCombatComponent::SetWarpTarget(const FVector Location, const FRotator Rotation)
{
	WarpTargetLocation = Location;
	WarpTargetRotation = Rotation;
	UpdateWarpTarget.Broadcast();
}

void USCombatComponent::SetCurrentTarget(AActor* Target)
{
	CurrentTarget = Target;
}

void USCombatComponent::ApplyDamage()
{
	if(CurrentTarget == nullptr)
	{
		return;
	}

 	FHitResult HitResult {};
	USkeletalMeshComponent* SkeletalMesh = CurrentTarget->GetComponentByClass<USkeletalMeshComponent>();
	HitResult.Component = SkeletalMesh;
	HitResult.BoneName = FName("spine_01");

	const ASGameMode* GM = GetWorld()->GetAuthGameMode<ASGameMode>();
	const float DamageAmount = GM && GM->IsLastEnemyRemaining() ? 10000.0f : 30.0f;
	USGameplayFunctionLibrary::ApplyDirectionalDamage(GetOwner(), CurrentTarget, DamageAmount, HitResult);
}

AActor* USCombatComponent::GetCurrentTarget() const
{
	return CurrentTarget;
}

