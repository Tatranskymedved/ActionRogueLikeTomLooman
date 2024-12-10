// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"

#include "DrawDebugHelpers.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "SInteractionComponent.h"
#include "Animation/AnimMontage.h"
#include <Kismet/KismetMathLibrary.h>
#include <SProjectileSpawner.h>

// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	ProjectileSpawner = CreateDefaultSubobject<ASProjectileSpawner>("ProjectileSpawner");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//AddMovementInput(this->GetActorForwardVector(), Value);
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// axis X = Forward (Red)
	// axis Y = Right (Green)
	// axis Z = Up (Blue)

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", EInputEvent::IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", EInputEvent::IE_Pressed, this, &ASCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", EInputEvent::IE_Pressed, this, &ASCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Pressed, this, &ASCharacter::Dash);
}

void ASCharacter::PrimaryAttack()
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No primary projectile was set for player."));
		return;
	}

	PlayAnimMontage(AttackAnim);

	//Delayed spawn of projectile so that animation can play
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector CameraWorldLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

	AActor* ProjectileActor = ProjectileSpawner->SpawnProjectileWithLineTrace(ProjectileClass, GetWorld(), this, HandLocation, CameraWorldLocation, GetControlRotation().Vector());

	if (ProjectileActor)
	{
		MoveIgnoreActorAdd(ProjectileActor);
	}
}

void ASCharacter::SecondaryAttack()
{
	if (!SecondaryProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No secondary projectile was set for player."));
		return;
	}
	PlayAnimMontage(AttackAnim);

	//Delayed spawn of projectile so that animation can play
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::SecondaryAttack_TimeElapsed, 0.2f);
}
void ASCharacter::SecondaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector CameraWorldLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

	AActor* ProjectileActor = ProjectileSpawner->SpawnProjectileWithLineTrace(SecondaryProjectileClass, GetWorld(), this, HandLocation, CameraWorldLocation, GetControlRotation().Vector());

	if (ProjectileActor)
	{
		MoveIgnoreActorAdd(ProjectileActor);
	}
}

void ASCharacter::PrimaryInteract()
{
	if (ensure(InteractionComp))
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::Dash()
{
	if (!DashProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No dash projectile was set for player."));
		return;
	}
	PlayAnimMontage(AttackAnim);

	//Delayed spawn of projectile so that animation can play
	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, 0.2f);
}
void ASCharacter::Dash_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector CameraWorldLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

	AActor* ProjectileActor = ProjectileSpawner->SpawnProjectileWithLineTrace(DashProjectileClass, GetWorld(), this, HandLocation, CameraWorldLocation, GetControlRotation().Vector());

	if (ProjectileActor)
	{
		MoveIgnoreActorAdd(ProjectileActor);
	}
}