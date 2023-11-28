#include "SAttributeComponent.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global damage multiplier to apply in Attributes"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return FromActor->FindComponentByClass<USAttributeComponent>();
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* FromActor)
{
	USAttributeComponent* AttributeComponent = GetAttributes(FromActor);
	if(AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}

	return false;
}

void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}


bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if(Delta < 0.0f)
	{
		const float Multiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= Multiplier;
	}
	
	const float OldHealth = Health;
	const float LocalHealth = FMath::Clamp(Health + Delta, 0, MaxHealth);
	const float ActualDelta = LocalHealth - OldHealth;
	Health += ActualDelta;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	return ActualDelta != 0;
}


bool USAttributeComponent::IsFullHealth() const
{
	return Health >= MaxHealth;
}