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

    UPROPERTY(VisibleAnywhere, Category = Basic) // �÷��̾� �̸�
        FString D_PlayerName;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���̺� ���� ���� �̸�
        FString D_SaveSlotName;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ����
        int32 D_Level;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���� ����ġ
        int32 D_CurrentEXP;
    UPROPERTY(VisibleAnywhere, Category = Basic) // �ִ� ����ġ
        int32 D_MaxEXP;
    UPROPERTY(VisibleAnywhere, Category = Basic) // �ִ� ü��
        int32 D_MaxHealth;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���� ü��
        int32 D_CurrentHealth;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���� ���
        int32 D_Gold;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���� ����
        int32 D_SwordLv;
    UPROPERTY(VisibleAnywhere, Category = Basic) // �� ����
        int32 D_SheildLv;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���� ��
        int32 D_Second;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���� ��
        int32 D_Minute;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���� ��
        int32 D_Hour;
    UPROPERTY(VisibleAnywhere, Category = Basic) // �� ī�� ��
        TArray<int32> D_Deck;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���൵
        TArray<bool> D_Progress;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���̺� �� ��
        int32 D_SaveMap;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���̺� �� ��ġ
        FTransform D_SaveTransform;
    UPROPERTY(VisibleAnywhere, Category = Basic) // 2�� ���̺� �� ��ġ
        FTransform D_SaveTransform2;
    UPROPERTY(VisibleAnywhere, Category = Basic) // �� ����1_1 �ܿ� ����
        TArray<int32> D_MyStore1_1Count;
    UPROPERTY(VisibleAnywhere, Category = Basic) // �� ����1_3 �ܿ� ����
        TArray<bool> D_MyStore1_3Count;
    UPROPERTY(VisibleAnywhere, Category = Basic) // �� ����2_1 �ܿ� ����
        TArray<int32> D_MyStore2_1Count;
    UPROPERTY(VisibleAnywhere, Category = Basic) // �� ����2_3 �ܿ� ����
        TArray<bool> D_MyStore2_3Count;
    UPROPERTY(VisibleAnywhere, Category = Basic) // �� ����4_1 �ܿ� ����
        TArray<int32> D_MyStore4_1Count;
    UPROPERTY(VisibleAnywhere, Category = Basic) // �� ����4_3 �ܿ� ����
        TArray<bool> D_MyStore4_3Count;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ȸ�� �̺�Ʈ ������
        TArray<int32> D_EventHealData;
    UPROPERTY(VisibleAnywhere, Category = Basic) // �м� �̺�Ʈ ������
        TArray<int32> D_EventFountainData;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���� �̺�Ʈ ������
        TArray<int32> D_EventDungeonData;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ī�� ��ȭ/���� �̺�Ʈ ������
        TArray<int32> D_Event3Data;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ��� ��ȭ �̺�Ʈ ������
        TArray<bool> D_Event4Data;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���� �� ī�� ����Ƚ��
        TArray<int32> D_StoreDeleteStack;
    UPROPERTY(VisibleAnywhere, Category = Basic) // ���� �Ǽ�����
        TArray<int32> D_MyAccessoryData;
    UPROPERTY(VisibleAnywhere, Category = Basic) // Ʃ�丮�� ��ŵ ����
        bool D_Tutorial;

    UMySaveGame();
	
};
