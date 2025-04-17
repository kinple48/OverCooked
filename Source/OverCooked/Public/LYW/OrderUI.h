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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimePercent = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MissionTime = 10.0f;

	float CurrentTime = 0.0f;

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

	FLinearColor StartColor = FLinearColor::Green;
	FLinearColor EndColor = FLinearColor::Red;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};