// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/DataAsset.h"

#include "DCData_Character.generated.h"

class UDCData_AbilitySet;
class UDCData_InputConfig;
class UDCCameraMode;


/**
 *	Character の基本情報を定義する
 */
UCLASS(BlueprintType, Const)
class UDCData_Character : public UPrimaryDataAsset
{
public:
	GENERATED_BODY()
public:
	UDCData_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
	}

public:
	//
	// この Character の AbilitySystem に付与する AbilitySet 
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TObjectPtr<UDCData_AbilitySet>> AbilitySets;

	//
	// 入力マッピングを作成し、入力アクションをバインドするために、プレーヤーが制御する Pawn によって使用される入力構成
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UDCData_InputConfig> InputConfig{ nullptr };

	//
	// プレイヤーが制御する Character が使用するデフォルトの CameraMode
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TSubclassOf<UDCCameraMode> DefaultCameraMode{ nullptr };

};
