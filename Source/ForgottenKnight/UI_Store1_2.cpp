// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Store1_2.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"

void UUI_Store1_2::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_Store1_2::OnBackClicked);
	}
	if (Button_Behind != nullptr)
	{
		Button_Behind->OnClicked.AddDynamic(this, &UUI_Store1_2::OnBehindClicked);
	}
	if (Button_Front != nullptr)
	{
		Button_Front->OnClicked.AddDynamic(this, &UUI_Store1_2::OnFrontClicked);
	}
	if (Button_BuySword != nullptr)
	{
		Button_BuySword->OnClicked.AddDynamic(this, &UUI_Store1_2::OnBuySwordClicked);
	}
	if (Button_BuySheild != nullptr)
	{
		Button_BuySheild->OnClicked.AddDynamic(this, &UUI_Store1_2::OnBuySheildClicked);
	}

	ChangeEquipStat();
	ChangeGold();
}

void UUI_Store1_2::ChangeGold()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	FString NewWString = FString::FromInt(AForgottenKnightCharacter::MyGold);
	FText InWText = FText::FromString(NewWString + " G");
	Text_Gold->SetText(InWText);
}

void UUI_Store1_2::ChangeEquipStat()
{
	FString NewWString;

	NewWString = FString::Printf(TEXT("철검+%d"), AForgottenKnightCharacter::MySwordLv-1);
	if (AForgottenKnightCharacter::MySwordLv == 1)
	{
		NewWString = FString::Printf(TEXT("철검"));
	}
	Text_SwordLvFront->SetText(FText::FromString(NewWString));
	NewWString = FString::Printf(TEXT("철검+%d"), AForgottenKnightCharacter::MySwordLv);
	Text_SwordLvBehind->SetText(FText::FromString(NewWString));
	NewWString = FString::Printf(TEXT("%d G"), AForgottenKnightCharacter::MySwordLv * 20);
	Text_SwordPrice->SetText(FText::FromString(NewWString));
	NewWString = FString::Printf(TEXT("Lv.%d"), AForgottenKnightCharacter::MySwordLv * 3);
	Text_SwordNeedLv->SetText(FText::FromString(NewWString));

	SwordPrice = AForgottenKnightCharacter::MySwordLv * 20;
	SwordLv = AForgottenKnightCharacter::MySwordLv * 3;

	NewWString = FString::Printf(TEXT("철갑옷+%d"), AForgottenKnightCharacter::MySheildLv - 1);
	if (AForgottenKnightCharacter::MySheildLv == 1)
	{
		NewWString = FString::Printf(TEXT("철갑옷"));
	}
	Text_SheildLvFront->SetText(FText::FromString(NewWString));
	NewWString = FString::Printf(TEXT("철갑옷+%d"), AForgottenKnightCharacter::MySheildLv);
	Text_SheildLvBehind->SetText(FText::FromString(NewWString));
	NewWString = FString::Printf(TEXT("%d G"), AForgottenKnightCharacter::MySheildLv * 10);
	Text_SheildPrice->SetText(FText::FromString(NewWString));
	NewWString = FString::Printf(TEXT("Lv.%d"), AForgottenKnightCharacter::MySheildLv * 2);
	Text_SheildNeedLv->SetText(FText::FromString(NewWString));

	SheildPrice = AForgottenKnightCharacter::MySheildLv * 10;
	SheildLv = AForgottenKnightCharacter::MySheildLv * 2;

	NewWString = FString::Printf(TEXT("\n현재 효과\n추가 대미지 + %d\n\n강화 후 효과\n추가 대미지 + %d"), 2*(AForgottenKnightCharacter::MySwordLv-1), 2*AForgottenKnightCharacter::MySwordLv);
	Text_SwordStat->SetText(FText::FromString(NewWString));

	NewWString = FString::Printf(TEXT("\n현재 효과\n받는 대미지 - %d, 숨 고르기 효과 + %d\n\n강화 후 효과\n받는 대미지 - %d, 숨 고르기 효과 + %d"), AForgottenKnightCharacter::MySheildLv - 1, AForgottenKnightCharacter::MySheildLv - 1 / 2, AForgottenKnightCharacter::MySheildLv, AForgottenKnightCharacter::MySheildLv / 2);
	Text_SheildStat->SetText(FText::FromString(NewWString));

}

void UUI_Store1_2::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		if (PlayerController->StoreOrderNum == 1)
		{
			PlayerController->HideStoreWidget(1, 2);
		}
		else if (PlayerController->StoreOrderNum == 2)
		{
			PlayerController->HideStoreWidget(2, 2);
		}
		else if (PlayerController->StoreOrderNum == 4)
		{
			PlayerController->HideStoreWidget(4, 2);
		}
	}
}

void UUI_Store1_2::OnBehindClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		if (PlayerController->StoreOrderNum == 1)
		{
			PlayerController->HideStoreWidget(1, 2);
			PlayerController->ShowStoreWidget(1, 3);
		}
		else if (PlayerController->StoreOrderNum == 2)
		{
			PlayerController->HideStoreWidget(2, 2);
			PlayerController->ShowStoreWidget(2, 3);
		}
		else if (PlayerController->StoreOrderNum == 4)
		{
			PlayerController->HideStoreWidget(4, 2);
			PlayerController->ShowStoreWidget(4, 3);
		}
	}
}

void UUI_Store1_2::OnFrontClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		if (PlayerController->StoreOrderNum == 1)
		{
			PlayerController->HideStoreWidget(1, 2);
			PlayerController->ShowStoreWidget(1, 1);
		}
		else if (PlayerController->StoreOrderNum == 2)
		{
			PlayerController->HideStoreWidget(2, 2);
			PlayerController->ShowStoreWidget(2, 1);
		}
		else if (PlayerController->StoreOrderNum == 4)
		{
			PlayerController->HideStoreWidget(4, 2);
			PlayerController->ShowStoreWidget(4, 1);
		}
	}
}

void UUI_Store1_2::OnBuySwordClicked()
{
	if (SwordPrice <= AForgottenKnightCharacter::MyGold) // 골드 충족
	{
		if (SwordLv <= AForgottenKnightCharacter::MyLevel) // 레벨 충족
		{
			AForgottenKnightCharacter::MyGold -= SwordPrice;
			AForgottenKnightCharacter::MySwordLv += 1;
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

	ChangeEquipStat();
	ChangeGold();
}

void UUI_Store1_2::OnBuySheildClicked()
{
	if (SheildPrice <= AForgottenKnightCharacter::MyGold) // 골드 충족
	{
		if (SheildLv <= AForgottenKnightCharacter::MyLevel) // 레벨 충족
		{
			AForgottenKnightCharacter::MyGold -= SheildPrice;
			AForgottenKnightCharacter::MySheildLv += 1;
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

	ChangeEquipStat();
	ChangeGold();
}
