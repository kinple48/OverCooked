// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_createRoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* edit_roomName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class USlider* slider_playerCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_playerCount;

	UPROPERTY()
	class UHSGameInstance* gi;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CreateRoom();

	UFUNCTION()
	void OnValueChanged(float Value);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* btn_CreateSession;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* btn_FindSession;

	UFUNCTION()
	void SwitchCreatePanel();

	UFUNCTION()
	void SwitchFindPanel();
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* btn_back;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* btn_back_1;

	UFUNCTION()
	void BackToMain();

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UScrollBox* scroll_roomList;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USessionSlotWidget> sessionInfoWidget;

	UFUNCTION()
	void AddSlotWidget(const struct FSessionInfo& sessioninfo);

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* btn_find;

	// 방찾기 버튼 클릭시 호출될 콜백
	UFUNCTION()
	void OnClickedFindSession();

	// 방 찾기 상태 이벤트 콜백
	UFUNCTION()
	void OnChangeButtonEnable(bool bIsSearching);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* txt_findMsg;

	// 사용자 이름
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UEditableText* edit_userName;
};
