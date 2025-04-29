// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameUI.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED_API UEndGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetupResult(int32 FinalScore, int32 StarCount);

private:
	void ShowResultPanel();

	int32 CachedScore;
	int32 CachedStarCount;

public:
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* ResultPanel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
	class UImage* StarImage1;

	UPROPERTY(meta = (BindWidget))
	class UImage* StarImage2;

	UPROPERTY(meta = (BindWidget))
	class UImage* StarImage3;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* TimeOverPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_exit;

	UFUNCTION()
	void OnExit();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* StarAppearAnim;

	
	
private:
	TArray<class UImage*> StarImages;
};
