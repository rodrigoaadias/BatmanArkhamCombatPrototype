#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "AbilitySystem/SAbilityComponent.h"
#include "Combat/SCharacterDamageCauser.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASAICharacter::ASAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attribute Component"));
	AbilityComponent = CreateDefaultSubobject<USAbilityComponent>(TEXT("Ability Component"));
	DamageCauserComp = CreateDefaultSubobject<USCharacterDamageCauser>(TEXT("Damage Causer Component"));
	DamageTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Damage Trigger"));
	DamageTrigger->SetupAttachment(GetMesh(), DamageSocketName);
	DamageTrigger->SetCollisionProfileName("Trigger");
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::HandleHealthChanged);
	DamageCauserComp->SetTriggerReference(DamageTrigger);
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
	bIsDead = true;
	OnCharacterDied.Broadcast(this);
	OnCharacterDied.Clear();
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
		AbilityComponent->StartAbilityByTagName(InstigatorActor, BackHitAbilityName);	
	}
	else
	{
		AbilityComponent->StartAbilityByTagName(InstigatorActor, FrontHitAbilityName);	
	}
}

void ASAICharacter::SetMinimumHealth()
{
	const float Delta = AttributeComponent->GetCurrentHealth() - 1;
	AttributeComponent->ApplyHealthChange(this, -Delta);
}
