#include "LYW/OrderUI.h"

void UOrderUI::NativeConstruct()
{
	Super::NativeConstruct();

	//SetCanTick(true); // 위젯 틱 활성화
	CurrentTime = MissionTime;
	PlayAnimation(IngredientDown);
}

void UOrderUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentTime -= InDeltaTime;

	TimePercent = FMath::Clamp(CurrentTime / MissionTime, 0.0f, 1.0f);
	ProgressColor = FMath::Lerp(StartColor, EndColor, 1.0f - TimePercent);

	if (ProgressBar)
	{
		ProgressBar->SetPercent(TimePercent);
		ProgressBar->SetFillColorAndOpacity(ProgressColor); // 바로 색 적용
	}
}