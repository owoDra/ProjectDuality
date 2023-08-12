// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/DataAsset.h"

#include "DCData_InputConfig.generated.h"

class UInputAction;
struct FGameplayTag;


/**
 * 使用するすべての InputAction と GameplayTag を関連づけるためデータアセット
 */
UCLASS(BlueprintType, Const)
class UDCData_InputConfig : public UDataAsset
{
public:
	GENERATED_BODY()

public:
	UDCData_InputConfig(const FObjectInitializer& ObjectInitializer) {}

public:
	/**
	 * 指定した GameplayTag に関連する通常の InputAction を検索する
	 */
	UFUNCTION(BlueprintCallable, Category = "Pawn")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	/**
	 * 指定した GameplayTag に関連するアビリティ用の InputAction を検索する
	 */
	UFUNCTION(BlueprintCallable, Category = "Pawn")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	//
	// 通常の InputAction と GameplayTag を関連づけるためマッピングリスト
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction", Categories = "InputTag", ForceInlineRow))
	TMap<TObjectPtr<const UInputAction>, FGameplayTag> NativeInputActions;

	//
	// アビリティ用の InputAction と GameplayTag を関連づけるためマッピングリスト
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction", Categories = "InputTag", ForceInlineRow))
	TMap<TObjectPtr<const UInputAction>, FGameplayTag> AbilityInputActions;
};
