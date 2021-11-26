// Copyright Epic Games, Inc. All Rights Reserved.


#include "TencentNetFPSGameModeBase.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterController.h"
#include "UI/ShootHUD.h"

ATencentNetFPSGameModeBase::ATencentNetFPSGameModeBase()
{
	bIsGameOver = false;
	PrimaryActorTick.bCanEverTick = true;
	RemainTime = 600;
	SpawnVectors.Add(FVector(0,0,0));
	SpawnVectors.Add(FVector(10,0,0));
	SpawnVectors.Add(FVector(-10,0,0));
	SpawnVectors.Add(FVector(0,10,0));
	SpawnVectors.Add(FVector(0,-10,0));
	SpawnVectors.Add(FVector(10,10,0));
	SpawnVectors.Add(FVector(-10,-10,0));
	SpawnVectors.Add(FVector(10,-10,0));
	SpawnVectors.Add(FVector(-10,10,0));
	SpawnNum = 0;
	
	
	static ConstructorHelpers::FClassFinder<APlayerCharacter> PlayerPawnBPClass(TEXT("/Game/BluePrint/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Succeeded())
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AShootHUD> BaseHUDClass(TEXT("/Game/BluePrint/BP_ShootHUD"));
	if(BaseHUDClass.Succeeded())
	{
		HUDClass = BaseHUDClass.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APlayerCharacterController> ControllerBPClass(TEXT("/Game/BluePrint/BP_PlayerCharacterController"));
	if(ControllerBPClass.Succeeded())
	{
		PlayerControllerClass = ControllerBPClass.Class;
	}
}

void ATencentNetFPSGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATencentNetFPSGameModeBase::Tick(float DeltaSeconds)
{
	if(!bIsGameOver)
	{
		if(RemainTime>0)
		{
			for(int i=0;i<PlayerControllerList.Num();i++)
			{
				PlayerControllerList[i]->ClientUpdateTime(RemainTime);
			}
			RemainTime -= DeltaSeconds;
		}
		else
		{
			for(int i=0;i<PlayerControllerList.Num();i++)
			{
				PlayerControllerList[i]->ClientGameOver();
			}
			bIsGameOver = true;
		}
	}
}

void ATencentNetFPSGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(NewPlayer);
	if(NewPlayer->GetPawn() && PlayerCharacterController)
	{
		PlayerControllerSelectMap.Add(NewPlayer->GetPawn(),PlayerCharacterController);
		PlayerControllerList.Add(PlayerCharacterController);
		PlayerCharacterController->ClientInitGameWidget();
	}
}

void ATencentNetFPSGameModeBase::ReStartLifePlayer_Implementation(const APawn* PlayerCharacter)
{
	if(PlayerControllerSelectMap.Contains(PlayerCharacter))
	{
		APlayerCharacterController* DeadPlayerController = PlayerControllerSelectMap[PlayerCharacter];
		PlayerControllerSelectMap.Remove(PlayerCharacter);
		DeadPlayerController->ClientAddDeadNumber(1);
		DeadPlayerController->Possess(nullptr);
		auto SpawnPlayerCharacter = [this,DeadPlayerController]()
		{
			if (DefaultPawnClass)
			{
				UWorld* World = GetWorld();
				if (World)
				{
					int idx = SpawnNum % SpawnVectors.Num();
					APlayerCharacter* NewPlayerCharacter = World->SpawnActor<APlayerCharacter>(DefaultPawnClass, SpawnVectors[idx], FRotator::ZeroRotator);
					SpawnNum++;
					DeadPlayerController->Possess(NewPlayerCharacter);
					PlayerControllerSelectMap.Add(NewPlayerCharacter,DeadPlayerController);
				}
			}
		};
		FTimerHandle Timer;
		const FTimerDelegate SpawnPlayerCharacterDelegate = FTimerDelegate::CreateLambda(SpawnPlayerCharacter);
		GetWorld()->GetTimerManager().SetTimer(Timer,SpawnPlayerCharacterDelegate,5.f,false);
	}
}



