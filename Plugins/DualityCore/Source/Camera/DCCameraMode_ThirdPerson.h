// Copyright (C) 2023 owoDra

#pragma once

#include "DCCameraMode.h"

#include "DCCameraMode_ThirdPerson.generated.h"


/**
 * 三人称用の CameraMode
 */
UCLASS(Abstract, Blueprintable)
class UDCCameraMode_ThirdPerson : public UDCCameraMode
{
public:
	GENERATED_BODY()
public:
	UDCCameraMode_ThirdPerson() {}

protected:
	//
	// 支点からのカメラ位置のオフセット
	//
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	FVector TargetOffset { 0.0f, 300.0f, 25.0f };

	//
	// 当たり判定をどれだけ大きく扱うか
	// (本来の当たり判定より大きく扱うことでめり込みを防ぐ)
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionPushOutDistance{ 2.0f };


protected:
	virtual void UpdateView(float DeltaTime) override;

	/**
	 * めり込み回避を考慮した視点情報を更新する
	 */
	void UpdatePreventPenetration(float DeltaTime);

};
