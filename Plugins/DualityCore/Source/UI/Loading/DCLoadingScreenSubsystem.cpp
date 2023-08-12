// Copyright (C) 2023 owoDra

#include "DCLoadingScreenSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCLoadingScreenSubsystem)


void UDCLoadingScreenSubsystem::SetLoadingScreenWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (LoadingScreenWidgetClass != NewWidgetClass)
	{
		LoadingScreenWidgetClass = NewWidgetClass;

		OnLoadingScreenWidgetChanged.Broadcast(LoadingScreenWidgetClass);
	}
}
