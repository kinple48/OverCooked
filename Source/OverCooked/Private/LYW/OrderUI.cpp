// OrderUI.cpp
#include "LYW/OrderUI.h"
#include "LYW/GameDataManager.h"
#include "Kismet/GameplayStatics.h"
#include "LYW/OC_GameState.h"


void UOrderUI::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(IngredientDown);
	TimePercent = 1.0f;
	ProgressBar->SetPercent(TimePercent);
	ProgressBar->SetFillColorAndOpacity(StartColor);
}

void UOrderUI::InitUI(float startTime, float duration)
{
	StartTime = startTime; Duration  = duration; 
}

void UOrderUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SetPercent();
}


void UOrderUI::SetPercent()
{
	//TimePercent = percent;
	float Now = GetWorld()->GetTimeSeconds();
	TimePercent = 1.0f - (Now - StartTime) / Duration;

	SetProgress();
}

void UOrderUI::SetProgress()
{

	if (ProgressBar)
	{
		ProgressColor = FMath::Lerp(StartColor, EndColor, 1.0f - TimePercent);
		ProgressBar->SetPercent(TimePercent);
		ProgressBar->SetFillColorAndOpacity(ProgressColor);

		if (TimePercent <= 0.0f)
		{
			TimePercent = 1.0f;
			StartTime = GetWorld()->GetTimeSeconds();
			AGameDataManager* man = Cast<AGameDataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameDataManager::StaticClass()));
			if (man)
			{
				man->FinishOrderTime(myIndex);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("OrderUI: I can't Find GameDataManager"));
			}
		}
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
