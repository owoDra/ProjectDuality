// Copyright (C) 2023 owoDra

#pragma once

#include "Abilities/GameplayAbility.h"

#include "DCGameplayAbility.generated.h"

class UDCAbilityCost;
class ADCPlayerController;
class ADCCharacter;
class UDCCharacterCameraComponent;


/**
 * アビリティの発動条件
 */
UENUM(BlueprintType)
enum class EDCAbilityActivationPolicy : uint8
{
	OnInputTriggered,	// 入力がされたとき

	WhileInputActive,	// 入力がされている間、継続的に
	
	OnSpawn				// 付与されて Avatar が設定されたとき
};


/**
 *	キャラクターなどに追加の能力を付与するためのクラス
 */
UCLASS(Abstract, HideCategories = Input)
class DUALITYCORE_API UDCGameplayAbility : public UGameplayAbility
{
public:
	GENERATED_BODY()

	friend class UDCAbilitySystemComponent;

public:
	UDCGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//
	// アビリティの発動条件
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Activation")
	EDCAbilityActivationPolicy ActivationPolicy;

public:
	/**
	 * アビリティの発動条件を取得する
	 */
	EDCAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	/**
	 * アビリティの付与時に実行し、アビリティの発動を試みる
	 */
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

protected:
	/**
	 * アビリティの付与時に実行
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability", DisplayName = "OnGiveAbility")
	void K2_OnGiveAbility();
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	/**
	 * アビリティの削除時に実行
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability", DisplayName = "OnRemoveAbility")
	void K2_OnRemoveAbility();
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	/**
	 * Avatar 登録時に実行
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability", DisplayName = "OnPawnAvatarSet")
	void K2_OnAvatarSet();
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

public:
	/**
	 * ActorInfo から DCAbilitySystemComponent を取得
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	UDCAbilitySystemComponent* GetDCAbilitySystemComponentFromActorInfo() const;

	/**
	 * ActorInfo から DCPlayerController を取得
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	ADCPlayerController* GetDCPlayerControllerFromActorInfo() const;

	/**
	 * ActorInfo から Controller を取得
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	AController* GetControllerFromActorInfo() const;

	/**
	 * ActorInfo から DCCharacter を取得
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	ADCCharacter* GetDCCharacterFromActorInfo() const;

	/**
	 * ActorInfo から DCCharacterCameraComponent を取得
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	UDCCharacterCameraComponent* GetDCCharacterCameraComponentFromActorInfo() const;
};
