// Copyright (C) 2023 owoDra

#include "DCAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCAttributeSet)


UWorld* UDCAttributeSet::GetWorld() const
{
	const auto* Outer{ GetOuter() };
	check(Outer);

	return Outer->GetWorld();
}

UDCAbilitySystemComponent* UDCAttributeSet::GetDCAbilitySystemComponent() const
{
	return Cast<UDCAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
