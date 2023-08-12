// Copyright (C) 2023 owoDra

#pragma once

#include "AbilitySystemComponent.h"

#include "DCAbilitySystemComponent.generated.h"

class AActor;
class UDCGameplayAbility;
struct FGameplayTag;
struct FGameplayAbilitySpec;
struct FGameplayAbilitySpecHandle;


/**
 * AbilitySystem 全般を管理するコンポーネント
 */
UCLASS()
class DUALITYCORE_API UDCAbilitySystemComponent : public UAbilitySystemComponent
{
public:
	GENERATED_BODY()
public:
	UDCAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;


protected:
	//
	// 押し込み入力を受けたアビリティのハンドル
	//
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	//
	// リリース入力を受けたアビリティのハンドル
	//
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	//
	// 長押し入力を受けたアビリティのハンドル
	//
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

public:
	/**
	 * 特定の関数に基づいてアビリティがキャンセル可能か検証し、アビリティのキャンセルを実行する
	 */
	typedef TFunctionRef<bool(const UDCGameplayAbility* DCAbility, FGameplayAbilitySpecHandle Handle)> TShouldCancelAbilityFunc;
	void CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility);

	/**
	 * 入力によってアクティブになったアビリティをキャンセルする
	 */
	void CancelInputActivatedAbilities(bool bReplicateCancelAbility);

	/**
	 * アビリティに紐づけられた押し込み入力が行われたことを知らせ、処理が必要なアビリティを登録する
	 */
	void AbilityInputTagPressed(const FGameplayTag& InputTag);

	/**
	 * アビリティに紐づけられたリリース入力が行われたことを知らせ、処理が必要なアビリティを登録する
	 */
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	/**
	 * アビリティに紐づけられた処理が必要なアビリティを実行する
	 */
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

	/**
	 * アビリティに紐づけられた入力をすべて削除する
	 */
	void ClearAbilityInput();

};
