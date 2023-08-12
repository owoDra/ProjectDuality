// Copyright (C) 2023 owoDra

#include "DCCharacterCameraComponent.h"

#include "Camera/DCCameraMode.h"
#include "Character/DCData_Character.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCCharacterCameraComponent)


UDCCharacterCameraComponent::UDCCharacterCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(false);
}


void UDCCharacterCameraComponent::OnRegister()
{
	Super::OnRegister();

	if (!CameraModeStack)
	{
		CameraModeStack = NewObject<UDCCameraModeStack>(this);
		check(CameraModeStack);
	}

	CurrentLookAtRotation = GetComponentRotation();
}

void UDCCharacterCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);
	ComputeCameraView(DeltaTime, DesiredView);
}

void UDCCharacterCameraComponent::OnCharacterDataApplied(const UDCData_Character* CharacterData)
{
	check(CharacterData);
	check(CharacterData->DefaultCameraMode);

	PushCameraMode(CharacterData->DefaultCameraMode);
}


void UDCCharacterCameraComponent::ComputeCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	auto* PlayerController{ GetOwnerController() };
	check(PlayerController);
	
	FDCCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	ComputeLookAt(DeltaTime, CameraModeView);
	
	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);
	PlayerController->SetControlRotation(CameraModeView.ControlRotation);
	FieldOfView = CameraModeView.FieldOfView;

	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
}

void UDCCharacterCameraComponent::ComputeLookAt(float DeltaTime, FDCCameraModeView& CameraModeView)
{
	if (LookAtTarget.IsValid())
	{
		static constexpr auto LookInterpSpeed{ 15.0f };

		TargetLookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraModeView.Location, LookAtTarget->GetComponentLocation());
		CurrentLookAtRotation = FMath::RInterpTo(CurrentLookAtRotation, TargetLookAtRotation, DeltaTime, LookInterpSpeed);
		CameraModeView.Rotation = CurrentLookAtRotation;
	}
	else
	{
		static constexpr auto UnLookInterpSpeed{ 25.0f };

		CurrentLookAtRotation = FMath::RInterpTo(CurrentLookAtRotation, CameraModeView.Rotation, DeltaTime, UnLookInterpSpeed);
		CameraModeView.Rotation = CurrentLookAtRotation;
	}
}


APlayerController* UDCCharacterCameraComponent::GetOwnerController() const
{
	if (auto* TargetPawn{ Cast<APawn>(GetOwner()) })
	{
		return TargetPawn->GetController<APlayerController>();
	}

	return nullptr;
}

UDCCharacterCameraComponent* UDCCharacterCameraComponent::FindCameraComponent(const APawn* Pawn)
{
	return (Pawn ? Pawn->FindComponentByClass<UDCCharacterCameraComponent>() : nullptr);
}

void UDCCharacterCameraComponent::GetBlendInfo(float& OutWeightOfTopLayer) const
{
	check(CameraModeStack);
	CameraModeStack->GetBlendInfo(OutWeightOfTopLayer);
}

void UDCCharacterCameraComponent::PushCameraMode(TSubclassOf<UDCCameraMode> CameraModeClass)
{
	check(CameraModeStack);
	CameraModeStack->PushCameraMode(CameraModeClass);
}

void UDCCharacterCameraComponent::SetLookAtTarget(UPrimitiveComponent* TargetComponent)
{
	if (TargetComponent && (TargetComponent != LookAtTarget))
	{
		LookAtTarget = TargetComponent;
	}
}
