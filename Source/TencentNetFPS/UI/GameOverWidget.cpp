// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"

bool UGameOverWidget::Initialize()
{
	Super::Initialize();
	KillNumberBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("KillNumber")));
	DeadNumberBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("DeadNumber")));
	
	return true;
}

void UGameOverWidget::SetScore(int KillNum, int DeadNum)
{
	KillNumberBlock->SetText(FText::FromString(FString::FromInt(KillNum)));
	DeadNumberBlock->SetText(FText::FromString(FString::FromInt(DeadNum)));
}
