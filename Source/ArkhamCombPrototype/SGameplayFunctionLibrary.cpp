#include "SGameplayFunctionLibrary.h"
#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(TargetActor);
	if(AttributeComponent)
	{
		return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}

	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& Hit)
{
	if(ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComponent = Hit.GetComponent();
		if(HitComponent && HitComponent->IsSimulatingPhysics(Hit.BoneName))
		{
			FVector Direction = Hit.TraceEnd - Hit.TraceStart;
			Direction.Normalize();
			HitComponent->AddImpulseAtLocation(Direction * 300000.f, Hit.Location, Hit.BoneName);
		}
		return true;
	}

	return false;
}