// Copyright (C) 2023 owoDra

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "DCGlobalAbilitySystem.generated.h"

class UDCAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
struct FGameplayAbilitySpecHandle;


/**
 * 登録された AbilitySystemComponent に対して追加した GameplayAbility のハンドルリスト
 */
USTRUCT()
struct FGlobalAppliedAbilityList
{
public:
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<UDCAbilitySystemComponent*, FGameplayAbilitySpecHandle> Handles;

public:
	/**
	 * 特定の AbilitySystemComponent に GameplayAbility を追加する
	 */
	void AddToASC(TSubclassOf<UGameplayAbility> Ability, UDCAbilitySystemComponent* ASC);

	/**
	 * 特定の AbilitySystemComponent に追加した GameplayAbility をすべて削除する
	 */
	void RemoveFromASC(UDCAbilitySystemComponent* ASC);

	/**
	 * 全ての AbilitySystemComponent に追加した GameplayAbility をすべて削除する
	 */
	void RemoveFromAll();
};


/**
 * 登録された AbilitySystemComponent に対して追加した GameplayEffect のハンドルリスト
 */
USTRUCT()
struct FGlobalAppliedEffectList
{
public:
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<UDCAbilitySystemComponent*, FActiveGameplayEffectHandle> Handles;

public:
	/**
	 * 特定の AbilitySystemComponent に GameplayEffect を追加する
	 */
	void AddToASC(TSubclassOf<UGameplayEffect> Effect, UDCAbilitySystemComponent* ASC);

	/**
	 * 特定の AbilitySystemComponent に追加した GameplayEffect をすべて削除する
	 */
	void RemoveFromASC(UDCAbilitySystemComponent* ASC);

	/**
	 * 全ての AbilitySystemComponent に追加した GameplayEffect をすべて削除する
	 */
	void RemoveFromAll();
};


/**
 * ワールドに存在する登録された全ての AbilitySystemComponent に対しての処理を行うサブシステム
 */
UCLASS()
class UDCGlobalAbilitySystem : public UWorldSubsystem
{
public:
	GENERATED_BODY()
public:
	UDCGlobalAbilitySystem() {}

public:
	/**
	 * 登録された全ての AbilitySystemComponent に GameplayAbility を追加する
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="DC")
	void ApplyAbilityToAll(TSubclassOf<UGameplayAbility> Ability);

	/**
	 * 登録された全ての AbilitySystemComponent に GameplayEffect を追加する
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="DC")
	void ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect);

	/**
	 * 登録された全ての AbilitySystemComponent に追加した GameplayAbility をすべて削除する
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DC")
	void RemoveAbilityFromAll(TSubclassOf<UGameplayAbility> Ability);

	/**
	 * 登録された全ての AbilitySystemComponent に追加した GameplayEffect をすべて削除する
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DC")
	void RemoveEffectFromAll(TSubclassOf<UGameplayEffect> Effect);

	/**
	 * サブシステムに AbilitySystemComponent を登録する
	 */
	void RegisterASC(UDCAbilitySystemComponent* ASC);

	/**
	 * サブシステムに AbilitySystemComponent を解除する
	 */
	void UnregisterASC(UDCAbilitySystemComponent* ASC);

private:
	//
	// 追加したすべての GameplayAbility のハンドルリスト
	//
	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGlobalAppliedAbilityList> AppliedAbilities;

	//
	// 追加したすべての GameplayEffect のハンドルリスト
	//
	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FGlobalAppliedEffectList> AppliedEffects;

	//
	// 登録したすべての AbilitySystemComponent
	//
	UPROPERTY()
	TArray<TObjectPtr<UDCAbilitySystemComponent>> RegisteredASCs;
};
