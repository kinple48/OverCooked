// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/LobbyGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = false;
}

// ALobbyGameMode.cpp

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true;

		// UI Only로 입력 설정
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);

		PC->SetIgnoreMoveInput(true);
		PC->SetIgnoreLookInput(true);
	}
}


void ALobbyGameMode::StartGame()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());

		if (APawn* Pawn = PC->GetPawn())
		{
			Pawn->EnableInput(PC);
		}

		// 본 게임 레벨 로드 또는 상태 전환
		UGameplayStatics::OpenLevel(this, "MainMap");
	}
}
