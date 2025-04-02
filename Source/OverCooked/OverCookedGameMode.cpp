// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverCookedGameMode.h"
#include "OverCookedPlayerController.h"
#include "OverCookedCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOverCookedGameMode::AOverCookedGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AOverCookedPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}