// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARKHAMCOMBPROTOTYPE_API USCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USCombatComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Query")
	TEnumAsByte<ECollisionChannel> EnemyObjectType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Query")
	float MaxDistanceToFindEnemy = 500.0f;

public:
	AActor* GetEnemyOnDirection(const FVector Direction) const;
};
