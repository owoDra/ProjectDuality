// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Object.h"

//#include "Containers/Array.h"
//#include "Engine/World.h"
//#include "GameplayTagContainer.h"
//#include "Math/Rotator.h"
//#include "Math/UnrealMathSSE.h"
//#include "Math/Vector.h"
//#include "Templates/SubclassOf.h"
//#include "UObject/ObjectPtr.h"
//#include "UObject/UObjectGlobals.h"

#include "DCCameraMode.generated.h"

/**
 *	カメラのデフォルト値の定数
 */
namespace DCCameraDefaults
{
	static constexpr auto FOV{ 90.0f };
	static constexpr auto PitchMin{ -89.0f };
	static constexpr auto PitchMax{ 89.0f };
}


/**
 *	CameraMode 間の遷移に使用されるブレンド関数
 */
UENUM(BlueprintType)
enum class EDCCameraModeBlendFunction : uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,

	COUNT	UMETA(Hidden)
};


/**
 *	CameraMode の有効化状態
 */
UENUM(BlueprintType)
enum class EDCCameraModeActivationState : uint8
{
	PreActivate,	// アクティブになる前のブレンド中の状態
	Activated,		// アクティブな状態
	PreDeactevate,	// 非アクティブになる前のブレンド中の状態
	Deactevated,	// 非アクティブな状態

	COUNT	UMETA(Hidden)
};


/**
 *	CameraMode のブレンドに使用される CameraMode によって生成されたデータ
 */
struct FDCCameraModeView
{
public:
	FDCCameraModeView() {}

public:
	FVector Location{ ForceInit };
	FRotator Rotation{ ForceInit };
	FRotator ControlRotation{ ForceInit };
	float FieldOfView{ DCCameraDefaults::FOV };

public:
	void Blend(const FDCCameraModeView& Other, float OtherWeight);
};


/**
 *	カメラの仕様を決めるための CameraMode の基本クラス
 */
UCLASS(Abstract, NotBlueprintable)
class DUALITYCORE_API UDCCameraMode : public UObject
{
public:
	GENERATED_BODY()
public:
	UDCCameraMode() {}
	
public:
	/**
	 *  CameraMode の ActivationState を変更する
	 */
	virtual void SetActivationState(EDCCameraModeActivationState NewActivationState);

protected:
	/**
	 *  CameraMode が Active になる前の Blend が開始する際に呼び出される
	 */
	virtual void PreActivateMode() {};

	/**
	 *  CameraMode の Blend が終了し Active になった際に呼び出される
	 */
	virtual void PostActivateMode() {};

	/**
	 *  CameraMode が Deactive になる前の Blend が開始する際に呼び出される
	 */
	virtual void PreDeactivateMode() {};

	/**
	 *  CameraMode の Blend が終了し Deactive になった際に呼び出される
	 */
	virtual void PostDeactivateMode() {};


protected:
	//
	// CameraMode の最終的な出力データ
	//
	FDCCameraModeView View;

	//
	// カメラの視野角
	//
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView{ DCCameraDefaults::FOV };

	//
	// カメラの回転の Pitch の最小値
	//
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin{ DCCameraDefaults::PitchMin };

	//
	// カメラの回転の Pitch の最大値
	//
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax{ DCCameraDefaults::PitchMax };

	//
	// CameraMode の Blend にかかる時間
	//
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime{ 0.5f };

	//
	// CameraMode のブレンド関数
	//
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	EDCCameraModeBlendFunction BlendFunction{ EDCCameraModeBlendFunction::EaseOut };

	//
	// 曲線の形状を制御するためにブレンド関数で使用される指数
	//
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent{ 4.0f };

	//
	// Blend の計算に用いる値
	//
	UPROPERTY(Transient)
	float BlendAlpha{ 1.0f };

	//
	// CameraMode がどのくらい Blend されたかの値
	// 1.0 以上は Blend が完了されたということになる
	//
	UPROPERTY(Transient)
	float BlendWeight{ 1.0f };

	//
	// CameraMode の有効化状態
	//
	UPROPERTY(Transient)
	EDCCameraModeActivationState ActivationState{ EDCCameraModeActivationState::Deactevated };

	//
	// CameraMode のブレンドで補間をスキップするか
	//
	UPROPERTY(transient)
	bool bResetInterpolation{ false };

protected:
	/**
	 *  カメラの支点の位置を取得
	 */
	virtual FVector GetPivotLocation() const;

	/**
	 *  カメラの支点の回転を取得
	 */
	virtual FRotator GetPivotRotation() const;

	/**
	 *  CameraMode の視点情報を更新する
	 */
	virtual void UpdateView(float DeltaTime);

	/**
	 *  CameraMode の視点情報のブレンドを更新する
	 */
	virtual void UpdateBlending(float DeltaTime);

public:
	/**
	 *  CameraMode の更新をする
	 */
	void UpdateCameraMode(float DeltaTime);

	/**
	 *  現在の BlendWeight を設定する
	 */
	void SetBlendWeight(float Weight);

public:
	float GetBlendTime() const { return BlendTime; }
	float GetBlendWeight() const { return BlendWeight; }
	const FDCCameraModeView& GetCameraModeView() const { return View; }

	UDCCharacterCameraComponent* GetDCCharacterCameraComponent() const;

	AActor* GetTargetActor() const;

	virtual UWorld* GetWorld() const override;

};


/**
 * UDCCameraModeStack
 *
 *	CameraMode を Blend するため Stack
 */
UCLASS()
class UDCCameraModeStack : public UObject
{
public:
	GENERATED_BODY()
public:
	UDCCameraModeStack() {}

protected:
	UPROPERTY(transient)
	TArray<TObjectPtr<UDCCameraMode>> CameraModeInstances;

	UPROPERTY(transient)
	TArray<TObjectPtr<UDCCameraMode>> CameraModeStack;

protected:
	/**
	 *	指定した CameraMode のインスタンスを取得する
	 *  無い場合は新たに作成する
	 */
	UDCCameraMode* GetCameraModeInstance(TSubclassOf<UDCCameraMode> CameraModeClass);

	/**
	 *  Stack 内の CameraMode の更新を行う。
	 *	ブレンドが完了した CameraMode を判定して Stack から除外する。
	 */
	void UpdateStack(float DeltaTime);

	/**
	 *	Stack 内の CameraMode の Blend を行う
	 */
	void BlendStack(FDCCameraModeView& OutCameraModeView) const;

public:
	/**
	 *	Stack の最初に新しく CameraMode を追加し Blend を始める
	 */
	void PushCameraMode(TSubclassOf<UDCCameraMode> CameraModeClass);

	/**
	 *	CameraComponent から呼び出され Stack の更新と最終的な出力データを返す
	 */
	void EvaluateStack(float DeltaTime, FDCCameraModeView& OutCameraModeView);

	/**
	 *	現在の Stack の Blend 情報を取得する
	 */
	void GetBlendInfo(float& OutWeightOfTopLayer) const;

};
