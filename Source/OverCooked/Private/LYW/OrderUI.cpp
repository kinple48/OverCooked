#include "LYW/OrderUI.h"
#include "LYW/GameDataManager.h"
#include "Kismet/GameplayStatics.h"

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
		if (CurrentTime <=0.0f)
		{
			AGameDataManager* dm = Cast<AGameDataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameDataManager::StaticClass()));
			dm->AddCoin(-1);
			CurrentTime = MissionTime;
			TimePercent = 1.0f;
		}
	}
}