// Copyright (C) 2023 owoDra

#pragma once

#include "GameFramework/HUD.h"

#include "DCHUD.generated.h"

class UUserWidget;
class UDCUILayers;
class UDCHUDLayouts;
class APlayerController;
struct FGameplayTag;


/**
 * プレイヤーに表示するユーザーインターフェースの管理を行うクラス
 */
UCLASS(Config = "Game", meta = (DisplayName = "DCUIManager (DCHUD)"))
class DUALITYCORE_API ADCHUD : public AHUD
{
public:
	GENERATED_BODY()
public:
	ADCHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
	}

protected:
	//
	// UI 表示のレイヤーウィジェットクラス
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Config")
	TSubclassOf<UDCUILayers> UILayersClass;

	//
	// HUD 表示のレイアウトウィジェットクラス
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Config")
	TSubclassOf<UDCHUDLayouts> HUDLayoutsClass;

private:
	//
	// UI 表示のレイヤーウィジェットインスタンス
	//
	UPROPERTY(Transient)
	TObjectPtr<UDCUILayers> UILayersWidget;

	//
	// HUD 表示のレイアウトウィジェットインスタンス
	//
	UPROPERTY(Transient)
	TObjectPtr<UDCHUDLayouts> HUDLayoutsWidget;


protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/**
	 * Widget を UILayers に追加する
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|Layers")
	void PushUIWidgetToLayer(APlayerController* PC, const FGameplayTag& LayerTag, TSoftClassPtr<UUserWidget> WidgetClassPtr) const;

	/**
	 * Widget を HUDLayouts に追加する
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void PushHUDWidgetToSlot(APlayerController* PC, const FGameplayTag& SlotTag, TSoftClassPtr<UUserWidget> WidgetClassPtr) const;
};
