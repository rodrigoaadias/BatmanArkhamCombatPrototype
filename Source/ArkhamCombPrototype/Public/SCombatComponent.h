// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartCombatWarping);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARKHAMCOMBPROTOTYPE_API USCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USCombatComponent();

protected:

	TObjectPtr<AActor> CurrentTarget{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Query")
	TEnumAsByte<ECollisionChannel> EnemyObjectType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Query")
	float MaxDistanceToFindEnemy = 500.0f;

public:
	AActor* GetEnemyOnDirection(const FVector Direction) const;

	UPROPERTY(BlueprintReadOnly)
	FVector WarpTargetLocation;

	UPROPERTY(BlueprintReadOnly)
	FRotator WarpTargetRotation;

	void SetWarpTarget(const FVector Location, const FRotator Rotation);

	UPROPERTY(BlueprintAssignable)
	FOnStartCombatWarping UpdateWarpTarget;

	void SetCurrentTarget(AActor* Target);

	UFUNCTION(BlueprintCallable)
	void ApplyDamage();
};
