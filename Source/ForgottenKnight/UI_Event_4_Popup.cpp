// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Event_4_Popup.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"

void UUI_Event_4_Popup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_Event_4_Popup::OnBackClicked);
	}
	if (Button_BuySword != nullptr)
	{
		Button_BuySword->OnClicked.AddDynamic(this, &UUI_Event_4_Popup::OnBuySwordClicked);
	}
	if (Button_BuySheild != nullptr)
	{
		Button_BuySheild->OnClicked.AddDynamic(this, &UUI_Event_4_Popup::OnBuySheildClicked);
	}

	ChangeEquipStat();
	ChangeGold();
}

void UUI_Event_4_Popup::ChangeGold()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	FString NewWString = FString::FromInt(AForgottenKnightCharacter::MyGold);
	FText InWText = FText::FromString(NewWString + " G");
	Text_Gold->SetText(InWText);
}

void UUI_Event_4_Popup::ChangeEquipStat()
{
	FString NewWString;

	NewWString = FString::Printf(TEXT("철검+%d"), AForgottenKnightCharacter::MySwordLv - 1);
	if (AForgottenKnightCharacter::MySwordLv == 1)
	{
		NewWString = FString::Printf(TEXT("철검"));
	}
	Text_SwordLvFront->SetText(FText::FromString(NewWString));
	NewWString = FString::Printf(TEXT("철검+%d"), AForgottenKnightCharacter::MySwordLv);
	Text_SwordLvBehind->SetText(FText::FromString(NewWString));
	NewWString = FString::Printf(TEXT("%d G"), AForgottenKnightCharacter::MySwordLv * 10);
	Text_SwordPrice->SetText(FText::FromString(NewWString));
	NewWString = FString::Printf(TEXT("Lv.%d"), AForgottenKnightCharacter::MySwordLv * 3);
	Text_SwordNeedLv->SetText(FText::FromString(NewWString));

	SwordPrice = AForgottenKnightCharacter::MySwordLv * 10;
	SwordLv = AForgottenKnightCharacter::MySwordLv * 3;

	NewWString = FString::Printf(TEXT("철갑옷+%d"), AForgottenKnightCharacter::MySheildLv - 1);
	if (AForgottenKnightCharacter::MySheildLv == 1)
	{
		NewWString = FString::Printf(TEXT("철갑옷"));
	}
	Text_SheildLvFront->SetText(FText::FromString(NewWString));
	NewWString = FString::Printf(TEXT("철갑옷+%d"), AForgottenKnightCharacter::MySheildLv);
	Text_SheildLvBehind->SetText(FText::FromString(NewWString));
	NewWString = FString::Printf(TEXT("%d G"), AForgottenKnightCharacter::MySheildLv * 5);
	Text_SheildPrice->SetText(FText::FromString(NewWString));
	NewWString = FString::Printf(TEXT("Lv.%d"), AForgottenKnightCharacter::MySheildLv * 2);
	Text_SheildNeedLv->SetText(FText::FromString(NewWString));

	SheildPrice = AForgottenKnightCharacter::MySheildLv * 5;
	SheildLv = AForgottenKnightCharacter::MySheildLv * 2;

}

void UUI_Event_4_Popup::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowEvent4Widget(EventOrder);
		PlayerController->HideEvent4PopupWidget();
	}
}

void UUI_Event_4_Popup::OnBuySwordClicked()
{
	if (AController_MainCharacter::Event4Data[EventOrder])
	{
		if (SwordPrice <= AForgottenKnightCharacter::MyGold) // 골드 충족
		{
			if (SwordLv <= AForgottenKnightCharacter::MyLevel) // 레벨 충족
			{
				if (AForgottenKnightCharacter::MySwordLv <= 4 || EventOrder == 2) // 1맵 이벤트는 강화 수치 제한
				{
					AForgottenKnightCharacter::MyGold -= SwordPrice;
					AForgottenKnightCharacter::MySwordLv += 1;
					AController_MainCharacter::Event4Data[EventOrder] = false;
				}
				else
				{
					FString sStr = FString::Printf(TEXT("강화 수치가 4 이상입니다"));
					Text_Notify->SetText(FText::FromString(sStr));
					PlayAnimation(Anim_Message);
				}
			}
			else
			{
				FString sStr = FString::Printf(TEXT("레벨이 부족합니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
			}
		}
		else
		{
			FString sStr = FString::Printf(TEXT("골드가 부족합니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
		}
	}
	else
	{
		FString sStr = FString::Printf(TEXT("이미 기회를 모두 사용했습니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}

	ChangeEquipStat();
	ChangeGold();
}

void UUI_Event_4_Popup::OnBuySheildClicked()
{
	if (AController_MainCharacter::Event4Data[EventOrder])
	{
		if (SheildPrice <= AForgottenKnightCharacter::MyGold) // 골드 충족
		{
			if (SheildLv <= AForgottenKnightCharacter::MyLevel) // 레벨 충족
			{
				if (AForgottenKnightCharacter::MySheildLv <= 4 || EventOrder == 2) // 1맵 이벤트는 강화 수치 제한
				{
					AForgottenKnightCharacter::MyGold -= SheildPrice;
					AForgottenKnightCharacter::MySheildLv += 1;
					AController_MainCharacter::Event4Data[EventOrder] = false;
				}
				else
				{
					FString sStr = FString::Printf(TEXT("강화 수치가 4 이상입니다"));
					Text_Notify->SetText(FText::FromString(sStr));
					PlayAnimation(Anim_Message);
				}
			}
			else
			{
				FString sStr = FString::Printf(TEXT("레벨이 부족합니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
			}
		}
		else
		{
			FString sStr = FString::Printf(TEXT("골드가 부족합니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
		}
	}
	else
	{
		FString sStr = FString::Printf(TEXT("이미 기회를 모두 사용했습니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}

	ChangeEquipStat();
	ChangeGold();
}
