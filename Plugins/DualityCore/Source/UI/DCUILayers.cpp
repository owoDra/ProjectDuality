// Copyright (C) 2023 owoDra

#include "DCUILayers.h"

#include "Development/DCLogChannels.h"

#include "Containers/Map.h"
#include "UObject/ObjectPtr.h"
#include "GameplayTagContainer.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCUILayers)


void UDCUILayers::RegisterLayer(FGameplayTag LayerTag, UOverlay* Overlay)
{
	if (LayerTag.IsValid() && Overlay)
	{
		RegisteredLayers.Add(LayerTag, { Overlay, nullptr });
		return;
	}

	UE_LOG(LogDCUserInterface, Warning, TEXT("DCUILayers にて無効なレイヤーの登録が実行されようとしてました"));
}

void UDCUILayers::PushWidgetToLayer(const FGameplayTag& LayerTag, UUserWidget* Widget)
{
	if (!(LayerTag.IsValid() && Widget))
	{
		UE_LOG(LogDCUserInterface, Warning, TEXT("DCUILayers にて無効な Widget の追加が実行されようとしてました"));
		return;
	}

	if (auto * Layer{ RegisteredLayers.Find(LayerTag) })
	{
		// 既に Widget がある場合は削除する

		if (Layer->ActiveWidget)
		{
			Layer->ActiveWidget->RemoveFromParent();
			Layer->ActiveWidget = Widget;
		}

		auto* OverlaySlot{ Layer->OverlayPanel->AddChildToOverlay(Widget) };

		OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	}
}
