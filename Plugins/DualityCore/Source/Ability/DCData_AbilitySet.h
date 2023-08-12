// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/DataAsset.h"

#include "GameplayTagContainer.h"

#include "DCData_AbilitySet.generated.h"

class UDCAbilitySystemComponent;
class UDCGameplayAbility;
class UGameplayEffect;
class UAttributeSet;


/**
 *	付与する GameplayAbility のデータ
 */
USTRUCT(BlueprintType)
struct FDCAbilitySet_GameplayAbility
{
public:
	GENERATED_BODY()

public:
	//
	// 付与する GameplayAbility のクラス
	//
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDCGameplayAbility> Ability = nullptr;

	//
	// 付与する GameplayAbility のレベル
	//
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	//
	// 付与する GameplayAbility に紐づける入力の GameplayTag
	// 入力による発動を行わない場合は設定しない
	//
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


/**
 *	付与する GameplayEffect のデータ
 */
USTRUCT(BlueprintType)
struct FDCAbilitySet_GameplayEffect
{
public:
	GENERATED_BODY()

public:
	//
	// 付与する GameplayEffect のクラス
	//
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	//
	// 付与する GameplayEffect のレベル
	//
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};


/**
 *	付与する AttributeSet のデータ
 */
USTRUCT(BlueprintType)
struct FDCAbilitySet_AttributeSet
{
public:
	GENERATED_BODY()

public:
	//
	// 付与する AttributeSet のクラス
	//
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;
};


/**
 *	付与された GameplayAbility, GameplayEffect, AttributeSet のハンドル
 */
USTRUCT(BlueprintType)
struct FDCAbilitySet_GrantedHandles
{
public:
	GENERATED_BODY()

protected:
	//
	// 付与された GameplayAbility のハンドル
	//
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	//
	// 付与された GameplayEffect のハンドル
	//
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	//
	// 付与された AttributeSet の参照を配列に追加する
	//
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;

public:
	/**
	 * 付与された GameplayAbility のハンドルを配列に追加する
	 */
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

	/**
	 * 付与された GameplayEffect のハンドルを配列に追加する
	 */
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);

	/**
	 * 付与された AttributeSet の参照を配列に追加する
	 */
	void AddAttributeSet(UAttributeSet* Set);

	/**
	 * AbilitySpecHandles, GameplayEffectHandles, GrantedAttributeSets をもとにそれぞれ
	 * AbilitySystemComponent から登録解除する
	 */
	void TakeFromAbilitySystem(UDCAbilitySystemComponent* DCASC);
};


/**
 * AbilitySystemComponent に付与する GameplayAbility, GameplayEffect, AttributeSet を定義するデータアセット
 */
UCLASS(BlueprintType, Const)
class UDCData_AbilitySet : public UPrimaryDataAsset
{
public:
	GENERATED_BODY()
public:
	UDCData_AbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
	}

protected:
	//
	// AbilitySystemComponent に付与する GameplayAbility のリスト
	//
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySet", meta = (TitleProperty = Ability))
	TArray<FDCAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	//
	// AbilitySystemComponent に付与する GameplayEffect のリスト
	//
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySet", meta = (TitleProperty = GameplayEffect))
	TArray<FDCAbilitySet_GameplayEffect> GrantedGameplayEffects;

	//
	// AbilitySystemComponent に付与する AttributeSet のリスト
	//
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySet", meta = (TitleProperty = AttributeSet))
	TArray<FDCAbilitySet_AttributeSet> GrantedAttributes;

public:
	/**
	 * 指定した AbilitySystemComponent に定義した GameplayAbility, GameplayEffect, AttributeSet を付与する
	 */
	void GiveToAbilitySystem(UDCAbilitySystemComponent* DCASC, OUT FDCAbilitySet_GrantedHandles* OutGrantedHandles = nullptr, UObject* SourceObject = nullptr) const;
};
