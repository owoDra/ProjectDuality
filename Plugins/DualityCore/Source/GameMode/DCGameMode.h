// Copyright (C) 2023 owoDra

#pragma once

#include "GameFramework/GameModeBase.h"

#include "DCGameMode.generated.h"


/**
 * ゲームモードのプレイヤースポーンなどの管理を行うクラス
 */
UCLASS(Config = Game)
class ADCGameMode : public AGameModeBase
{
public:
	GENERATED_BODY()
public:
	ADCGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
