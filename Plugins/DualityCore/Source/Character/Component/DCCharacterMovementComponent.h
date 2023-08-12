// Copyright (C) 2023 owoDra

#pragma once

#include "GameFramework/CharacterMovementComponent.h"

#include "Character/DCCharacterComponentInterface.h"

#include "DCCharacterMovementComponent.generated.h"

class ADCCharacter;

/**
 * UDCCharacterMovementComponent
 *
 *	Character の Movement 関係の処理を担当するコンポーネント
 */
UCLASS()
class DUALITYCORE_API UDCCharacterMovementComponent 
	: public UCharacterMovementComponent
	, public IDCCharacterComponentInterface
{
public:
	GENERATED_BODY()
public:
	UDCCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

public:
	/**
	 *	このコンポーネントを所有する DCCharacter を取得する
	 */
	ADCCharacter* GetDCCharacterOwner() const { return Cast<ADCCharacter>(CharacterOwner); }

	/**
	 *	Character からこのコンポーネントを取得する
	 */
	UFUNCTION(BlueprintPure, Category = "Character")
	static UDCCharacterMovementComponent* FindDCCharacterMovementComponent(const ADCCharacter* Character);
};
