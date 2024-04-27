// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_SaveSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "MySaveGame.h"
#include "Controller_MainCharacter.h"

void UUI_SaveSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_SaveSlot::OnBackClicked);
	}
	if (Button_Slot1 != nullptr)
	{
		Button_Slot1->OnClicked.AddDynamic(this, &UUI_SaveSlot::OnSlot1Clicked);
	}
	if (Button_Slot2 != nullptr)
	{
		Button_Slot2->OnClicked.AddDynamic(this, &UUI_SaveSlot::OnSlot2Clicked);
	}
	if (Button_Slot3 != nullptr)
	{
		Button_Slot3->OnClicked.AddDynamic(this, &UUI_SaveSlot::OnSlot3Clicked);
	}

	CalcSlotData(1);
	CalcSlotData(2);
	CalcSlotData(3);
}

void UUI_SaveSlot::CalcSlotData(int SlotNum)
{
	FString sStr = {""};
	UMySaveGame* NewPlayerData = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	if (SlotNum == 1)
	{
		NewPlayerData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("1"), 0));
		if (NewPlayerData == nullptr)
		{
			sStr = FString::Printf(TEXT("세이브 데이터 1\n\n현재 슬롯에 데이터가 없습니다"));
			Text_Save1->SetText(FText::FromString(sStr));
			return;
		}
	}
	else if (SlotNum == 2)
	{
		NewPlayerData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("2"), 0));
		if (NewPlayerData == nullptr)
		{
			sStr = FString::Printf(TEXT("세이브 데이터 2\n\n현재 슬롯에 데이터가 없습니다"));
			Text_Save2->SetText(FText::FromString(sStr));
			return;
		}
	}
	else if (SlotNum == 3)
	{
		NewPlayerData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("3"), 0));
		if (NewPlayerData == nullptr)
		{
			sStr = FString::Printf(TEXT("세이브 데이터 3\n\n현재 슬롯에 데이터가 없습니다"));
			Text_Save3->SetText(FText::FromString(sStr));
			return;
		}
	}

	int ClearStageNum = 0; // 클리어 스테이지
	for (int i = 1; i < NewPlayerData->D_Progress.Num(); i++) // 1부터 6
	{
		if (NewPlayerData->D_Progress[i])
		{
			ClearStageNum += 1;
		}
	}

	FString ThisHour = {""};
	if (NewPlayerData->D_Hour >= 10)
	{
		ThisHour = FString::FromInt(NewPlayerData->D_Hour);
	}
	else
	{
		ThisHour = "0" + FString::FromInt(NewPlayerData->D_Hour);
	}
	FString ThisMinute = { "" };
	if (NewPlayerData->D_Minute >= 10)
	{
		ThisMinute = FString::FromInt(NewPlayerData->D_Minute);
	}
	else
	{
		ThisMinute = "0" + FString::FromInt(NewPlayerData->D_Minute);
	}
	FString ThisSecond = { "" };
	if (NewPlayerData->D_Second >= 10)
	{
		ThisSecond = FString::FromInt(NewPlayerData->D_Second);
	}
	else
	{
		ThisSecond = "0" + FString::FromInt(NewPlayerData->D_Second);
	}

	sStr = FString::Printf(TEXT("세이브 데이터 %d\n\n클리어한 스테이지 -> %d\n\n플레이 시간 -> %s : %s : %s"), SlotNum, ClearStageNum, *ThisHour, *ThisMinute, *ThisSecond);

	if (SlotNum == 1 && Text_Save1 != nullptr) // 세이브 슬롯에 담길 요소 1. 세이브 슬롯 인덱스 2. 클리어 스테이지(진행도) 3. 플레이 시간
	{
		Text_Save1->SetText(FText::FromString(sStr));
	}
	else if (SlotNum == 2 && Text_Save2 != nullptr) // 세이브 슬롯에 담길 요소 1. 세이브 슬롯 인덱스 2. 클리어 스테이지(진행도) 3. 플레이 시간
	{
		Text_Save2->SetText(FText::FromString(sStr));
	}
	else if (SlotNum == 3 && Text_Save3 != nullptr) // 세이브 슬롯에 담길 요소 1. 세이브 슬롯 인덱스 2. 클리어 스테이지(진행도) 3. 플레이 시간
	{
		Text_Save3->SetText(FText::FromString(sStr));
	}
}

void UUI_SaveSlot::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideSaveSlotWidget();
		//PlayerController->ShowPauseWidget();
	}
}

void UUI_SaveSlot::OnSlot1Clicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	MyPlayer->SaveData(1);
	CalcSlotData(1);
}

void UUI_SaveSlot::OnSlot2Clicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	MyPlayer->SaveData(2);
	CalcSlotData(2);
}

void UUI_SaveSlot::OnSlot3Clicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	MyPlayer->SaveData(3);
	CalcSlotData(3);
}
