// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARKHAMCOMBPROTOTYPE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USAttributeComponent();

	UFUNCTION(BlueprintCallable)
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable)
	static bool IsActorAlive(AActor* FromActor);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth;
	
	UPROPERTY(BlueprintReadOnly, Category = "Atrributes")
	float Health;

	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const {return Health;}

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const {return MaxHealth;}

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;
};
