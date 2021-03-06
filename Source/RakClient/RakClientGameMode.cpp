// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "RakClient.h"
#include "RakClientGameMode.h"
#include "RakClientPlayerController.h"
#include "RakClientCharacter.h"

ARakClientGameMode::ARakClientGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARakClientPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}