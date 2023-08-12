// Copyright (C) 2023 owoDra

#include "DCGlobalAbilitySystem.h"

#include "Ability/DCAbilitySystemComponent.h"

//#include "Abilities/GameplayAbility.h"
//#include "GameplayAbilitySpec.h"
//#include "GameplayEffect.h"
//#include "GameplayEffectTypes.h"
//#include "Misc/AssertionMacros.h"
//#include "Templates/ChooseClass.h"
//#include "Templates/Tuple.h"
//#include "Templates/TypeHash.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCGlobalAbilitySystem)


#pragma region GlobalAppliedAbilityList

void FGlobalAppliedAbilityList::AddToASC(TSubclassOf<UGameplayAbility> Ability, UDCAbilitySystemComponent* ASC)
{
	// 既に追加されている場合は先に削除する

	if (auto* SpecHandle{ Handles.Find(ASC) })
	{
		RemoveFromASC(ASC);
	}

	auto* AbilityCDO{ Ability->GetDefaultObject<UGameplayAbility>() };
	FGameplayAbilitySpec AbilitySpec{ AbilityCDO };
	const auto AbilitySpecHandle{ ASC->GiveAbility(AbilitySpec) };
	Handles.Add(ASC, AbilitySpecHandle);
}

void FGlobalAppliedAbilityList::RemoveFromASC(UDCAbilitySystemComponent* ASC)
{
	if (auto* SpecHandle{ Handles.Find(ASC) })
	{
		ASC->ClearAbility(*SpecHandle);
		Handles.Remove(ASC);
	}
}

void FGlobalAppliedAbilityList::RemoveFromAll()
{
	for (auto& KVP : Handles)
	{
		if (KVP.Key != nullptr)
		{
			KVP.Key->ClearAbility(KVP.Value);
		}
	}
	Handles.Empty();
}

#pragma endregion


#pragma region GlobalAppliedEffectList

void FGlobalAppliedEffectList::AddToASC(TSubclassOf<UGameplayEffect> Effect, UDCAbilitySystemComponent* ASC)
{
	// 既に追加されている場合は先に削除する

	if (auto* EffectHandle{ Handles.Find(ASC) })
	{
		RemoveFromASC(ASC);
	}

	const auto* GameplayEffectCDO{ Effect->GetDefaultObject<UGameplayEffect>() };
	const auto GameplayEffectHandle{ ASC->ApplyGameplayEffectToSelf(GameplayEffectCDO, 1, ASC->MakeEffectContext()) };
	Handles.Add(ASC, GameplayEffectHandle);
}

void FGlobalAppliedEffectList::RemoveFromASC(UDCAbilitySystemComponent* ASC)
{
	if (auto* EffectHandle{ Handles.Find(ASC) })
	{
		ASC->RemoveActiveGameplayEffect(*EffectHandle);
		Handles.Remove(ASC);
	}
}

void FGlobalAppliedEffectList::RemoveFromAll()
{
	for (auto& KVP : Handles)
	{
		if (KVP.Key != nullptr)
		{
			KVP.Key->RemoveActiveGameplayEffect(KVP.Value);
		}
	}
	Handles.Empty();
}

#pragma endregion


#pragma region GlobalAbilitySystem

void UDCGlobalAbilitySystem::ApplyAbilityToAll(TSubclassOf<UGameplayAbility> Ability)
{
	if ((Ability.Get() != nullptr) && (!AppliedAbilities.Contains(Ability)))
	{
		auto& Entry{ AppliedAbilities.Add(Ability) };
		for (auto& ASC : RegisteredASCs)
		{
			Entry.AddToASC(Ability, ASC);
		}
	}
}

void UDCGlobalAbilitySystem::ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect)
{
	if ((Effect.Get() != nullptr) && (!AppliedEffects.Contains(Effect)))
	{
		auto& Entry{ AppliedEffects.Add(Effect) };
		for (auto& ASC : RegisteredASCs)
		{
			Entry.AddToASC(Effect, ASC);
		}
	}
}

void UDCGlobalAbilitySystem::RemoveAbilityFromAll(TSubclassOf<UGameplayAbility> Ability)
{
	if ((Ability.Get() != nullptr) && AppliedAbilities.Contains(Ability))
	{
		auto& Entry{ AppliedAbilities[Ability] };
		Entry.RemoveFromAll();
		AppliedAbilities.Remove(Ability);
	}
}

void UDCGlobalAbilitySystem::RemoveEffectFromAll(TSubclassOf<UGameplayEffect> Effect)
{
	if ((Effect.Get() != nullptr) && AppliedEffects.Contains(Effect))
	{
		auto& Entry{ AppliedEffects[Effect] };
		Entry.RemoveFromAll();
		AppliedEffects.Remove(Effect);
	}
}

void UDCGlobalAbilitySystem::RegisterASC(UDCAbilitySystemComponent* ASC)
{
	check(ASC);

	for (auto& Entry : AppliedAbilities)
	{
		Entry.Value.AddToASC(Entry.Key, ASC);
	}
	for (auto& Entry : AppliedEffects)
	{
		Entry.Value.AddToASC(Entry.Key, ASC);
	}

	RegisteredASCs.AddUnique(ASC);
}

void UDCGlobalAbilitySystem::UnregisterASC(UDCAbilitySystemComponent* ASC)
{
	check(ASC);
	for (auto& Entry : AppliedAbilities)
	{
		Entry.Value.RemoveFromASC(ASC);
	}
	for (auto& Entry : AppliedEffects)
	{
		Entry.Value.RemoveFromASC(ASC);
	}

	RegisteredASCs.Remove(ASC);
}

#pragma endregion
