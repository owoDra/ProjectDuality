// Copyright (C) 2023 owoDra

#pragma once

#include "AttributeSet.h"

#include "Delegates/Delegate.h"

#include "DCAttributeSet.generated.h"

class UWorld;
class UDCAbilitySystemComponent;


/**
 * アトリビュートの変数、Getter/Setter 関数、初期化の定義を簡略化するためのマクロ
 *
 * 例:
 *		ATTRIBUTE_ACCESSORS(UDCXXXSet, YYY)
 * 
 * 上記のように書くことで以下が生成される:
 *		static FGameplayAttribute GetYYYAttribute();
 *		float GetYYY() const;
 *		void SetYYY(float NewVal);
 *		void InitYYY(float NewVal);
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 *	アトリビュートの変更を知らせるためのデリゲート
 */
DECLARE_MULTICAST_DELEGATE_FourParams(FDCAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec& /*EffectSpec*/, float /*EffectMagnitude*/);


/**
 *	アトリビュートを定義し扱うためのクラス
 */
UCLASS()
class DUALITYCORE_API UDCAttributeSet : public UAttributeSet
{
public:
	GENERATED_BODY()
public:
	UDCAttributeSet() {}

public:
	UWorld* GetWorld() const override;

	/**
	 * この AttributeSet が登録されている AbilitySystemComponent を取得する
	 */
	UDCAbilitySystemComponent* GetDCAbilitySystemComponent() const;
};
