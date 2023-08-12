// Copyright (C) 2023 owoDra

#pragma once

#include "Blueprint/UserWidget.h"

#include "DCHUDLayouts.generated.h"

class UOverlay;
struct FGameplayTag;


/**
 * 登録されているスロットの情報
 */
USTRUCT(BlueprintType)
struct FHUDSlot
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
 * ゲームプレイにプスロットに表示される HUD 項目の表示位置などを決定するためのウィジェット
 */
UCLASS(Abstract, Blueprintable)
class UDCHUDLayouts : public UUserWidget
{
public:
	GENERATED_BODY()
public:
	UDCHUDLayouts(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
	}

protected:
	//
	// 登録されたスロットのマッピングリスト
	//
	UPROPERTY(Transient)
	TMap<FGameplayTag, FHUDSlot> RegisteredSlots;

	/**
	 * OverlayPanel をスロットとして登録する
	 *
	 * この関数は OnInitialized 時に実行する必要があります
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|Layers", meta = (GameplayTagFilter = "UI.HUD.Slot"))
	void RegisterSlot(FGameplayTag SlotTag, UOverlay* Overlay);

public:
	/**
	 * SlotTag に関連するスロットに Widget を追加する
	 * 元々 Widget がある場合は削除する
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|Layers", meta = (GameplayTagFilter = "UI.HUD.Slot"))
	void PushWidgetToSlot(const FGameplayTag& SlotTag, UUserWidget* Widget);
};