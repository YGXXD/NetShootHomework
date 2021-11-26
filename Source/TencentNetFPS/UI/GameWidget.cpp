// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"

bool UGameWidget::Initialize()
{
	Super::Initialize();

	RemainTimeTextBlock = Cast<UTextBlock>(GetWidgetFromName("RemainTime"));
	KillNumberTextBlock = Cast<UTextBlock>(GetWidgetFromName("KillNumber"));
	DeadNumberTextBlock = Cast<UTextBlock>(GetWidgetFromName("DeadNumber"));
	
	return true;
}

void UGameWidget::UpdateTime(float RemainTime)
{
	RemainTimeTextBlock->SetText(FText::FromString(FString::FromInt(RemainTime)));
}

void UGameWidget::UpdateKda(int KillNumber, int DeadNumber)
{
	KillNumberTextBlock->SetText(FText::FromString(FString::FromInt(KillNumber)));
	DeadNumberTextBlock->SetText(FText::FromString(FString::FromInt(DeadNumber)));
}

