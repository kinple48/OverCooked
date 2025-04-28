// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/HSGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "OverCooked/OverCooked.h"

void UHSGameInstance::Init()
{
	Super::Init();

	if ( auto subsys = IOnlineSubsystem::Get() )
	{
		sessionInterface = subsys->GetSessionInterface();
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UHSGameInstance::OnCreateSessionComplete);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UHSGameInstance::OnFindSessionsComplete);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UHSGameInstance::OnJoinSessionComplete);
		mySessionName.Append(FString::Printf(TEXT("_%d_%d"), FMath::Rand32(), FDateTime::Now().GetMillisecond()));
	}
}

void UHSGameInstance::CreateMySession(FString roomName, int32 playerCount)
{
	// 세션설정 변수
	FOnlineSessionSettings sessionSettings;

	// 1. Dedicated Server 접속 여부
	sessionSettings.bIsDedicated = false;

	// 2. 랜선(로컬)매칭을 할지 steam 매칭을 할지 여부
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	sessionSettings.bIsLANMatch = (subsysName == "NULL");

	// 3. 매칭이 온라인을 통해 노출될지 여부
	// false 이면 초대를 통해서만 입장이 가능
	// SendSessionInviteToFriend() 함수를 통해 친구를 초대할 수 있다.
	sessionSettings.bShouldAdvertise = true;

	// 4. 온라인 상태(Presence) 정보를 활용할지 여부
	sessionSettings.bUsesPresence = true;
	sessionSettings.bUseLobbiesIfAvailable = true;

	// 5. 게임진행중에 참여 허가할지 여부
	sessionSettings.bAllowJoinViaPresence = true;
	sessionSettings.bAllowJoinInProgress = true;

	// 6. 세션에 참여할 수 있는 공개(public) 연결의 최대 허용 수
	sessionSettings.NumPublicConnections = playerCount;

	// 7. 커스텀 룸네임 설정
	sessionSettings.Set(FName("ROOM_NAME"), StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 8. 호스트 네임 설정
	sessionSettings.Set(FName("HOST_NAME"),StringBase64Encode(mySessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// netID
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	PRINTLOG(TEXT("Create Session Strat : %s"), *mySessionName);
	sessionInterface->CreateSession(*netID, FName(mySessionName), sessionSettings);
}

void UHSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	PRINTLOG(TEXT("SessionName : %s, bWasSuccessful : %d"), *SessionName.ToString(), bWasSuccessful);
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(TEXT("/Game/HHS/Maps/MainMap?listen"));
	}
}

void UHSGameInstance::FindOtherSession()
{
	onSearchState.Broadcast(true);
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	// 1. 세션 검색 조건 설정
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// 2. Lan 여부
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");

	// 3. 최대 검색 세션 수
	SessionSearch->MaxSearchResults = 10;

	// 4. 세션검색
	sessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UHSGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	// 찾기 실패시
	if (!bWasSuccessful)
	{
		onSearchState.Broadcast(false);
		PRINTLOG(TEXT("Session search failed..."));
		return;
	}
	onSearchState.Broadcast(false);

	// 세션검색결과 배열
	auto results = SessionSearch->SearchResults;
	PRINTLOG(TEXT("Search Result Count : %d"), results.Num());
	for ( int i=0; i<results.Num(); i++ )
	{
		auto sr = results[i];
		if (sr.IsValid() == false)
		{
			continue;
		}

		FString roomName;
		FString hostName;
		
		// 세션 정보 구조체 선언
		FSessionInfo SessionInfo;
		SessionInfo.index = i;
		
		sr.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
		sr.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);
		SessionInfo.roomName = StringBase64Decode(roomName);
		SessionInfo.hostName = StringBase64Decode(hostName);

		// 입장가능한 플레이어 수
		int32 maxPlayerCount = sr.Session.SessionSettings.NumPublicConnections;
		// 현재 입장한 플레이어 수 ( 최대 - 현재 입장 가능한 수 )
		// NumOpenPublicConnections 스팀에서만 정상적으로 값이 들어온다.
		int32 currentPlayerCount = maxPlayerCount - sr.Session.NumOpenPublicConnections;
		SessionInfo.playerCount = FString::Printf(TEXT("(%d/%d)"),currentPlayerCount, maxPlayerCount);
		// 핑정보 (스팀에서는 9999로 나온다)
		int32 pingSpeed = sr.PingInMs;
		PRINTLOG(TEXT("%s"), *SessionInfo.ToString());
		
		onSearchCompleted.Broadcast(SessionInfo);
	}

}

void UHSGameInstance::JoinSelectedSession(int32 index)
{
	auto sr = SessionSearch->SearchResults;

	sr[index].Session.SessionSettings.bUseLobbiesIfAvailable = true;
	sr[index].Session.SessionSettings.bUsesPresence = true;
	
	sessionInterface->JoinSession(0, FName(mySessionName), sr[index]);
}

void UHSGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		auto pc = GetWorld()->GetFirstPlayerController();

		FString url;
		sessionInterface->GetResolvedConnectString(sessionName, url);

		PRINTLOG(TEXT("Join URL : %s"), *url);

		if (!url.IsEmpty())
		{
			pc->ClientTravel(url, TRAVEL_Absolute);
		}
	}
	else
	{
		PRINTLOG(TEXT("Join Session failed : %d"), result);
	}
}

FString UHSGameInstance::StringBase64Encode(const FString& str)
{
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UHSGameInstance::StringBase64Decode(const FString& str)
{
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string utf8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(utf8String.c_str());
}
