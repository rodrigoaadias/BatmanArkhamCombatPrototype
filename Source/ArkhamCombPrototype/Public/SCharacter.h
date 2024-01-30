// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class USCombatComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpPressed);

class USClimbingComponent;
class USAbilityComponent;
class USpringArmComponent;
class UCameraComponent;
class USCharacterMovementComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FVector2D LookStickSensitivity;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Attack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_CounterAttack;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAbilityComponent> AbilityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAttributeComponent> AttributeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag InAirTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USCombatComponent> CombatComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	TObjectPtr<USCharacterMovementComponent> CharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UArrowComponent* CombatArrowComp;

	UFUNCTION()
	void HandleHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

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
	FVector InputDirection {FVector::ZeroVector};

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
};
