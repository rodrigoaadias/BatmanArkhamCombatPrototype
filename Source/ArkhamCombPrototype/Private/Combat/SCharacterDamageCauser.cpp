#include "Combat/SCharacterDamageCauser.h"
#include "ArkhamCombPrototype/SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"

USCharacterDamageCauser::USCharacterDamageCauser()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USCharacterDamageCauser::BeginPlay()
{
	Super::BeginPlay();
	DisableDamage();
	DamageTrigger->OnComponentBeginOverlap.AddDynamic(this, &USCharacterDamageCauser::HandleBeginOverlap);
}

void USCharacterDamageCauser::SetTriggerReference(TObjectPtr<USphereComponent> Trigger)
{
	DamageTrigger = Trigger;
}

void USCharacterDamageCauser::EnableDamage()
{
	check(DamageTrigger);
	DamageTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void USCharacterDamageCauser::DisableDamage()
{
	check(DamageTrigger);
	DamageTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
}

void USCharacterDamageCauser::ApplyDamageTo(AActor* OtherActor) const
{
	if(OtherActor == nullptr || DamageTrigger == nullptr || OtherActor == GetOwner())
	{
		return;
	}

	FHitResult HitResult {};
	USkeletalMeshComponent* SkeletalMesh = OtherActor->GetComponentByClass<USkeletalMeshComponent>();
	HitResult.Component = SkeletalMesh;
	HitResult.BoneName = FName("spine_01");
	USGameplayFunctionLibrary::ApplyDirectionalDamage(GetOwner(), OtherActor, 30.0f, HitResult);
}

void USCharacterDamageCauser::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ApplyDamageTo(OtherActor);
}
