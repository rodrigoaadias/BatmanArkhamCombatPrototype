// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpPressed);

class USClimbingComponent;
class USAbilityComponent;
class USpringArmComponent;
class UCameraComponent;
class USCharacterMovementComponent;

UCLASS()
class ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAbilityComponent> AbilityComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag JumpAbilityTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag CrouchAbilityTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag InAirTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	TObjectPtr<USCharacterMovementComponent> CharacterMovementComponent;

	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;

	void StartCrouch();
	void StopCrouch();

	void StartJump();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Input")
	FVector GetInputDirection() const;

	UPROPERTY(BlueprintAssignable)
	FJumpPressed OnJumpPressed;

private:
	bool bWasFallingLastFrame;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
};
