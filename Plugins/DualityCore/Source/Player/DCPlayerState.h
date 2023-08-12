// Copyright (C) 2023 owoDra

#pragma once

#include "GameFramework/PlayerState.h"

#include "DCPlayerState.generated.h"

/**
 * ADCPlayerState
 *
 *	このプロジェクトで使用されるベースの PlayerState クラス。
 */
UCLASS(Config = Game)
class DUALITYCORE_API ADCPlayerState : public APlayerState
{
public:
	GENERATED_BODY()
public:
	ADCPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
	}
	
public:
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	ADCPlayerController* GetDCPlayerController() const;
};
