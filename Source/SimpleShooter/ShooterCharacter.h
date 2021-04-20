// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
		bool IsDead() const;

	UFUNCTION(BlueprintPure)
		float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
		FString GetAmmo() const;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Shoot();
	void Reload();

	UFUNCTION(BlueprintCallable)
	void SetRPressed(bool R);

	UFUNCTION(BlueprintCallable)
		void GetRiffle();
private:

	bool RPressed;
	UPROPERTY(EditAnywhere)
		float RotateRate = 70.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* Arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	void MoveForward(float AxisValue);
	void LookUpDown(float AxisValue);
	void MoveSideways(float AxisValue);
	void LookRightLeft(float AxisValue);
	void LookUpDownRate(float AxisValue);
	void LookRightLeftRate(float AxisValue);

	
	UFUNCTION()
	void StartCrouch();
	UFUNCTION()
	void EndCrouch();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGun> GunClass;
	UPROPERTY()
		AGun* Gun;
	UPROPERTY()
		UAnimSequence* Anim;
	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere)
		float Health;
	UPROPERTY(EditAnywhere)
		bool HideLauncher;
	UPROPERTY(EditAnywhere)
		bool HideRiffle;
};
