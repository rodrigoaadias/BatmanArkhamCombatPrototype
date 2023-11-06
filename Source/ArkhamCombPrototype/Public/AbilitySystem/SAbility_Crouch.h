// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAbility.h"
#include "SAbility_Crouch.generated.h"

/**
 * 
 */
UCLASS()
class  USAbility_Crouch : public USAbility
{
	GENERATED_BODY()

public:

	virtual bool CanStart(AActor* InstigatorActor) override;
	virtual void StartAbility_Implementation(AActor* InstigatorActor) override;
	virtual void StopAbility_Implementation(AActor* InstigatorActor) override;
};
