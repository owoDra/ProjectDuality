// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Interface.h"

#include "DCCharacterComponentInterface.generated.h"

class UDCData_Character;


/**
 *  Character に追加機能を付与するコンポーネントに実装するインターフェイス
 *  
 *  キャラクターのデータ更新に応じた処理を行う
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UDCCharacterComponentInterface : public UInterface
{
public:
	GENERATED_BODY()

};

class DUALITYCORE_API IDCCharacterComponentInterface
{
public:
	GENERATED_BODY()

public:
	/**
	 * キャラクターの CharacterData が更新されたときに実行され、それぞれのコンポーネントのデータを更新する
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void OnCharacterDataApplied(const UDCData_Character* CharacterData) {}

};
