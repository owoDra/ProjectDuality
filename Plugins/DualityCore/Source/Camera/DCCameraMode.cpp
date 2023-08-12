// Copyright (C) 2023 owoDra

#include "DCCameraMode.h"

#include "Character/Component/DCCharacterCameraComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCCameraMode)


#pragma region CameraModeView

void FDCCameraModeView::Blend(const FDCCameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.0f)
	{
		return;
	}
	else if (OtherWeight >= 1.0f)
	{
		*this = Other;
		return;
	}

	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	const auto DeltaRotation{ (Other.Rotation - Rotation).GetNormalized() };
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const auto DeltaControlRotation{ (Other.ControlRotation - ControlRotation).GetNormalized() };
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

#pragma endregion


#pragma region CameraMode

void UDCCameraMode::SetActivationState(EDCCameraModeActivationState NewActivationState)
{
	if (ActivationState == NewActivationState)
	{
		return;
	}

	ActivationState = NewActivationState;

	if (ActivationState == EDCCameraModeActivationState::PreActivate)
	{
		PreActivateMode();
	}
	else if (ActivationState == EDCCameraModeActivationState::Activated)
	{
		PostActivateMode();
	}
	else if (ActivationState == EDCCameraModeActivationState::PreDeactevate)
	{
		PreDeactivateMode();
	}
	else if (ActivationState == EDCCameraModeActivationState::Deactevated)
	{
		PostDeactivateMode();
	}
}


