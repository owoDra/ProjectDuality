// Copyright (C) 2023 owoDra

#include "DCAbilitySystemComponent.h"

#include "GameplayTag/DCTags_Flag.h"
#include "Development/DCLogChannels.h"
#include "Ability/DCGlobalAbilitySystem.h"
#include "Ability/DCGameplayAbility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCAbilitySystemComponent)


UDCAbilitySystemComponent::UDCAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UDCAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (auto* GlobalAbilitySystem{ UWorld::GetSubsystem<UDCGlobalAbilitySystem>(GetWorld()) })
	{
		GlobalAbilitySystem->UnregisterASC(this);
	}

	Super::EndPlay(EndPlayReason);
}

void UDCAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	auto* ActorInfo{ AbilityActorInfo.Get() };

	check(ActorInfo);
	check(InOwnerActor);

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	// 新し Avatar が設定されようとしていたら、登録されている DCGameplayAbility に知らせる

	if (InAvatarActor != ActorInfo->AvatarActor)
	{
		ABILITYLIST_SCOPE_LOCK();
		for (const auto& AbilitySpec : ActivatableAbilities.Items)
		{
			auto* DCAbilityCDO{ CastChecked<UDCGameplayAbility>(AbilitySpec.Ability) };

			if (DCAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
			{
				auto Instances{ AbilitySpec.GetAbilityInstances() };
				for (auto* AbilityInstance : Instances)
				{
					auto* DCAbilityInstance{ CastChecked<UDCGameplayAbility>(AbilityInstance) };
					DCAbilityInstance->OnAvatarSet(ActorInfo, AbilitySpec);
				}
			}
			else
			{
				DCAbilityCDO->OnAvatarSet(ActorInfo, AbilitySpec);
			}
		}

		// 全体に適応される GameplayAbility に応答できるように GlobalAbilitySystem にこの AbilitySystemComponent を追加する

		if (UDCGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UDCGlobalAbilitySystem>(GetWorld()))
		{
			GlobalAbilitySystem->RegisterASC(this);
		}
	}
}


void UDCAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// WaitInputPressTask の為に入力をレプリケートする

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UDCAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// WaitInputPressTask の為に入力をレプリケートする

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UDCAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility)
{
	ABILITYLIST_SCOPE_LOCK();
	for (const auto& AbilitySpec : ActivatableAbilities.Items)
	{
		// アクティブでないならスキップ

		if (!AbilitySpec.IsActive())
		{
			continue;
		}

		auto* DCAbilityCDO{ CastChecked<UDCGameplayAbility>(AbilitySpec.Ability) };

		// アビリティをインスタンスが存在する場合はそれらもキャンセルする

		if (DCAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
		{
			auto Instances{ AbilitySpec.GetAbilityInstances() };
			for (auto* AbilityInstance : Instances)
			{
				auto* DCAbilityInstance{ CastChecked<UDCGameplayAbility>(AbilityInstance) };

				// キャンセル可能か検証

				if (ShouldCancelFunc(DCAbilityInstance, AbilitySpec.Handle))
				{
					check(DCAbilityInstance->CanBeCanceled());
					DCAbilityInstance->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), DCAbilityInstance->GetCurrentActivationInfo(), bReplicateCancelAbility);
				}
			}
		}

		// アビリティをインスタンスが存在しない場合は CDO のみキャンセルする

		else
		{
			// キャンセル可能か検証

			if (ShouldCancelFunc(DCAbilityCDO, AbilitySpec.Handle))
			{
				check(DCAbilityCDO->CanBeCanceled());
				DCAbilityCDO->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), FGameplayAbilityActivationInfo(), bReplicateCancelAbility);
			}
		}
	}
}

void UDCAbilitySystemComponent::CancelInputActivatedAbilities(bool bReplicateCancelAbility)
{
	TShouldCancelAbilityFunc ShouldCancelFunc = [this](const UDCGameplayAbility* DCAbility, FGameplayAbilitySpecHandle Handle)
	{
		const auto ActivationPolicy{ DCAbility->GetActivationPolicy() };
		return ((ActivationPolicy == EDCAbilityActivationPolicy::OnInputTriggered) || (ActivationPolicy == EDCAbilityActivationPolicy::WhileInputActive));
	};

	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
}

void UDCAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const auto& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UDCAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const auto& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UDCAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	// アビリティに関する入力が制限されている場合は入力をキャンセル

	if (HasMatchingGameplayTag(TAG_Flag_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

	// 長押し入力を行うアビリティをキャッシュする

	for (const auto& SpecHandle : InputHeldSpecHandles)
	{
		if (const auto* AbilitySpec{ FindAbilitySpecFromHandle(SpecHandle) })
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const auto* DCAbilityCDO{ CastChecked<UDCGameplayAbility>(AbilitySpec->Ability) };

				if (DCAbilityCDO->GetActivationPolicy() == EDCAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	// 押し込み入力を行うアビリティをキャッシュする

	for (const auto& SpecHandle : InputPressedSpecHandles)
	{
		if (auto* AbilitySpec{ FindAbilitySpecFromHandle(SpecHandle) })
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const auto* DCAbilityCDO{ CastChecked<UDCGameplayAbility>(AbilitySpec->Ability) };

					if (DCAbilityCDO->GetActivationPolicy() == EDCAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	// キャッシュしたアクティブにすべきアビリティの実行を試みる

	for (const auto& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	// リリース入力を行うアビリティをすべて実行する

	for (const auto& SpecHandle : InputReleasedSpecHandles)
	{
		if (auto* AbilitySpec{ FindAbilitySpecFromHandle(SpecHandle) })
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	// 配列をリセットする

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UDCAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}
