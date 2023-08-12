
// Copyright (C) 2023 owoDra

#pragma once

#include "EnhancedInputComponent.h"

#include "DCInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UDCData_InputConfig;


/**
 * プレイヤーのキー入力とそれに応じたアクションの紐づけなどの管理を行うコンポーネント
 */
UCLASS(Config = Input)
class UDCInputComponent : public UEnhancedInputComponent
{
public:
	GENERATED_BODY()

public:
	UDCInputComponent(const FObjectInitializer& ObjectInitializer) {}
	
public:
	/**
	 * プレイヤーのキー入力のマッピング情報を追加する
	 */
	void AddInputMappings(const UDCData_InputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;

	/**
	 * プレイヤーのキー入力のマッピング情報を削除する
	 */
	void RemoveInputMappings(const UDCData_InputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;

	/**
	 * プレイヤーのキー入力と通常のアクションの紐づけを行う
	 * (移動、視点移動 など)
	 */
	template<class UserClass, typename FuncType>
	void BindNativeAction(const UDCData_InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	/**
	 * プレイヤーのキー入力とアビリティアクションの紐づけを行う
	 * (スキル、ダッシュ　など)
	 */
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UDCData_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

	/**
	 * プレイヤーのキー入力の紐づけを削除する
	 */
	void RemoveBinds(TArray<uint32>& BindHandles);
};
