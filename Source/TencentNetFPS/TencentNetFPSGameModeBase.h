// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterController.h"
#include "UI/GameWidget.h"
#include "TencentNetFPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTNETFPS_API ATencentNetFPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
private:
	bool bIsGameOver;
	float RemainTime;
	TArray<FVector> SpawnVectors;
	int SpawnNum;
	ATencentNetFPSGameModeBase();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	UPROPERTY()
	TMap<APawn*,APlayerCharacterController*> PlayerControllerSelectMap;
	
	UPROPERTY()
	TArray<APlayerCharacterController*> PlayerControllerList;
	
	

	UFUNCTION(Server,Reliable)
	void ReStartLifePlayer(const APawn* PlayerCharacter);
	
};
