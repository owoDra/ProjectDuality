// Copyright (C) 2023 owoDra

#include "DCHUDLayouts.h"

#include "Development/DCLogChannels.h"

#include "Containers/Map.h"
#include "UObject/ObjectPtr.h"
#include "GameplayTagContainer.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCHUDLayouts)


void UDCHUDLayouts::RegisterSlot(FGameplayTag SlotTag, UOverlay* Overlay)
{
	if (SlotTag.IsValid() && Overlay)
	{
		RegisteredSlots.Add(SlotTag, { Overlay, nullptr });
		return;
	}

	UE_LOG(LogDCUserInterface, Warning, TEXT("DCHUDLayouts にて無効なレイヤーの登録が実行されようとしてました"));
}

void UDCHUDLayouts::PushWidgetToSlot(const FGameplayTag& SlotTag, UUserWidget* Widget)
{
	if (!(SlotTag.IsValid() && Widget))
	{
		UE_LOG(LogDCUserInterface, Warning, TEXT("DCHUDLayouts にて無効な Widget の追加が実行されようとしてました"));
		return;
	}

	if (auto* HUDSlot{ RegisteredSlots.Find(SlotTag) })
	{
		// 既に Widget がある場合は削除する

		if (HUDSlot->ActiveWidget)
		{
			HUDSlot->ActiveWidget->RemoveFromParent();
			HUDSlot->ActiveWidget = Widget;
		}

		auto* OverlaySlot{ HUDSlot->OverlayPanel->AddChildToOverlay(Widget) };

		OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	}
}
