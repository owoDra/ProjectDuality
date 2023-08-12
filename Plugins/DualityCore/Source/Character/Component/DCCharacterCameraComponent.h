// Copyright (C) 2023 owoDra

#pragma once

#include "Camera/CameraComponent.h"

#include "Character/DCCharacterComponentInterface.h"

#include "DCCharacterCameraComponent.generated.h"

class UDCCameraModeStack;
class UDCCameraMode;
class UDCData_Character;
struct FDCCameraModeView;
struct FGameplayTag;


/**
 *	プレイヤーが視点操作可能にするための処理を行う
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class DUALITYCORE_API UDCCharacterCameraComponent 
	: public UCameraComponent
	, public IDCCharacterComponentInterface
{
public:
	GENERATED_BODY()
public:
	UDCCharacterCameraComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

public:
	virtual void OnCharacterDataApplied(const UDCData_Character* CharacterData) override;

protected:
	//
	// CameraMode を管理するスタック
	//
	UPROPERTY(Transient)
	TObjectPtr<UDCCameraModeStack> CameraModeStack{ nullptr };

	//
	// 注目するワールドに配置された PrimitiveComponent
	// カメラはこの PrimitiveComponent を見るように回転する
	//
	UPROPERTY(Transient)
	TWeakObjectPtr<UPrimitiveComponent> LookAtTarget{ nullptr };

	// 
	// カメラ位置から LookAtTarget を見る時の目標カメラ回転
	//
	UPROPERTY(Transient)
	FRotator TargetLookAtRotation;

	// 
	// カメラ位置から LookAtTarget を見る時の現在のカメラ回転
	//
	UPROPERTY(Transient)
	FRotator CurrentLookAtRotation;

protected:
	/**
	 * ComputeCameraView
	 *
	 *	Camera の最終的な視点情報を計算する
	 */
	virtual void ComputeCameraView(float DeltaTime, FMinimalViewInfo& DesiredView);

	/**
	 * ComputeLookAt
	 *
	 *	CameraMode から出力された視点情報を LookAtTarget の方を向くように遷移させる
	 */
	virtual void ComputeLookAt(float DeltaTime, FDCCameraModeView& CameraModeView);


public:
	APlayerController* GetOwnerController() const;

	/**
	 * Pawn が所有する DCCharacterCameraComponent を取得する
	 */
	UFUNCTION(BlueprintPure, Category = "Camera")
	static UDCCharacterCameraComponent* FindCameraComponent(const APawn* Pawn);

	/**
	 *	現在の Stack の Blend 情報を取得する
	 */
	UFUNCTION(BlueprintPure, Category = "Camera")
	void GetBlendInfo(float& OutWeightOfTopLayer) const;

	/**
	 *	Stack の最初に新しく CameraMode を追加し Blend を始める
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void PushCameraMode(TSubclassOf<UDCCameraMode> CameraModeClass);

	/**
	 *	注目するターゲットを設定する
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetLookAtTarget(UPrimitiveComponent* TargetComponent);
};
