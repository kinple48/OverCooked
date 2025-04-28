// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/LoginWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "HHS/HSGameInstance.h"
#include "Components/ScrollBox.h"
#include "HHS/SessionSlotWidget.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	gi = Cast<UHSGameInstance>(GetWorld()->GetGameInstance());
	gi->onSearchCompleted.AddDynamic(this, &ULoginWidget::AddSlotWidget);
	gi->onSearchState.AddDynamic(this, &ULoginWidget::OnChangeButtonEnable);
	
	btn_createRoom->OnClicked.AddDynamic(this, &ULoginWidget::CreateRoom);
	slider_playerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnValueChanged);

	btn_CreateSession->OnClicked.AddDynamic(this, &ULoginWidget::SwitchCreatePanel);
	btn_FindSession->OnClicked.AddDynamic(this, &ULoginWidget::SwitchFindPanel);
	btn_back->OnClicked.AddDynamic(this, &ULoginWidget::BackToMain);
	btn_back_1->OnClicked.AddDynamic(this, &ULoginWidget::BackToMain);
	
	btn_find->OnClicked.AddDynamic(this, &ULoginWidget::OnClickedFindSession);
}

void ULoginWidget::CreateRoom()
{
	if (!gi || edit_roomName->GetText().IsEmpty()) return;

	gi->CreateMySession(edit_roomName->GetText().ToString(), slider_playerCount->GetValue()); 
}

void ULoginWidget::OnValueChanged(float Value)
{
	txt_playerCount->SetText(FText::AsNumber(Value));
}

void ULoginWidget::SwitchCreatePanel()
{
	if (edit_userName->GetText().IsEmpty() == false)
	{
		gi->mySessionName = edit_userName->GetText().ToString();
	}
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void ULoginWidget::SwitchFindPanel()
{
	if (edit_userName->GetText().IsEmpty() == false)
	{
		gi->mySessionName = edit_userName->GetText().ToString();
	}
	WidgetSwitcher->SetActiveWidgetIndex(2);
	OnClickedFindSession();
}

void ULoginWidget::BackToMain()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULoginWidget::OnClickedFindSession()
{
	scroll_roomList->ClearChildren();
	if ( gi != nullptr )
	{
		gi->FindOtherSession();
	}
}

void ULoginWidget::OnChangeButtonEnable(bool bIsSearching)
{
	btn_find->SetIsEnabled(!bIsSearching);

	if ( bIsSearching == true )
	{
		// 검색 중 보이도록 처리
		txt_findMsg->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// 검색 중 사라지도록 처리
		txt_findMsg->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULoginWidget::AddSlotWidget(const struct FSessionInfo& sessionInfo)
{
	auto slot = CreateWidget<USessionSlotWidget>(this, sessionInfoWidget);
	slot->Set(sessionInfo);

	scroll_roomList->AddChild(slot);
}

