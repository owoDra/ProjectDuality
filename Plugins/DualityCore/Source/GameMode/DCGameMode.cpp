// Copyright (C) 2023 owoDra

#include "DCGameMode.h"

#include "DCGameState.h"
#include "Player/DCPlayerController.h"
#include "Player/DCPlayerState.h"
//#include "Character/DCCharacter.h"
#include "UI/DCHUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCGameMode)


ADCGameMode::ADCGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ADCGameState::StaticClass();
	PlayerControllerClass = ADCPlayerController::StaticClass();
	PlayerStateClass = ADCPlayerState::StaticClass();
	//DefaultPawnClass = ADCCharacter::StaticClass();
	HUDClass = ADCHUD::StaticClass();
}
