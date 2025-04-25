// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "HSGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString roomName;
	UPROPERTY(BlueprintReadOnly)
	FString hostName;
	UPROPERTY(BlueprintReadOnly)
	FString playerCount;
	UPROPERTY(BlueprintReadOnly)
	int32 pingSpeed;
	UPROPERTY(BlueprintReadOnly)
	int32 index;

	inline FString ToString()
	{
		return FString::Printf(TEXT("[%d] %s : %s -%s, %dms"), index, *roomName, *hostName, *playerCount, pingSpeed);
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, const FSessionInfo&, sessionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchStateSignature, bool, bIsSearching);

/**
 * 
 */
UCLASS()
class OVERCOOKED_API UHSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
public:
	virtual void Init() override;

public:
	IOnlineSessionPtr sessionInterface;

	void CreateMySession(FString roomName, int32 playerCount);

	// 세션 호스트 이름
	FString mySessionName = "OverCooked";

	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	
	FSearchSignature onSearchCompleted;
	FSearchStateSignature onSearchState;	

	// 방검색
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	void FindOtherSession();
	void OnFindSessionsComplete(bool bWasSuccessful);

	// 세션 (방) 입장
	void JoinSelectedSession(int32 index);

	// 세션 입장 콜백
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);

	// 다국어 인코딩
	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);
};
