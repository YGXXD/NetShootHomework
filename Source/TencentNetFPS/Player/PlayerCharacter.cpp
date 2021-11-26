// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework//PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TencentNetFPS/TencentNetFPSGameModeBase.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	
	Speed = 1.0f;
	PlayerHealth = 100;
	bIsDead = false;
	MeshComp = GetMesh();
	MeshComp->SetOwnerNoSee(true);
	
	MovementComp = ACharacter::GetMovementComponent();
	MovementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
	CameraComp->bUsePawnControlRotation = true;
	
	FPSSkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMeshComp"));
	FPSSkeletalMeshComp->SetupAttachment(CameraComp);
	FPSSkeletalMeshComp->SetOnlyOwnerSee(true);
	FPSSkeletalMeshComp->bCastDynamicShadow = false;
	FPSSkeletalMeshComp->CastShadow = false;
	FPSSkeletalMeshComp->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	FPSSkeletalMeshComp->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	
	static ConstructorHelpers::FClassFinder<AWeapon> WeaponBP(TEXT("/Game/BluePrint/BP_Weapon"));
	if(WeaponBP.Succeeded())
	{
		WeaponClass = WeaponBP.Class;
	}
	static ConstructorHelpers::FClassFinder<AWeapon> FPSWeaponBP(TEXT("/Game/BluePrint/BP_FPSWeapon"));
	if(FPSWeaponBP.Succeeded())
	{
		FPSWeaponClass = FPSWeaponBP.Class;
	}
	static  ConstructorHelpers::FClassFinder<AProjectile> ProjectileBP(TEXT("/Game/BluePrint/BP_Projectile"));
	if(ProjectileBP.Succeeded())
	{
		BaseProjectileClass = ProjectileBP.Class;
	}

	PlayerDeathMontage = LoadObject<UAnimMontage>(this,TEXT("AnimMontage'/Game/AnimStarterPack/Death_Montage.Death_Montage'"));
	PlayerFireMontage = LoadObject<UAnimMontage>(this,TEXT("AnimMontage'/Game/AnimFPSPack/Animations/FirstPerson_Fire_Montage.FirstPerson_Fire_Montage'"));
	ShootSound = LoadObject<USoundCue>(this,TEXT("SoundCue'/Game/Sound/Player_Projectile_Cue.Player_Projectile_Cue'"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWeapon();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward",this,&APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp",this,&APlayerCharacter::AddControllerPitchInput);
	

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&APlayerCharacter::PlayerCrouch);
	PlayerInputComponent->BindAction("Crouch",IE_Released,this,&APlayerCharacter::PlayerUnCrouch);
	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&APlayerCharacter::Fire);
}

void APlayerCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(),Value * Speed);
}

void APlayerCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(),Value * Speed);
}

void APlayerCharacter::PlayerCrouch()
{
	Crouch(true);
}

void APlayerCharacter::PlayerUnCrouch()
{
	UnCrouch(true);
}

void APlayerCharacter::GetWeapon()
{
	UWorld* World = GetWorld();
	if(World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AWeapon* Weapon = World->SpawnActor<AWeapon>(WeaponClass, SpawnParams);
		Weapon->K2_AttachToComponent(MeshComp,TEXT("weapon_socket"),
									 EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
									 EAttachmentRule::SnapToTarget, true);


		AWeapon* FPSWeapon = World->SpawnActor<AWeapon>(FPSWeaponClass, SpawnParams);
		FPSWeapon->K2_AttachToComponent(FPSSkeletalMeshComp,TEXT("weapon_socket"),
									 EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
									 EAttachmentRule::SnapToTarget, true);
		FPSMainWeapon = FPSWeapon;
		TPSWeapon = Weapon;
	}
}

void APlayerCharacter::NetMulticastDestroyWeapon_Implementation()
{
	TPSWeapon->Destroy();
	FPSMainWeapon->Destroy();
}

void APlayerCharacter::Fire()
{
	const FVector SpawnLocation = FPSMainWeapon->MuzzleLocation->GetComponentLocation();
	const FRotator MuzzleRotation = FPSMainWeapon->MuzzleLocation->GetComponentRotation();
	FRotator SpawnRotation = GetControlRotation();
	SpawnRotation.Pitch = MuzzleRotation.Pitch - 10;
	ServerFire(SpawnLocation, SpawnRotation);
}

void APlayerCharacter::ServerFire_Implementation(FVector SpawnLocation, FRotator SpawnRotation)
{
	NetMulticastPlayFireAnimMontage();
	//试图发射发射物。
	if (BaseProjectileClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// 在枪口位置生成发射物。
			AProjectile* Projectile = World->SpawnActor<AProjectile>(BaseProjectileClass, SpawnLocation, SpawnRotation,
			                                                         SpawnParams);
		}
	}
}

void APlayerCharacter::PlayerTakeDamage(float Value)
{
	PlayerHealth -= Value;
	if(PlayerHealth <=0)
	{
		PlayerHealth = 0;
		Die();
	}
}

void APlayerCharacter::Die()
{
	bIsDead = true;
	ServerDie();
}

void APlayerCharacter::ServerDie_Implementation()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		NetMulticastPlayDieAnimMontage();
		NetMulticastDestroyWeapon();
		ATencentNetFPSGameModeBase* GameMode = Cast<ATencentNetFPSGameModeBase>(GetWorld()->GetAuthGameMode());
		if(GameMode)
		{
			GameMode->ReStartLifePlayer(this);
		}
	}
}

void APlayerCharacter::ServerDestroySelf_Implementation()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		Destroy();
	}
}

void APlayerCharacter::NetMulticastPlayDieAnimMontage_Implementation()
{
	MeshComp->GetAnimInstance()->Montage_Play(PlayerDeathMontage);
}

void APlayerCharacter::NetMulticastPlayFireAnimMontage_Implementation()
{
	FPSSkeletalMeshComp->GetAnimInstance()->Montage_Play(PlayerFireMontage);
	UGameplayStatics::PlaySoundAtLocation(this, ShootSound, FPSMainWeapon->MuzzleLocation->GetComponentLocation());
}



