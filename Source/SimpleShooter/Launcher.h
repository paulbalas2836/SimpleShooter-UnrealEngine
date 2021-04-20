// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Launcher.generated.h"

UCLASS()
class SIMPLESHOOTER_API ALauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALauncher();
	void Shoot();
	void Reload();
	int GetAmmo() const;
	int GetBackAmmo() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		USoundBase* OutOfAmmoSound;
	UPROPERTY(EditAnywhere)
		USoundBase* MuzzleSound;
	UPROPERTY(EditAnywhere)
		USoundBase* MuzzleSoundAtLocation;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
		UParticleSystem* HitFlash;
	UPROPERTY(EditAnywhere)
		float MaxRange = 1000;

	UPROPERTY(EditAnywhere)
		float Damage = 10;

	UPROPERTY(EditAnywhere)
		int Ammo = 15;

	UPROPERTY(EditAnywhere)
		int BackAmmo = 60;

	bool LauncherTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;
};