FVector UDCCameraMode::GetPivotLocation() const
{
	const auto* TargetActor{ GetTargetActor() };
	check(TargetActor);

	if (const auto* TargetPawn{ Cast<APawn>(TargetActor) })
	{
		// Height adjustments for characters to account for crouching.
		if (const auto* TargetCharacter{ Cast<ACharacter>(TargetPawn) })
		{
			const auto* TargetCharacterCDO{ TargetCharacter->GetClass()->GetDefaultObject<ACharacter>() };
			check(TargetCharacterCDO);

			const auto* CapsuleComp{ TargetCharacter->GetCapsuleComponent() };
			check(CapsuleComp);

			const auto* CapsuleCompCDO{ TargetCharacterCDO->GetCapsuleComponent() };
			check(CapsuleCompCDO);

			const auto DefaultHalfHeight{ CapsuleCompCDO->GetUnscaledCapsuleHalfHeight() };
			const auto ActualHalfHeight{ CapsuleComp->GetUnscaledCapsuleHalfHeight() };
			const auto HeightAdjustment{ (DefaultHalfHeight - ActualHalfHeight) + TargetCharacterCDO->BaseEyeHeight };

			return TargetCharacter->GetActorLocation() + (FVector::UpVector * HeightAdjustment);
		}

		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator UDCCameraMode::GetPivotRotation() const
{
	const auto* TargetActor{ GetTargetActor() };
	check(TargetActor);

	if (const auto* TargetPawn{ Cast<APawn>(TargetActor) })
	{
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

void UDCCameraMode::UpdateView(float DeltaTime)
{
	auto PivotLocation{ GetPivotLocation() };
	auto PivotRotation{ GetPivotRotation() };

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

void UDCCameraMode::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.0f)
	{
		BlendAlpha += (DeltaTime / BlendTime);
		BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	const auto Exponent{ (BlendExponent > 0.0f) ? BlendExponent : 1.0f };

	switch (BlendFunction)
	{
	case EDCCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;

	case EDCCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;

	case EDCCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;

	case EDCCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;

	default:
		checkf(false, TEXT("UDCCameraMode::UpdateBlending: 無効な BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

void UDCCameraMode::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);
	UpdateBlending(DeltaTime);
}

void UDCCameraMode::SetBlendWeight(float Weight)
{
	BlendWeight = FMath::Clamp(Weight, 0.0f, 1.0f);

	const auto InvExponent{ (BlendExponent > 0.0f) ? (1.0f / BlendExponent) : 1.0f };

	switch (BlendFunction)
	{
	case EDCCameraModeBlendFunction::Linear:
		BlendAlpha = BlendWeight;
		break;

	case EDCCameraModeBlendFunction::EaseIn:
		BlendAlpha = FMath::InterpEaseIn(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	case EDCCameraModeBlendFunction::EaseOut:
		BlendAlpha = FMath::InterpEaseOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	case EDCCameraModeBlendFunction::EaseInOut:
		BlendAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	default:
		checkf(false, TEXT("UDCCameraMode::SetBlendWeight: 無効な BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}


UDCCharacterCameraComponent* UDCCameraMode::GetDCCharacterCameraComponent() const
{
	return CastChecked<UDCCharacterCameraComponent>(GetOuter());
}

UWorld* UDCCameraMode::GetWorld() const
{
	return HasAnyFlags(RF_ClassDefaultObject) ? nullptr : GetOuter()->GetWorld();
}

AActor* UDCCameraMode::GetTargetActor() const
{
	const auto* DCCharacterCameraComponent{ GetDCCharacterCameraComponent() };
	check(DCCharacterCameraComponent);

	return DCCharacterCameraComponent->GetOwner();
}

#pragma endregion


#pragma region CameraModeStack

UDCCameraMode* UDCCameraModeStack::GetCameraModeInstance(TSubclassOf<UDCCameraMode> CameraModeClass)
{
	check(CameraModeClass);

	// 既にインスタンスが作成されているか確認
	for (auto& CameraMode : CameraModeInstances)
	{
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	// 既にインスタンスが作成されていない場合は作成する
	auto* NewCameraMode{ NewObject<UDCCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags) };
	check(NewCameraMode);

	// インスタンスを配列に追加し後から参照できるようにする
	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}


void UDCCameraModeStack::UpdateStack(float DeltaTime)
{
	const auto StackSize{ CameraModeStack.Num() };

	// Stack が 0 以下(つまり空)の場合はスキップ
	if (StackSize <= 0)
	{
		return;
	}

	int32 RemoveCount{ 0 };
	int32 RemoveIndex{ INDEX_NONE };

	// Stack 内の CameraMode をすべて更新する
	for (int32 StackIndex{ 0 }; StackIndex < StackSize; ++StackIndex)
	{
		auto& CameraMode{ CameraModeStack[StackIndex] };
		check(CameraMode);

		CameraMode->UpdateCameraMode(DeltaTime);

		// CameraMode の BlendWeight が 1.0 以上(つまり Blend が完了済み)かどうか
		if (CameraMode->GetBlendWeight() >= 1.0f)
		{
			// Blend が完了している CameraMode より前に追加された最初の項目の Index とそこから何個の項目を削除するか記録
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);

			// Index 0 の Blend が完了したということは完全に Active になったということなので
			// ActivationState を Activated に設定
			if (StackIndex == 0)
			{
				CameraMode->SetActivationState(EDCCameraModeActivationState::Activated);
			}
			break;
		}
	}

	// Blend が完了し除外しても問題ない項目があるか
	if (RemoveCount > 0)
	{
		// 記録した項目を削除する
		for (int32 StackIndex{ RemoveIndex }; StackIndex < StackSize; ++StackIndex)
		{
			auto& CameraMode{ CameraModeStack[StackIndex] };
			check(CameraMode);

			// ActivationState を Deactevated 設定する
			CameraMode->SetActivationState(EDCCameraModeActivationState::Deactevated);
		}

		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void UDCCameraModeStack::BlendStack(FDCCameraModeView& OutCameraModeView) const
{
	const auto LastIndex{ static_cast<int32>(CameraModeStack.Num() - 1) };

	// Stack が空の場合はスキップ
	if (LastIndex < 0)
	{
		return;
	}

	// Stack の一番最後の CameraMode から順番に Blend する
	UDCCameraMode* CameraMode = CameraModeStack[LastIndex];
	check(CameraMode);

	OutCameraModeView = CameraMode->GetCameraModeView();

	for (int32 StackIndex{ LastIndex - 1 }; StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		OutCameraModeView.Blend(CameraMode->GetCameraModeView(), CameraMode->GetBlendWeight());
	}
}

void UDCCameraModeStack::PushCameraMode(TSubclassOf<UDCCameraMode> CameraModeClass)
{
	// 新しく適応する CameraMode が有効かどうか
	if (!CameraModeClass)
	{
		return;
	}

	// CameraMode からインスタンスを作成、またはキャッシュから取得
	auto* CameraMode{ GetCameraModeInstance(CameraModeClass) };
	check(CameraMode);

	auto StackSize{ CameraModeStack.Num() };

	// 追加しようとしている CameraMode が既に Stack の一番上(つまり有効化済み、または有効か途中)かどうか
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		return;
	}

	// Stack に既にあるかどうかを確認し、Stack 内の CameraMode の BlendWeight から
	// 新しく追加する CameraMode の BlendWeight を決める
	auto ExistingStackIndex{ static_cast<int32>(INDEX_NONE) };
	auto ExistingStackContribution{ 1.0f };
	auto bCameraModeExisted{ false };

	for (int32 StackIndex{ 0 }; StackIndex < StackSize; ++StackIndex)
	{
		const auto bSameCameraMode{ CameraModeStack[StackIndex] == CameraMode };

		// 新しく追加する CameraMode が存在するかわかっていない間だけ ExistingStackContribution の計算を続ける
		if (bCameraModeExisted == false)
		{
			if (bSameCameraMode)
			{
				ExistingStackIndex = StackIndex;
				ExistingStackContribution *= CameraMode->GetBlendWeight();

				// CameraMode が存在しているとわかったのでフラグを立てる
				bCameraModeExisted = true;
			}
			else
			{
				ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->GetBlendWeight());
			}
		}

		// 新しく追加する CameraMode と同じではないときにだけ、その CameraMode の ActivationState を PreDeactevate にする
		if (bSameCameraMode == false)
		{
			CameraModeStack[StackIndex]->SetActivationState(EDCCameraModeActivationState::PreDeactevate);
		}
	}

	// Stack に既にないなら全体の BlendWeight に影響しないので 0.0 に設定
	if (ExistingStackIndex == INDEX_NONE)
	{
		ExistingStackContribution = 0.0f;
	}

	// Stack に既にあるなら削除する
	else
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}

	// 新しく追加する CameraMode の BlendWeight を決める
	const auto bShouldBlend{ (CameraMode->GetBlendTime() > 0.0f) && (StackSize > 0) };
	const auto BlendWeight{ bShouldBlend ? ExistingStackContribution : 1.0f };

	CameraMode->SetBlendWeight(BlendWeight);

	// Stack の最初に挿入
	CameraModeStack.Insert(CameraMode, 0);

	// Stack の最後の項目の BlendWeight を 1.0 (100%)にする
	CameraModeStack.Last()->SetBlendWeight(1.0f);

	// CameraMode の ActivationState を PreActivate に設定
	CameraMode->SetActivationState(EDCCameraModeActivationState::PreActivate);
}

void UDCCameraModeStack::EvaluateStack(float DeltaTime, FDCCameraModeView& OutCameraModeView)
{
	UpdateStack(DeltaTime);
	BlendStack(OutCameraModeView);
}

void UDCCameraModeStack::GetBlendInfo(float& OutWeightOfTopLayer) const
{
	if (CameraModeStack.IsEmpty())
	{
		OutWeightOfTopLayer = 1.0f;
	}
	else
	{
		const auto& TopEntry{ CameraModeStack[0] };
		check(TopEntry);
		OutWeightOfTopLayer = TopEntry->GetBlendWeight();
	}
}

#pragma endregion
