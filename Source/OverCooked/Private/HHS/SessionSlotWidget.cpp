// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/SessionSlotWidget.h"

#include "Components/Button.h"
#include "HHS/HSGameInstance.h"
#include "Components/TextBlock.h"

void USessionSlotWidget::Set(const struct FSessionInfo& InSessionInfo)
{
	txt_roomName->SetText(FText::FromString(InSessionInfo.roomName));
	txt_hostName->SetText(FText::FromString(InSessionInfo.hostName));
	txt_playerCount->SetText(FText::FromString(InSessionInfo.playerCount));
	txt_pingSpeed->SetText(FText::FromString( FString::Printf(TEXT("%dms"), InSessionInfo.pingSpeed)));

	SessionNumber = InSessionInfo.index;
}

void USessionSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_join->OnClicked.AddDynamic(this, &USessionSlotWidget::JoinSession);
}

void USessionSlotWidget::JoinSession()
{
	auto gi = Cast<UHSGameInstance>(GetWorld()->GetGameInstance());
	if (gi)
	{
		gi->JoinSelectedSession(SessionNumber);	
	}
}
