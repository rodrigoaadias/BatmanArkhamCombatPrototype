#include "SCombatComponent.h"

USCombatComponent::USCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

AActor* USCombatComponent::GetEnemyOnDirection(const FVector Direction) const
{
	const FVector Direction2D = Direction.GetSafeNormal2D();

	TArray<FOverlapResult> OverlapResults;
	const FVector StartSweep = GetOwner()->GetActorLocation();

	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(EnemyObjectType);

	FCollisionShape SphereShape;
	SphereShape.SetSphere(MaxDistanceToFindEnemy);

	if(GetWorld()->OverlapMultiByObjectType(OverlapResults, StartSweep,
		FQuat::Identity,	QueryParams, SphereShape))
	{
		float BestDotResult = -10.f;
		AActor* SelectedEnemy = nullptr;
		for (FOverlapResult Overlap : OverlapResults)
		{
			AActor* OverlappedActor = Overlap.GetActor();
			FVector HitDirection = OverlappedActor->GetActorLocation() - StartSweep;
			const float CurrentDot = FVector::DotProduct(Direction2D, HitDirection.GetSafeNormal2D());
			if(CurrentDot > 0.2f && CurrentDot > BestDotResult)
			{
				BestDotResult = CurrentDot;
				SelectedEnemy = OverlappedActor;
			}
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

