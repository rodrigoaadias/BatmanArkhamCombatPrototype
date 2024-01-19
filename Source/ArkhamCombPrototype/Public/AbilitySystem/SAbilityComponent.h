// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SAbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityChanged);

inline TAutoConsoleVariable<bool> CVarDebugAbilityStartStop(TEXT("su.Abilitysystem.ShowStartStopAbilities"), false, TEXT("Show messages when an ability enters ou stops"), ECVF_Cheat);
inline TAutoConsoleVariable<bool> CVarDebugAbilityErrors(TEXT("su.Abilitysystem.ShowErrors"), false, TEXT("Show messages when an ability can not enter"), ECVF_Cheat);
inline TAutoConsoleVariable<bool> CVarDebugActiveTags(TEXT("su.Abilitysystem.ShowActiveTags"), false, TEXT("Show current active tags"), ECVF_Cheat);

class USAbility;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class  USAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USAbilityComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	FGameplayTagContainer ActiveTags;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TArray<TSubclassOf<USAbility>> DefaultAbilities;

	TArray<USAbility*> Abilities;
	
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void AddAbility(TSubclassOf<USAbility> AbilityClass, USAbility*& Result);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool StartAbilityByTagName(AActor* InstigatorActor, FName AbilityTagName);
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool StopAbilityByTagName(AActor* InstigatorActor, FName AbilityTagName);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void StopAbilitiesWithTag(AActor* Instigator, FGameplayTagContainer TagsToCancel);

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilityChanged OnAbilityChanged;
};
