// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAbility.generated.h"

class USAbilityComponent;
class ASCharacter;
/**
 * 
 */
UCLASS(Blueprintable)
class  USAbility : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	USAbility();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag AbilityName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer RequiredTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer CancelAbilitiesWithTag;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	float StartTime;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	float StopTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	bool bCanTick;

private:
	TObjectPtr<ASCharacter> CharacterOwner;

	// track last frame to avoid tick more than once in a frame
	uint32 LastTickFrame = INDEX_NONE;

	bool bIsRunning;

public:

	UFUNCTION(BlueprintCallable)
	virtual void Setup(AActor* Owner);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual bool CanStart(AActor* InstigatorActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	void StartAbility(AActor* InstigatorActor);

	virtual void TickAbility(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
	void StopAbility(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	USAbilityComponent* GetOwningComponent() const;

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetAbilityNameTag() const;

	UFUNCTION(BlueprintCallable)
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable)
	ASCharacter* GetCharacterOwner() const;

	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetAbilityTags() const;

private:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual TStatId GetStatId() const override;
};
