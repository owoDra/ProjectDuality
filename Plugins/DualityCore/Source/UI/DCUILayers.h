// Copyright (C) 2023 owoDra

#pragma once

#include "Blueprint/UserWidget.h"

#include "DCUILayers.generated.h"

class UOverlay;
struct FGameplayTag;


/**
 * 登録されているレイヤーの情報
 */
USTRUCT(BlueprintType)
struct FUILayer
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TObjectPtr<UOverlay> OverlayPanel{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TObjectPtr<UUserWidget> ActiveWidget{ nullptr };
};


/**
 * ゲームプレイにプレイヤーに表示されるUIの表示順序などを決定するためのウィジェット
 * 
 * このレイヤに追加されるUIは基本的には以下のものが当てはまります
 * - ゲームメニュー
 * - モーダルウィンドウ
 * - ロード画面
 */
UCLASS(Abstract, Blueprintable)
class UDCUILayers : public UUserWidget
{
public:
	GENERATED_BODY()
public:
	UDCUILayers(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
	}

protected:
	//
	// 登録されたレイヤーのマッピングリスト
	//
	UPROPERTY(Transient)
	TMap<FGameplayTag, FUILayer> RegisteredLayers;

	/**
	 * OverlayPanel をレイヤーとして登録する
	 * 
	 * この関数は OnInitialized 時に実行する必要があります
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|Layers", meta = (GameplayTagFilter = "UI.Layer"))
	void RegisterLayer(FGameplayTag LayerTag, UOverlay* Overlay);

public:
	/**
	 * LayerTag に関連するレイヤーに Widget を追加する
	 * 元々 Widget がある場合は削除する
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|Layers", meta = (GameplayTagFilter = "UI.Layer"))
	void PushWidgetToLayer(const FGameplayTag& LayerTag, UUserWidget* Widget);
};
