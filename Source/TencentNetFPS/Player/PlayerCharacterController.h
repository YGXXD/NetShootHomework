// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TencentNetFPS/UI/GameOverWidget.h"
#include "TencentNetFPS/UI/GameWidget.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTNETFPS_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TSubclassOf<UGameWidget> GameWidgetClass;

	UPROPERTY()
	TSubclassOf<UGameOverWidget> GameOverWidgetClass;
	
	UPROPERTY()
	UUserWidget* CurrentWidget;
	
	UPROPERTY()
	int KillNumber;

	UPROPERTY()
	int DeadNumber;

	APlayerCharacterController();
	virtual void BeginPlay() override;
	
public:
	int ControllerID;
	
	UPROPERTY()
	UGameWidget* GameWidget;

	UFUNCTION(Client,Reliable)
	void ClientInitGameWidget();

	UFUNCTION(Client,Reliable)
	void ClientAddKillNumber(int Num);

	UFUNCTION(Client,Reliable)
	void ClientAddDeadNumber(int Num);

	UFUNCTION(Client,Reliable)
	void ClientUpdateTime(float Time);

	UFUNCTION(Client,Reliable)
	void ClientChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION(Client,Reliable)
	void ClientGameOver();
};
