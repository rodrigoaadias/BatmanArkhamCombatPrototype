#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "AbilitySystem/SAbilityComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASAICharacter::ASAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attribute Component"));
	AbilityComponent = CreateDefaultSubobject<USAbilityComponent>(TEXT("Ability Component"));
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::HandleHealthChanged);
}

void ASAICharacter::Die()
{
	// ragdoll
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	// Destroy
	SetLifeSpan(10.0f);
}

void ASAICharacter::HandleHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp,
                                        float NewHealth, float Delta)
{
	FVector Direction = (GetActorLocation() - InstigatorActor->GetActorLocation()).GetSafeNormal2D();
	float ForwardDot = FVector::DotProduct(Direction, GetActorForwardVector());

	if(NewHealth <= 0 && Delta < 0)
	{
		Die();
		return;
	}

	if(ForwardDot > 0.0f)
	{
		PlayAnimMontage(BackHitMontage);		
	}
	else
	{
		PlayAnimMontage(FrontHitMontage);		
	}	
}
