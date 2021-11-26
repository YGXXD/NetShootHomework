// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "TencentNetFPS/TencentNetFPSGameModeBase.h"
#include "TencentNetFPS/Player/PlayerCharacter.h"

AProjectile::AProjectile() 
{
	//子弹碰撞体组件
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);//绑定碰撞事件
	
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	RootComponent = CollisionComp;

	ProjectileMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	ProjectileMeshComp->SetupAttachment(CollisionComp);
	
	// 子弹移动组件
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	//初始生命周期
	InitialLifeSpan = 3.0f;

	SetReplicates(true);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	APlayerCharacter* Enemy = Cast<APlayerCharacter>(OtherActor);
	if(Enemy && !Enemy->bIsDead)
	{
		if(Enemy->GetHealth()<=20)
		{
			ATencentNetFPSGameModeBase* GameMode = Cast<ATencentNetFPSGameModeBase>(GetWorld()->GetAuthGameMode());
			if(GameMode)
			{
				GameMode->PlayerControllerSelectMap[Cast<APawn>(GetOwner())]->ClientAddKillNumber(1);
			}
		}
		Enemy->PlayerTakeDamage(20);
		Destroy();
	}
}