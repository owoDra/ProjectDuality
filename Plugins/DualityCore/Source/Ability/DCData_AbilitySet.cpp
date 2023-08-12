// Copyright (C) 2023 owoDra

#include "DCData_AbilitySet.h"

#include "Ability/DCAbilitySystemComponent.h"
#include "Ability/DCGameplayAbility.h"
#include "Development/DCLogChannels.h"

#include "GameplayAbilitySpec.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCData_AbilitySet)


#pragma region GrantedHandles

void FDCAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FDCAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FDCAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	if (Set)
	{
		GrantedAttributeSets.Add(Set);
	}
}

void FDCAbilitySet_GrantedHandles::TakeFromAbilitySystem(UDCAbilitySystemComponent* DCASC)
{
	check(DCASC);

	// 権限があるか検証

	if (!DCASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	// 登録解除を行う

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			DCASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			DCASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		DCASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

#pragma endregion


#pragma region AbilitySet

void UDCData_AbilitySet::GiveToAbilitySystem(UDCAbilitySystemComponent* DCASC, FDCAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(DCASC);

	// 権限があるか検証

	if (!DCASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	// GameplayAbility の付与を行う

	for (int32 AbilityIndex{ 0 }; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const auto& AbilityToGrant{ GrantedGameplayAbilities[AbilityIndex] };

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogDCAbilitySystem, Warning, TEXT("AbilitySet(%s) の GrantedGameplayAbilities[%d] が無効です"), *GetNameSafe(this), AbilityIndex);
			continue;
		}

		auto* AbilityCDO{ AbilityToGrant.Ability->GetDefaultObject<UDCGameplayAbility>() };

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const auto AbilitySpecHandle{ DCASC->GiveAbility(AbilitySpec) };

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// GameplayEffect の付与を行う

	for (int32 EffectIndex{ 0 }; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const auto& EffectToGrant{ GrantedGameplayEffects[EffectIndex] };

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogDCAbilitySystem, Warning, TEXT("AbilitySet(%s) の GrantedGameplayEffects[%d] が無効です"), *GetNameSafe(this), EffectIndex);
			continue;
		}

		const auto* GameplayEffect{ EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>() };
		const auto GameplayEffectHandle{ DCASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, DCASC->MakeEffectContext()) };

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// AttributeSet の付与を行う
	
	for (int32 SetIndex{ 0 }; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const auto& SetToGrant{ GrantedAttributes[SetIndex] };

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UE_LOG(LogDCAbilitySystem, Warning, TEXT("AbilitySet(%s) の GrantedAttributes[%d] が無効です"), *GetNameSafe(this), SetIndex);
			continue;
		}

		auto* NewSet{ NewObject<UAttributeSet>(DCASC->GetOwner(), SetToGrant.AttributeSet) };
		DCASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
}

#pragma endregion
