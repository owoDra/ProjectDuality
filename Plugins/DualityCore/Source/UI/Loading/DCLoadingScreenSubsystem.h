// Copyright (C) 2023 owoDra

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"

#include "DCLoadingScreenSubsystem.generated.h"

class UUserWidget;


/**
 * ロード画面ウィジェットが変更されたことを知らせるデリゲート
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadingScreenWidgetChangedDelegate, TSubclassOf<UUserWidget>, NewWidgetClass);


/**
 * マップ間の遷移の為に現在のロード画面ウィジェットを管理するサブシステム
 */
UCLASS()
class DUALITYCORE_API UDCLoadingScreenSubsystem : public UGameInstanceSubsystem
{
public:
	GENERATED_BODY()
public:
	UDCLoadingScreenSubsystem() {}

private:
	//
	// ロード画面ウィジェットが変更されたことを知らせるデリゲート変数
	//
	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess))
	FLoadingScreenWidgetChangedDelegate OnLoadingScreenWidgetChanged;

	//
	// 現在のロード画面ウィジェットクラス
	//
	UPROPERTY(Transient)
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass;

public:
	/**
	 * 現在のロード画面ウィジェットクラスを設定する
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|Loading")
	void SetLoadingScreenWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	/**
	 * 現在のロード画面ウィジェットクラスを取得する
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI|Loading")
	TSubclassOf<UUserWidget> GetLoadingScreenWidget() const { return LoadingScreenWidgetClass; }
};
