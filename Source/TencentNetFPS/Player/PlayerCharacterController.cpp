// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"

#include "TencentNetFPS/UI/GameOverWidget.h"

APlayerCharacterController::APlayerCharacterController()
{
	KillNumber = 0;
	DeadNumber = 0;

	static ConstructorHelpers::FClassFinder<UGameOverWidget> GameOverWidgetBPClass(TEXT("/Game/BluePrint/UI/GameOver"));
	if(GameOverWidgetBPClass.Succeeded())
	{
		GameOverWidgetClass = GameOverWidgetBPClass.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UGameWidget> GameWidgetBPClass(TEXT("/Game/BluePrint/UI/GameTime"));
	if(GameWidgetBPClass.Succeeded())
	{
		GameWidgetClass = GameWidgetBPClass.Class;
	}
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacterController::ClientChangeWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if(CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
	if(NewWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(),NewWidgetClass);
		if(CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void APlayerCharacterController::ClientInitGameWidget_Implementation()
{
	ClientChangeWidget(GameWidgetClass);
	GameWidget = Cast<UGameWidget>(CurrentWidget);
	GameWidget->UpdateKda(KillNumber,DeadNumber);
}


void APlayerCharacterController::ClientAddKillNumber_Implementation(int Num)
{
	KillNumber += Num;
	GameWidget->UpdateKda(KillNumber,DeadNumber);
}

void APlayerCharacterController::ClientAddDeadNumber_Implementation(int Num)
{
	DeadNumber += Num;
	GameWidget->UpdateKda(KillNumber,DeadNumber);
}

void APlayerCharacterController::ClientUpdateTime_Implementation(float Time)
{
	GameWidget->UpdateTime(Time);
}

void APlayerCharacterController::ClientGameOver_Implementation()
{
	ClientChangeWidget(GameOverWidgetClass);
	UGameOverWidget* GameOverWidget = Cast<UGameOverWidget>(CurrentWidget);
	GameOverWidget->SetScore(KillNumber,DeadNumber);
}
