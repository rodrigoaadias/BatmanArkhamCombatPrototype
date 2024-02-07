// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterAction, ASAICharacter*, CharacterDied);

class USCharacterDamageCauser;
class USphereComponent;
class USAbilityComponent;
class USAttributeComponent;

UCLASS()
class ARKHAMCOMBPROTOTYPE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASAICharacter();

protected:

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAbilityComponent> AbilityComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FName FrontHitAbilityName{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FName BackHitAbilityName{};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USCharacterDamageCauser> DamageCauserComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> DamageTrigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	FName DamageSocketName{"Punch_Socket"};

	void Die();

	UFUNCTION()
	void HandleHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

private:
	bool bIsDead{false};

public:
	FORCEINLINE bool IsDead() const {return bIsDead;}

	UPROPERTY(BlueprintAssignable)
	FCharacterAction OnCharacterDied;

	void SetMinimumHealth();
};
