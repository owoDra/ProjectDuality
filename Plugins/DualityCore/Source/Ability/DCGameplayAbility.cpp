// Copyright (C) 2023 owoDra

#include "Ability/DCGameplayAbility.h"

#include "Ability/DCAbilitySystemComponent.h"
#include "Character/Component/DCCharacterCameraComponent.h"
#include "Player/DCPlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCGameplayAbility)


UDCGameplayAbility::UDCGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;

	ActivationPolicy = EDCAbilityActivationPolicy::OnInputTriggered;
}


void UDCGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	// ActorInfo が無効、または既にアクティブならば処理を中断

	if (!ActorInfo || Spec.IsActive())
	{
		return;
	}

	// 発動条件を検証

	const auto bActivationModeIsPredicting{ Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting };
	const auto bActivationPolicyIsOnSpawn{ ActivationPolicy == EDCAbilityActivationPolicy::OnSpawn };

	if (bActivationModeIsPredicting || !bActivationPolicyIsOnSpawn)
	{
		return;
	}

	// Avatar と AbilitySystemComponent を検証

	auto* ASC{ ActorInfo->AbilitySystemComponent.Get() };
	const auto* AvatarActor{ ActorInfo->AvatarActor.Get() };

	if (!(ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f)))
	{
		return;
	}

	// 権限を検証

	const auto bIsLocalExecution{ (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly) };
	const auto bIsServerExecution{ (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated) };

	const auto bClientShouldActivate{ ActorInfo->IsLocallyControlled() && bIsLocalExecution };
	const auto bServerShouldActivate{ ActorInfo->IsNetAuthority() && bIsServerExecution };

	if (bClientShouldActivate || bServerShouldActivate)
	{
		ASC->TryActivateAbility(Spec.Handle);
	}
}

void UDCGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	K2_OnGiveAbility();

	TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

void UDCGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	K2_OnRemoveAbility();

	Super::OnRemoveAbility(ActorInfo, Spec);
}

void UDCGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	TryActivateAbilityOnSpawn(ActorInfo, Spec);

	K2_OnAvatarSet();
}


UDCAbilitySystemComponent* UDCGameplayAbility::GetDCAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UDCAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

ADCPlayerController* UDCGameplayAbility::GetDCPlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ADCPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

AController* UDCGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (auto* PlayerController{ CurrentActorInfo->PlayerController.Get() })
		{
			return PlayerController;
		}

		auto* TestActor{ CurrentActorInfo->OwnerActor.Get() };
		while (TestActor)
		{
			if (auto* Controller{ Cast<AController>(TestActor) })
			{
				return Controller;
			}

			if (auto* Pawn{ Cast<APawn>(TestActor) })
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}

ADCCharacter* UDCGameplayAbility::GetDCCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ADCCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

UDCCharacterCameraComponent* UDCGameplayAbility::GetDCCharacterCameraComponentFromActorInfo() const
{
	return (CurrentActorInfo ? UDCCharacterCameraComponent::FindCameraComponent(Cast<APawn>(CurrentActorInfo->AvatarActor.Get())) : nullptr);
}
