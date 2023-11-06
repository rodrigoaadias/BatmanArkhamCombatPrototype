// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacterMovementComponent.h"

#include "AbilitySystem/SAbilityComponent.h"
#include "GameFramework/Character.h"

USCharacterMovementComponent::USCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	FallingRotationRate = 10.0f;
}

// Called when the game starts
void USCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	AbilityComponent = CharacterOwner->GetComponentByClass<USAbilityComponent>();
}

void USCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_Climbing:
		PhysClimbing(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Custom Movement Mode"));
	}
}

float USCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if(MovementMode != MOVE_Custom)
	{
		return Super::GetMaxBrakingDeceleration();
	}

	switch (CustomMovementMode)
	{
	case CMOVE_Climbing:
		return BrakingDecelerationFlying;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid custom movement mode"));
		return -1.f;
	}
}

void USCharacterMovementComponent::PhysicsRotation(float DeltaTime)
{
	if(IsCustomMovementMode(CMOVE_Climbing) || MovementMode == MOVE_Falling)
	{
		return;
	}

	Super::PhysicsRotation(DeltaTime);
}

bool USCharacterMovementComponent::DoJump(bool bReplayingMoves)
{
	const bool DidJump = Super::DoJump(bReplayingMoves);
	if(DidJump && Acceleration.GetSafeNormal2D().SquaredLength() > 1.0f)
	{
		MoveUpdatedComponent(FVector::ZeroVector, Velocity.GetSafeNormal2D().Rotation(), false);
	}

	return DidJump;
}

// Called every frame
void USCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool USCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode CustomMovement) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == CustomMovement; 
}

void USCharacterMovementComponent::PhysClimbing(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}	
	
	if(AbilityComponent->ActiveTags.HasTag(InterpolatingTag) || AbilityComponent->ActiveTags.HasAny(TagsWithoutRootMotionInClimbing))
	{
		Velocity = FVector::ZeroVector;
		return;
	}

	RestorePreAdditiveRootMotionVelocity();
	
	if( !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() )
	{
		if( bCheatFlying && Acceleration.IsZero() )
		{
			Velocity = FVector::ZeroVector;
		}
		const float Friction = 2.0f;
		CalcVelocity(deltaTime, Friction, true, GetMaxBrakingDeceleration());
	}	
		
	ApplyRootMotionToVelocity(deltaTime);

	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

	if (Hit.Time < 1.f)
	{
		const FVector GravDir = FVector(0.f, 0.f, -1.f);
		const FVector VelDir = Velocity.GetSafeNormal();
		const float UpDown = GravDir | VelDir;

		bool bSteppedUp = false;
		if ((FMath::Abs(Hit.ImpactNormal.Z) < 0.2f) && (UpDown < 0.5f) && (UpDown > -0.2f) && CanStepUp(Hit))
		{
			float stepZ = UpdatedComponent->GetComponentLocation().Z;
			bSteppedUp = StepUp(GravDir, Adjusted * (1.f - Hit.Time), Hit);
			if (bSteppedUp)
			{
				OldLocation.Z = UpdatedComponent->GetComponentLocation().Z + (OldLocation.Z - stepZ);
			}
		}

		if (!bSteppedUp)
		{
			//adjust and try again
			HandleImpact(Hit, deltaTime, Adjusted);
			SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
		}
	}

	if( !bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() )
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}
}