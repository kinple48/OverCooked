// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/EndGameUI.h"
#include "Components/CanvasPanel.h" 
#include "Components/TextBlock.h"      
#include "Components/Image.h"    

void UEndGameUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 별 이미지를 배열로 등록
	StarImages = { StarImage1, StarImage2, StarImage3 };

	// 결과 패널은 처음에 숨김
	if (ResultPanel)
	{
		ResultPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	if (TimeOverPanel)
	{
		TimeOverPanel->SetVisibility(ESlateVisibility::Visible); // 처음에 시간 종료 텍스트 보이기
	}

	// 2초 뒤에 결과 표시
	FTimerHandle TempHandle;
	GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &UEndGameUI::ShowResultPanel, 2.0f, false);
}

void UEndGameUI::SetupResult(int32 FinalScore, int32 StarCount)
{
	CachedScore = FinalScore;
	CachedStarCount = StarCount;
}

void UEndGameUI::ShowResultPanel()
{
	if (TimeOverPanel)
	{
		TimeOverPanel->SetVisibility(ESlateVisibility::Hidden); 
	}

	if (ResultPanel)
	{
		ResultPanel->SetVisibility(ESlateVisibility::Visible); 
	}

	if (ScoreText)
	{
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CachedScore)));
	}

	for (int32 i = 0; i < StarImages.Num(); ++i)
	{
		if (StarImages[i])
		{
			StarImages[i]->SetVisibility(i < CachedStarCount ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
	}
}