// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"

#include "SCharacterMovementComponent.h"
#include "AbilitySystem/SAbilityComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SCombatComponent.h"
#include "Components/ArrowComponent.h"

ASCharacter::ASCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<USCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterMovementComponent = Cast<USCharacterMovementComponent>(GetCharacterMovement());

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Comp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	AbilityComponent = CreateDefaultSubobject<USAbilityComponent>(TEXT("Ability Comp"));
	CombatComponent = CreateDefaultSubobject<USCombatComponent>(TEXT("Combat Component"));
	CombatArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Combat Arrow Debug"));
	CombatArrowComp->SetupAttachment(RootComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);	
	AbilityComponent->ActiveTags.RemoveTag(InAirTag);
}

void ASCharacter::StartCrouch()
{
	AbilityComponent->StartAbilityByTagName(this, "Crouch");
}

void ASCharacter::StopCrouch()
{
	AbilityComponent->StopAbilityByTagName(this, "Crouch");
}

void ASCharacter::StartJump()
{
	AbilityComponent->StartAbilityByTagName(this, "Jump");
	OnJumpPressed.Broadcast();
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetCharacterMovement()->IsFalling() && !bWasFallingLastFrame)
	{
		AbilityComponent->ActiveTags.AddTag(InAirTag);
	}

	if(!GetCharacterMovement()->IsFalling() && AbilityComponent->ActiveTags.HasTag(InAirTag))
	{
		AbilityComponent->ActiveTags.RemoveTag(InAirTag);
	}

	bWasFallingLastFrame = GetCharacterMovement()->IsFalling();
	const FVector Direction = CharacterMovementComponent->GetCurrentAcceleration().GetSafeNormal();
	AActor* Enemy = CombatComponent->GetEnemyOnDirection(Direction);
	if(Enemy)
	{
		DrawDebugSphere(GetWorld(), Enemy->GetActorLocation(), 30.0f,
			32, FColor::Green);
		const FVector NewDirection = Enemy->GetActorLocation() - GetActorLocation();
		CombatArrowComp->SetWorldRotation(NewDirection.Rotation());
	}
	else
	{		
		CombatArrowComp->SetWorldRotation(Direction.Rotation());
	}

	CombatArrowComp->SetVisibility(CharacterMovementComponent->GetCurrentAcceleration().SquaredLength() > 0.5f);
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC->GetLocalPlayer(); 

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(DefaultMappingContext, 0);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ASCharacter::Move);
	EnhancedInputComponent->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ASCharacter::Look);
	EnhancedInputComponent->BindAction(Input_Attack, ETriggerEvent::Triggered, this, &ASCharacter::Punch);
}

FVector ASCharacter::GetInputDirection() const
{
	FVector Acceleration = GetCharacterMovement()->GetCurrentAcceleration();
	Acceleration.Normalize();

	return Acceleration;
}

void ASCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll= 0;
	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(ControlRot.Vector(), Input.Y);
	AddMovementInput(RightVector, Input.X);	
}

void ASCharacter::Look(const FInputActionValue& Value)
{	
	const FVector2D Input = Value.Get<FVector2D>();

	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}

void ASCharacter::Punch(const FInputActionValue& Value)
{
	AbilityComponent->StartAbilityByTagName(this, "Punch");
}
