// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"
// Sets default values
#define OUT

AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::Shoot()
{
	if (Ammo > 0)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
		FHitResult Hit;
		FVector ShotDirection;
		bool bHitResult = GunTrace(Hit, ShotDirection);
		if (bHitResult)
		{
			//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFlash, Hit.Location, ShotDirection.Rotation());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), MuzzleSoundAtLocation, Hit.Location);
			AActor* HitActor = Hit.GetActor();
			if (HitActor != nullptr)
			{
				FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
				AController* OwnerController = GetOwnerController();
				HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			}
		}
		Ammo--;
	}
	else
	{
		UGameplayStatics::SpawnSoundAttached(OutOfAmmoSound, Mesh, TEXT("MuzzleFlashSocket"));
	}
}

void AGun::Reload()
{
	int NeededAmmo = 0;
	if (BackAmmo <= 0)
	{
		return;
	}

	if (Ammo != 0)
	{
		 NeededAmmo = 15 - Ammo;
		 if (BackAmmo < NeededAmmo)
		 {
			 Ammo += BackAmmo;
			 BackAmmo -= BackAmmo;
		 }
		 else
		 {
			 Ammo += NeededAmmo;
			 BackAmmo -= NeededAmmo;
		 }
	}
	else
	{
		Ammo += 15;
		BackAmmo -= 15;
	}
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(OUT Location, OUT Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + MaxRange * Rotation.Vector();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(OUT Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}

int AGun::GetAmmo() const
{
	return Ammo;
}
int AGun::GetBackAmmo() const
{
	return BackAmmo;
}