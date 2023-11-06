// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SAbility.h"
#include "SCharacter.h"
#include "AbilitySystem/SAbilityComponent.h"

USAbility::USAbility()
{
	bCanTick = false;
}

void USAbility::Setup(AActor* Owner)
{
	CharacterOwner = Cast<ASCharacter>(Owner);
}

bool USAbility::CanStart(AActor* InstigatorActor)
{
	if(IsRunning())
	{
		if(CVarDebugAbilityErrors.GetValueOnGameThread())
		{
			const FString FailMsg = FString::Printf(TEXT("Can't start %s because it's already Running"), *GetNameSafe(this));
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FailMsg);
		}
		return false;
	}

	if(GetOwningComponent()->ActiveTags.HasAny(BlockedTags))
	{
		return false;
	}

	return GetOwningComponent()->ActiveTags.HasAll(RequiredTags);
}

void USAbility::TickAbility(float DeltaTime)
{
}

void USAbility::StartAbility_Implementation(AActor* InstigatorActor)
{
	if(CVarDebugAbilityStartStop.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Running: %s"), * AbilityName.ToString());
	}

	GetOwningComponent()->StopAbilitiesWithTag(InstigatorActor, CancelAbilitiesWithTag);

	bIsRunning = true;
	GetOwningComponent()->ActiveTags.AppendTags(GrantsTags);
	StartTime = GetWorld()->TimeSeconds;
}

void USAbility::StopAbility_Implementation(AActor* InstigatorActor)
{
	if(CVarDebugAbilityStartStop.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), * AbilityName.ToString());
	}

	ensureAlways(bIsRunning);
	
	bIsRunning = false;
	GetOwningComponent()->ActiveTags.RemoveTags(GrantsTags);
	StopTime = GetWorld()->TimeSeconds;
}

USAbilityComponent* USAbility::GetOwningComponent() const
{
	return Cast<USAbilityComponent>(GetOuter());
}

UWorld* USAbility::GetWorld() const
{
	const UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if(Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}

FGameplayTag USAbility::GetAbilityNameTag() const
{
	return AbilityName;
}

bool USAbility::IsRunning() const
{
	return  bIsRunning;
}

ASCharacter* USAbility::GetCharacterOwner() const
{
	return CharacterOwner;
}

FGameplayTagContainer USAbility::GetAbilityTags() const
{
	return GrantsTags;
}

void USAbility::Tick(float DeltaTime)
{
	if (LastTickFrame == GFrameCounter)
	{
		return;
	}

	LastTickFrame = GFrameCounter;
	TickAbility(DeltaTime);
}

bool USAbility::IsTickable() const
{
	return bCanTick && bIsRunning;
}

bool USAbility::IsTickableInEditor() const
{
	return false;
}

bool USAbility::IsTickableWhenPaused() const
{
	return false;
}

ETickableTickType USAbility::GetTickableTickType() const
{
	return ETickableTickType::Conditional;
}

TStatId USAbility::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(MyTickableClass, STATGROUP_Tickables);
}