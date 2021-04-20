// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Arm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Arm);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	HideRiffle = false;
	HideLauncher = true;

	RPressed = false;
	Health = MaxHealth;
	GetRiffle();
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveSideways"), this, &AShooterCharacter::MoveSideways);
	PlayerInputComponent->BindAxis(TEXT("LookUpDown"), this, &AShooterCharacter::LookUpDown);
	PlayerInputComponent->BindAxis(TEXT("LookRightLeft"), this, &AShooterCharacter::LookRightLeft);
	PlayerInputComponent->BindAxis(TEXT("LookUpDownRate"), this, &AShooterCharacter::LookUpDownRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightLeftRate"), this, &AShooterCharacter::LookRightLeftRate);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AShooterCharacter::StartCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &AShooterCharacter::EndCrouch);

	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Released, this, &AShooterCharacter::Reload);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::LookUpDown(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AShooterCharacter::MoveSideways(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookRightLeft(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AShooterCharacter::StartCrouch()
{
	Crouch();
}
					
void AShooterCharacter::EndCrouch()
{
	UnCrouch();
}

void AShooterCharacter::LookUpDownRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotateRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightLeftRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotateRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	if (RPressed == false)
	{
		Gun->Shoot();
	}
	else
	{
		return;
	}
}

void AShooterCharacter::Reload()
{
	Gun->Reload();
   
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageApplied = FMath::Min(Health, DamageApplied);
	Health -= DamageApplied;
	UE_LOG(LogTemp, Warning, TEXT("Health is: %f"), Health);
	
	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageApplied;
}

bool AShooterCharacter::IsDead() const
{
	bool Dead = false;
	if (Health <= 0)
	{
		Dead = true;
	}
	return Dead;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

FString AShooterCharacter::GetAmmo() const
{
	return FString::Printf(TEXT("%i / %i"), Gun->GetAmmo(), Gun->GetBackAmmo());
}

void AShooterCharacter::SetRPressed(bool R)
{
	RPressed = R;
}

void AShooterCharacter::GetRiffle()
{
	HideLauncher = true;
	UE_LOG(LogTemp, Warning, TEXT("I took the riffle"));
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

