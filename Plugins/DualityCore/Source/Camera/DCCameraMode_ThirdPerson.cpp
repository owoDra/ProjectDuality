// Copyright (C) 2023 owoDra

#include "DCCameraMode_ThirdPerson.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCCameraMode_ThirdPerson)


void UDCCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	Super::UpdateView(DeltaTime);

	// TargetOffset を適用

	View.Location += View.Rotation.RotateVector(TargetOffset);

	// めり込み回避補正を行う

	UpdatePreventPenetration(DeltaTime);
}

void UDCCameraMode_ThirdPerson::UpdatePreventPenetration(float DeltaTime)
{
	const auto* TargetActor{ GetTargetActor() };
	const auto* World{ GetWorld() };

	// トレース用のパラメータを作成

	const auto TraceStart{ GetPivotLocation() };
	const auto TraceEnd{ View.Location };
	const auto TraceChannel{ ECC_Camera };
	const auto SphereShape{ FCollisionShape::MakeSphere(CollisionPushOutDistance) };
	const auto SphereParams{ FCollisionQueryParams(SCENE_QUERY_STAT(CameraPen), false, TargetActor) };

	// トレースを行う

	FHitResult Hit;
	const auto bHit
	{
		World->SweepSingleByChannel(
			Hit,
			TraceStart,
			TraceEnd,
			FQuat::Identity,
			TraceChannel,
			SphereShape,
			SphereParams
		)
	};

	// 障害物にぶつかった場合

	if (bHit)
	{
		View.Location = TraceStart + (TraceEnd - TraceStart) * Hit.Distance;
	}
}
