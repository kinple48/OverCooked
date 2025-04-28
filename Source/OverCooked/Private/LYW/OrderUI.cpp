// OrderUI.cpp
#include "LYW/OrderUI.h"
#include "LYW/GameDataManager.h"
#include "Kismet/GameplayStatics.h"

void UOrderUI::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(IngredientDown);
	TimePercent = 1.0f;
	ProgressBar->SetPercent(TimePercent);
	ProgressBar->SetFillColorAndOpacity(StartColor);
}

void UOrderUI::SetPercent(float percent)
{
	TimePercent = percent;
	SetProgress();
}

void UOrderUI::SetProgress()
{

	if (ProgressBar)
	{
		ProgressColor = FMath::Lerp(StartColor, EndColor, 1.0f - TimePercent);
		ProgressBar->SetPercent(TimePercent);
		ProgressBar->SetFillColorAndOpacity(ProgressColor);

		if (TimePercent * 100.0f < 20.0f && !bIsTimeLimitPlaying)
		{
			if (TimeLimit)
			{
				PlayAnimation(TimeLimit, 0.0f, 0);
				bIsTimeLimitPlaying = true;
			}
		}
		else if (TimePercent > 0.2f && bIsTimeLimitPlaying)
		{
			if (TimeLimit)
			{
				StopAnimation(TimeLimit);
				bIsTimeLimitPlaying = false;
			}
		}
	}
}
