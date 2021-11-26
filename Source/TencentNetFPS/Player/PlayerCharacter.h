// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "TencentNetFPS/Weapon/Projectile.h"
#include "TencentNetFPS/Weapon/Weapon.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TENCENTNETFPS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	float PlayerHealth;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FPSSkeletalMeshComp;
	
	UPROPERTY()
	USkeletalMeshComponent* MeshComp;
	
	UPROPERTY()
	UPawnMovementComponent* MovementComp;

	UPROPERTY(EditAnywhere,Category="Init")
	float Speed;

	UPROPERTY()
	TSubclassOf<AProjectile> BaseProjectileClass;

	UPROPERTY()
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY()
	TSubclassOf<AWeapon> FPSWeaponClass;

	UPROPERTY()
	AWeapon* FPSMainWeapon;

	UPROPERTY()
	AWeapon* TPSWeapon;

	UPROPERTY()
	UAnimMontage* PlayerDeathMontage;

	UPROPERTY()
	UAnimMontage* PlayerFireMontage;

	UPROPERTY()
	USoundCue* ShootSound;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void PlayerCrouch();
	void PlayerUnCrouch();
	void GetWeapon();
	void Fire();
	void Die();

	UFUNCTION(NetMulticast,Reliable,Category="Weapon")
	void NetMulticastDestroyWeapon();
	
	UFUNCTION(Server,Reliable,Category="Fire")
	void ServerFire(FVector SpawnLocation,FRotator SpawnRotation);

	UFUNCTION(NetMulticast,Reliable,Category="Fire")
	void NetMulticastPlayFireAnimMontage();
	
	UFUNCTION(Server,Reliable,Category="Die")
	void ServerDie();

	//蓝图动画通知调用
	UFUNCTION(BlueprintCallable,Server,Reliable,Category="Die")
	void ServerDestroySelf();
	
	UFUNCTION(NetMulticast,Reliable,Category="Die")
	void NetMulticastPlayDieAnimMontage();



public:
	int PlayerID;
	
	UPROPERTY()
	bool bIsDead;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayerTakeDamage(float Value);
	int GetHealth() const {return PlayerHealth;}
};
