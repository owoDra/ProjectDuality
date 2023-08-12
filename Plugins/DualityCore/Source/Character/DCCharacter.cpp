// Copyright (C) 2023 owoDra

#include "DCCharacter.h"

#include "Player/DCPlayerController.h"
#include "Player/DCPlayerState.h"
#include "Player/DCLocalPlayer.h"
#include "Ability/DCAbilitySystemComponent.h"
#include "Ability/DCData_AbilitySet.h"
#include "Character/Component/DCCharacterMovementComponent.h"
#include "Character/DCCharacterComponentInterface.h"
#include "Character/DCData_Character.h"

#include "Components/CapsuleComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCCharacter)


ADCCharacter::ADCCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDCCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	SetReplicates(false);

	GetCapsuleComponent()->InitCapsuleSize(100.0f, 100.0f);

	DCAbilitySystem = ObjectInitializer.CreateDefaultSubobject<UDCAbilitySystemComponent>(this, TEXT("DCAbilitySystem"));
	DCAbilitySystem->SetIsReplicated(false);

	DCCharacterMovement = Cast<UDCCharacterMovementComponent>(GetCharacterMovement());
}


void ADCCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitializeAbilitySystem();

	ApplyCharacterData();
}

void ADCCharacter::SetCharacterData(const UDCData_Character* InCharacterData)
{
	if (CharacterData != InCharacterData)
	{
		CharacterData = InCharacterData;

		ApplyCharacterData();
	}
}

void ADCCharacter::InitializeAbilitySystem()
{
	check(DCAbilitySystem);

	DCAbilitySystem->InitAbilityActorInfo(this, this);
}

void ADCCharacter::ApplyCharacterData()
{
	// コンポーネントに CharacterData の適用を知らせる

	check(CharacterData);

	ForEachComponent<UActorComponent>(
		false, [this](UActorComponent* ChildComponent)
		{
			if (auto * Interface{ Cast<IDCCharacterComponentInterface>(ChildComponent) })
			{
				Interface->OnCharacterDataApplied(CharacterData);
			}
		}
	);

	// AbilitySystemComponent に AbilitySet を適用

	check(DCAbilitySystem);

	GrantedHandles.TakeFromAbilitySystem(DCAbilitySystem);

	for (auto& AbilitySet : CharacterData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(DCAbilitySystem, &GrantedHandles, this);
		}
	}
}


ADCPlayerController* ADCCharacter::GetDCPlayerController() const
{
	return CastChecked<ADCPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

ADCPlayerState* ADCCharacter::GetDCPlayerState() const
{
	return CastChecked<ADCPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UDCAbilitySystemComponent* ADCCharacter::GetDCAbilitySystemComponent() const
{
	return DCAbilitySystem;
}

UAbilitySystemComponent* ADCCharacter::GetAbilitySystemComponent() const
{
	return DCAbilitySystem;
}


void ADCCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const auto* DCASC{ GetDCAbilitySystemComponent() })
	{
		DCASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool ADCCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (const auto * DCASC{ GetDCAbilitySystemComponent() })
	{
		return DCASC->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool ADCCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const auto * DCASC{ GetDCAbilitySystemComponent() })
	{
		return DCASC->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool ADCCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const auto * DCASC{ GetDCAbilitySystemComponent() })
	{
		return DCASC->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}
