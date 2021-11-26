// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShootHUD.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTNETFPS_API AShootHUD : public AHUD
{
	GENERATED_BODY()

public:
	// HUD绘制的主要调用。
	virtual void DrawHUD() override;

protected:
	// 将被绘制在屏幕中心。
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTexture2D* CenterTexture;
};
