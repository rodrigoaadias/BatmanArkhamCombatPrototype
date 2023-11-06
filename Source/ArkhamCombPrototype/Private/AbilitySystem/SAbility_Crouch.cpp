// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SAbility_Crouch.h"
#include "SCharacter.h"

bool USAbility_Crouch::CanStart(AActor* InstigatorActor)
{
	if(!GetCharacterOwner()->CanCrouch())
	{
		return false;
	}
	
	return Super::CanStart(InstigatorActor);
}

void USAbility_Crouch::StartAbility_Implementation(AActor* InstigatorActor)
{
	Super::StartAbility_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if(Character)
	{
		Character->Crouch();
	}
}

void USAbility_Crouch::StopAbility_Implementation(AActor* InstigatorActor)
{
	Super::StopAbility_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if(Character)
	{
		Character->UnCrouch();
	}
}
