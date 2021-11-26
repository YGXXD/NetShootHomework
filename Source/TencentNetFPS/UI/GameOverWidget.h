// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTNETFPS_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UTextBlock* KillNumberBlock;

	UPROPERTY()
	UTextBlock* DeadNumberBlock;

	
	virtual bool Initialize() override;

public:
	void SetScore(int KillNum,int DeadNum);
};
