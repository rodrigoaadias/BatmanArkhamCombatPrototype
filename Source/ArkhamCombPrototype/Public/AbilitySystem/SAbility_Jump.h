// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAbility.h"
#include "SAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class  USAbility_Jump : public USAbility
{
	GENERATED_BODY()

public:

	USAbility_Jump();
	
	virtual void StartAbility_Implementation(AActor* InstigatorActor) override;
	virtual void Tick(float DeltaTime) override;
};
