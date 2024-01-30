// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCharacterDamageCauser.generated.h"

class USphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARKHAMCOMBPROTOTYPE_API USCharacterDamageCauser : public UActorComponent
{
	GENERATED_BODY()

public:
	USCharacterDamageCauser();

protected:
	virtual void BeginPlay() override;

	TObjectPtr<USphereComponent> DamageTrigger{};

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount{30.0f};
public:
	void SetTriggerReference(TObjectPtr<USphereComponent> Trigger);
	UFUNCTION(BlueprintCallable)
	void EnableDamage();
	UFUNCTION(BlueprintCallable)
	void DisableDamage();
	void ApplyDamageTo(AActor* OtherActor) const;

private:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
