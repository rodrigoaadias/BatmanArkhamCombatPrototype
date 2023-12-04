#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "ArkhamCombPrototype/ArkhamCombPrototype.h"

ASAICharacter::ASAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attribute Component"));
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::HandleHealthChanged);
}

void ASAICharacter::HandleHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp,
	float NewHealth, float Delta)
{
	FVector Direction = (GetActorLocation() - InstigatorActor->GetActorLocation()).GetSafeNormal2D();
	float ForwardDot = FVector::DotProduct(Direction, GetActorForwardVector());

	if(ForwardDot > 0.0f)
	{
		PlayAnimMontage(BackHitMontage);		
	}
	else
	{
		PlayAnimMontage(FrontHitMontage);		
	}	
}
