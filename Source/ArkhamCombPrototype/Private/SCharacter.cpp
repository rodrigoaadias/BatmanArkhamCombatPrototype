// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"

#include "SCharacterMovementComponent.h"
#include "AbilitySystem/SAbilityComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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
	AbilityComponent->StartAbilityByTagName(this, CrouchAbilityTag);
}

void ASCharacter::StopCrouch()
{
	AbilityComponent->StopAbilityByTagName(this, CrouchAbilityTag);
}

void ASCharacter::StartJump()
{
	AbilityComponent->StartAbilityByTagName(this, JumpAbilityTag);
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
	EnhancedInputComponent->BindAction(Input_LookMouse, ETriggerEvent::Triggered, this, &ASCharacter::LookMouse);
	
	// PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::StartJump);
	// PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::StopJumping);
	//
	// PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::StartCrouch);
	// PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::StopCrouch);
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

void ASCharacter::LookMouse(const FInputActionValue& Value)
{	
	const FVector2D Input = Value.Get<FVector2D>();

	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}

void ASCharacter::LookStick(const FInputActionValue& Value)
{	
	const FVector2D Input = Value.Get<FVector2D>();

	AddControllerYawInput(Input.X * LookStickSensitivity.X * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(Input.Y * LookStickSensitivity.Y * GetWorld()->GetDeltaSeconds());
}
