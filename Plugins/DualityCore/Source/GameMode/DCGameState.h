// Copyright (C) 2023 owoDra

#pragma once

#include "GameFramework/GameStateBase.h"

#include "DCGameState.generated.h"


/**
 * ゲームモードの進捗情報などを管理するクラス
 */
UCLASS(Config = Game)
class DUALITYCORE_API ADCGameState : public AGameStateBase
{
public:
	GENERATED_BODY()
public:
	ADCGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
	}

};
