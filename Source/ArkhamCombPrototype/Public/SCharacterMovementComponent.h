// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SCharacterMovementComponent.generated.h"

class USAbilityComponent;

UENUM(BlueprintType)
enum ECustomMovementMode : uint8
{
	CMOVE_None		UMETA(Hidden),
	CMOVE_Climbing	UMETA(DisplayName = "Climbing"),
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class  USCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	USCharacterMovementComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Movement")
	FGameplayTag InterpolatingTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Movement")
	FGameplayTagContainer TagsWithoutRootMotionInClimbing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Falling")
	float FallingRotationRate;

	TObjectPtr<USAbilityComponent> AbilityComponent;
	
	virtual void BeginPlay() override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual float GetMaxBrakingDeceleration() const override;
	virtual void PhysicsRotation(float DeltaTime) override;
	virtual bool DoJump(bool bReplayingMoves) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	bool IsCustomMovementMode(ECustomMovementMode CustomMovement) const;

	// CLIMBING
private:
	void PhysClimbing(float deltaTime, int32 Iterations);
};
