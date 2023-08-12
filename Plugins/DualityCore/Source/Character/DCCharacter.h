// Copyright (C) 2023 owoDra

#pragma once

#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"

#include "DCCharacter.generated.h"

class UDCCharacterMovementComponent;
class UDCAbilitySystemComponent;
class ADCPlayerController;
class ADCPlayerState;
class UDCData_Character;


/**
 *	プレイヤーが操作するキャラクターのクラス
 */
UCLASS()
class DUALITYCORE_API ADCCharacter 
	: public ACharacter
	, public IAbilitySystemInterface
	, public IGameplayTagAssetInterface
{
public:
	GENERATED_BODY()
public:
	explicit ADCCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//
	// Character の移動を管理するコンポーネント。
	//
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UDCCharacterMovementComponent> DCCharacterMovement;

	//
	// Character の移動を管理するコンポーネント。
	//
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UDCAbilitySystemComponent> DCAbilitySystem;

	//
	// Character の移動を管理するコンポーネント。
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Transient, Category = "Config")
	TObjectPtr<const UDCData_Character> CharacterData{ nullptr };

	//
	// AbilitySystemComponent に追加した GameplayAbility, GameplayEffect, Attribute のハンドル
	//
	UPROPERTY(Transient)
	FDCAbilitySet_GrantedHandles GrantedHandles;

public:
	virtual void PostInitializeComponents() override;

	/**
	 *	CharacterData を設定する
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void SetCharacterData(const UDCData_Character* InCharacterData);

protected:
	/**
	 *	キャラクターの AbilitySystemComponent を初期化する
	 */
	virtual void InitializeAbilitySystem();

	/**
	 *	CharacterData を適用する
	 */
	virtual void ApplyCharacterData();


public:
	/**
	 *	キャラクターを所有する DCPlayerController を返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	ADCPlayerController* GetDCPlayerController() const;

	/**
	 *	キャラクターを所有する DCPlayerState を返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	ADCPlayerState* GetDCPlayerState() const;

	/**
	 *	キャラクターが所有する DCCharacterMovement を返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	UDCCharacterMovementComponent* GetDCCharacterMovement() const { return DCCharacterMovement; }

	/**
	 *	キャラクターが所有する DCAbilitySystemComponent を返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	virtual UDCAbilitySystemComponent* GetDCAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
};
