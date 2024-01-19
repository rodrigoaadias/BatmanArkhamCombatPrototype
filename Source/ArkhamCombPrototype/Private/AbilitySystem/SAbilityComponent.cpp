// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SAbilityComponent.h"
#include "AbilitySystem/SAbility.h"

USAbilityComponent::USAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAbility> Ability : DefaultAbilities)
	{
		USAbility* Result;
		AddAbility(Ability, Result);
	}
}

void USAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CVarDebugActiveTags.GetValueOnGameThread())
	{
		FString ActiveTagsMsg = FString::Printf(TEXT("Active Tags: %s"), *ActiveTags.ToStringSimple());
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, ActiveTagsMsg);
	}
}

void USAbilityComponent::AddAbility(TSubclassOf<USAbility> AbilityClass, USAbility*& Result)
{
	Result = nullptr;
	if(!ensure(AbilityClass))
	{
		return;
	}

	USAbility* NewAbility = NewObject<USAbility>(this, AbilityClass, "New Ability", RF_Load);
	if(ensure(NewAbility))
	{
		NewAbility->Setup(GetOwner());
		Abilities.Add(NewAbility);
		Result = NewAbility;
	}
}

bool USAbilityComponent::StartAbilityByTagName(AActor* InstigatorActor, FName AbilityTagName)
{
	for(USAbility* Ability : Abilities)
	{
		if(ensure(Ability) && Ability->GetAbilityNameTag() == AbilityTagName)
		{
			if(!Ability->CanStart(InstigatorActor))
			{
				if(CVarDebugAbilityErrors.GetValueOnGameThread())
				{
					FString FailedMsg = FString::Printf(TEXT("Failed to start ability: %s"), *AbilityTagName.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				}
				return false;
			}

			Ability->StartAbility(InstigatorActor);
			OnAbilityChanged.Broadcast();
			return true;
		}
	}

	return false;
}

bool USAbilityComponent::StopAbilityByTagName(AActor* InstigatorActor, FName AbilityTagName)
{
	for(USAbility* Ability : Abilities)
	{
		if(Ability && Ability->GetAbilityNameTag() == AbilityTagName)
		{
			if(!Ability->IsRunning())
			{
				return false;
			}

			Ability->StopAbility(InstigatorActor);
			OnAbilityChanged.Broadcast();
			return true;
		}
	}

	return false;
}

void USAbilityComponent::StopAbilitiesWithTag(AActor* Instigator, FGameplayTagContainer TagsToCancel)
{
	for (const auto Ability : Abilities)
	{
		if(!Ability->IsRunning())
		{
			continue;
		}

		if(Ability->GetAbilityTags().HasAny(TagsToCancel))
		{
			Ability->StopAbility(Instigator);
		}
	}
}

