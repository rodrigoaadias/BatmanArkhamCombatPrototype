// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SAbility_Jump.h"
#include "SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

USAbility_Jump::USAbility_Jump()
{
	bCanTick = true;
}

void USAbility_Jump::StartAbility_Implementation(AActor* InstigatorActor)
{
	Super::StartAbility_Implementation(InstigatorActor);
	GetCharacterOwner()->Jump();
}

void USAbility_Jump::Tick(float DeltaTime)
{
	if(!GetCharacterOwner()->GetCharacterMovement()->IsFalling())
	{
		StopAbility(GetCharacterOwner());
	}
}
