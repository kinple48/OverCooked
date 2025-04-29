#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "OrderUI.generated.h"

UCLASS()
class OVERCOOKED_API UOrderUI : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimePercent = 1.0f;

	float MissionEndTime;

	float StartTime;

	float Duration;

	int32 Price;

	int32 myIndex;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ProgressColor;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetAnim), Transient, Category = "UIAnim")
	class UWidgetAnimation* IngredientDown;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetAnim), Transient, Category = "UIAnim")
	class UWidgetAnimation* TimeLimit;

	UPROPERTY()
	bool bIsTimeLimitPlaying = false;

	float TimeAccumulator = 0.0f;
	const float UpdateInterval = 0.4f;

	//UPROPERTY()
	//class AOC_GameState* GameState;

	FLinearColor StartColor = FLinearColor::Green;
	FLinearColor EndColor = FLinearColor::Red;

	virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void InitUI(float startTime, float duration);
	void SetPercent();
	void SetProgress();
};