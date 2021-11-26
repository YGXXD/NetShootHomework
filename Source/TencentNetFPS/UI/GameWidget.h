// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameWidget.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTNETFPS_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UTextBlock* RemainTimeTextBlock;

	UPROPERTY()
	UTextBlock* KillNumberTextBlock;

	UPROPERTY()
	UTextBlock* DeadNumberTextBlock;
	
	virtual bool Initialize() override;

public:
	void UpdateTime(float RemainTime);

	void UpdateKda(int KillNumber,int DeadNumber);
};

