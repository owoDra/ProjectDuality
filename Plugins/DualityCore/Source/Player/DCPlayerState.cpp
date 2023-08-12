// Copyright (C) 2023 owoDra

#include "DCPlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCPlayerState)


ADCPlayerController* ADCPlayerState::GetDCPlayerController() const
{
	return GetOwner<ADCPlayerController>();
}
