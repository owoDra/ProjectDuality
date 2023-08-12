// Copyright (C) 2023 owoDra

#include "DCCharacterMovementComponent.h"

#include "Character/DCCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCCharacterMovementComponent)


UDCCharacterMovementComponent::UDCCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GravityScale = 1.0f;
	MaxAcceleration = 2400.0f;
	BrakingFrictionFactor = 1.0f;
	BrakingFriction = 6.0f;
	GroundFriction = 8.0f;

	bUseControllerDesiredRotation = true;

	bCanWalkOffLedgesWhenCrouching = false;
	bCanWalkOffLedges = false;

	NavAgentProps.bCanJump = false;
	NavAgentProps.bCanCrouch = false;
	NavAgentProps.bCanFly = false;
	NavAgentProps.bCanSwim = false;

	bUseAccelerationForPaths = true;
}


UDCCharacterMovementComponent* UDCCharacterMovementComponent::FindDCCharacterMovementComponent(const ADCCharacter* Character)
{
	return (Character ? Character->FindComponentByClass<UDCCharacterMovementComponent>() : nullptr);
}
