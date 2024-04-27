// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:

    UPROPERTY(VisibleAnywhere, Category = Basic) // 플레이어 이름
        FString D_PlayerName;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 세이브 파일 슬롯 이름
        FString D_SaveSlotName;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 레벨
        int32 D_Level;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 현재 경험치
        int32 D_CurrentEXP;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 최대 경험치
        int32 D_MaxEXP;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 최대 체력
        int32 D_MaxHealth;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 현재 체력
        int32 D_CurrentHealth;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 보유 골드
        int32 D_Gold;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 무기 레벨
        int32 D_SwordLv;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 방어구 레벨
        int32 D_SheildLv;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 게임 초
        int32 D_Second;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 게임 분
        int32 D_Minute;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 게임 시
        int32 D_Hour;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 내 카드 덱
        TArray<int32> D_Deck;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 진행도
        TArray<bool> D_Progress;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 세이브 한 맵
        int32 D_SaveMap;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 세이브 한 위치
        FTransform D_SaveTransform;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 2맵 세이브 한 위치
        FTransform D_SaveTransform2;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 내 상점1_1 잔여 수량
        TArray<int32> D_MyStore1_1Count;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 내 상점1_3 잔여 수량
        TArray<bool> D_MyStore1_3Count;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 내 상점2_1 잔여 수량
        TArray<int32> D_MyStore2_1Count;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 내 상점2_3 잔여 수량
        TArray<bool> D_MyStore2_3Count;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 내 상점4_1 잔여 수량
        TArray<int32> D_MyStore4_1Count;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 내 상점4_3 잔여 수량
        TArray<bool> D_MyStore4_3Count;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 회복 이벤트 데이터
        TArray<int32> D_EventHealData;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 분수 이벤트 데이터
        TArray<int32> D_EventFountainData;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 던전 이벤트 데이터
        TArray<int32> D_EventDungeonData;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 카드 강화/삭제 이벤트 데이터
        TArray<int32> D_Event3Data;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 장비 강화 이벤트 데이터
        TArray<bool> D_Event4Data;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 상점 별 카드 제거횟수
        TArray<int32> D_StoreDeleteStack;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 보유 악세서리
        TArray<int32> D_MyAccessoryData;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 튜토리얼 스킵 여부
        bool D_Tutorial;

    UMySaveGame();
	
};
