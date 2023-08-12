// Copyright (C) 2023 owoDra

#include "DCHUD.h"

#include "UI/DCUILayers.h"
#include "UI/DCHUDLayouts.h"

#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCHUD)


void ADCHUD::BeginPlay()
{
	auto* PlayerController{ GetOwningPlayerController() };

	// UILayers を作成する
	// ZOrder を 20 に設定し、その他の Widget の邪魔にならないように最前面に表示する

	static constexpr int32 UILayersZOrder{ 20 };

	UILayersWidget = CreateWidget<UDCUILayers>(PlayerController, UDCUILayers::StaticClass());
	if (UILayersWidget)
	{
		UILayersWidget->AddToViewport(20);
	}

	// HUDLayouts を作成する
	// ZOrder を 10 に設定し、その他の Widget の邪魔にならないように UILayers の背面に表示する

	static constexpr int32 HUDLayoutsZOrder{ 10 };

	HUDLayoutsWidget = CreateWidget<UDCHUDLayouts>(PlayerController, UDCHUDLayouts::StaticClass());
	if (HUDLayoutsWidget)
	{
		HUDLayoutsWidget->AddToViewport(10);
	}

	Super::BeginPlay();
}

void ADCHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UILayersWidget)
	{
		UILayersWidget->RemoveFromParent();
	}

	if (HUDLayoutsWidget)
	{
		HUDLayoutsWidget->RemoveFromParent();
	}

	Super::EndPlay(EndPlayReason);
}

void ADCHUD::PushUIWidgetToLayer(APlayerController* PC, const FGameplayTag& LayerTag, TSoftClassPtr<UUserWidget> WidgetClassPtr) const
{
	// 引数が有効かチェック

	if (!(PC && WidgetClassPtr.IsValid() && LayerTag.IsValid()))
	{
		return;
	}


	// ウィジェットをロード

	auto& StreamableManager{ UAssetManager::Get().GetStreamableManager() };
	auto StreamingHandle{ 
		StreamableManager.RequestAsyncLoad(
			WidgetClassPtr.ToSoftObjectPath(), 
			FStreamableDelegate::CreateWeakLambda(
				this, [this, PC, LayerTag, WidgetClassPtr]()
				{
					// ウィジェットを作成

					auto* Widget{ CreateWidget<UUserWidget>(PC, WidgetClassPtr.Get()) };


					// UILayers に追加

					if (auto * HUD{ Cast<ADCHUD>(PC->GetHUD()) })
					{
						HUD->UILayersWidget->PushWidgetToLayer(LayerTag, Widget);
					}
				}
			)
		) 
	};
}

void ADCHUD::PushHUDWidgetToSlot(APlayerController* PC, const FGameplayTag& SlotTag, TSoftClassPtr<UUserWidget> WidgetClassPtr) const
{
	// 引数が有効かチェック

	if (!(PC && WidgetClassPtr.IsValid() && SlotTag.IsValid()))
	{
		return;
	}


	// ウィジェットをロード

	auto& StreamableManager{ UAssetManager::Get().GetStreamableManager() };
	auto StreamingHandle{
		StreamableManager.RequestAsyncLoad(
			WidgetClassPtr.ToSoftObjectPath(),
			FStreamableDelegate::CreateWeakLambda(
				this, [this, PC, SlotTag, WidgetClassPtr]()
				{
					// ウィジェットを作成

					auto* Widget{ CreateWidget<UUserWidget>(PC, WidgetClassPtr.Get()) };


					// UILayers に追加

					if (auto * HUD{ Cast<ADCHUD>(PC->GetHUD()) })
					{
						HUD->HUDLayoutsWidget->PushWidgetToSlot(SlotTag, Widget);
					}
				}
			)
		)
	};
}
