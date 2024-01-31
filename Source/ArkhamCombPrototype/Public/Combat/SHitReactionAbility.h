#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/SAbility.h"
#include "SHitReactionAbility.generated.h"

UCLASS()
class ARKHAMCOMBPROTOTYPE_API USHitReactionAbility : public USAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	UAnimMontage* HitReactionMontage;

	virtual void StartAbility_Implementation(AActor* InstigatorActor) override;
	virtual void StopAbility_Implementation(AActor* InstigatorActor) override;

	FTimerHandle Hit_TimerHandle;	
};
