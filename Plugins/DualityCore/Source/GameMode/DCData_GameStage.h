// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/DataAsset.h"

#include "GameplayTagContainer.h"

#include "DCData_GameStage.generated.h"

class UTexture2D;
class UUserWidget;
struct FPrimaryAssetId;


/** 
 * ゲームのステージを定義するデータアセット
 * 
 * UI に表示される情報や遷移する Map の情報、ステージの解放条件が含まれる
 */
UCLASS(BlueprintType)
class UDCData_GameStage : public UPrimaryDataAsset
{
public:
	GENERATED_BODY()

public:
	//
	// ステージの開放に必要な GameplayTag
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction", Categories = "Stage", ForceInlineRow))
	FGameplayTagContainer ActivationRequestTags;

	//
	// 遷移する Map の PrimaryAssetId
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameStage", meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	//
	// UI に表示するステージタイトル名
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameStage")
	FText TileTitle;

	//
	// UI に表示するステージサブタイトル名
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameStage")
	FText TileSubTitle;

	//
	// UI に表示するステージ説明
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameStage")
	FText TileDescription;

	//
	// UI に表示するステージのプレビュー画像
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameStage")
	TObjectPtr<UTexture2D> PreviewImage;

	//
	// Map 遷移時に表示するロード画面の Widget
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameStage")
	TSoftClassPtr<UUserWidget> LoadingScreenWidget;
};
