// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Store3.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"

void UUI_Store3::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_Store3::OnBackClicked);
	}
	if (Button_Buy1 != nullptr)
	{
		Button_Buy1->OnClicked.AddDynamic(this, &UUI_Store3::OnBuy1Clicked);
	}
	if (Button_Buy2 != nullptr)
	{
		Button_Buy2->OnClicked.AddDynamic(this, &UUI_Store3::OnBuy2Clicked);
	}
	if (Button_Buy3 != nullptr)
	{
		Button_Buy3->OnClicked.AddDynamic(this, &UUI_Store3::OnBuy3Clicked);
	}
	if (Button_Buy4 != nullptr)
	{
		Button_Buy4->OnClicked.AddDynamic(this, &UUI_Store3::OnBuy4Clicked);
	}
	if (Button_Buy5 != nullptr)
	{
		Button_Buy5->OnClicked.AddDynamic(this, &UUI_Store3::OnBuy5Clicked);
	}
	if (Button_Front != nullptr)
	{
		Button_Front->OnClicked.AddDynamic(this, &UUI_Store3::OnFrontClicked);
	}
	if (Button_Behind != nullptr)
	{
		Button_Behind->OnClicked.AddDynamic(this, &UUI_Store3::OnBehindClicked);
	}
	if (Button_PopupYes != nullptr)
	{
		Button_PopupYes->OnClicked.AddDynamic(this, &UUI_Store3::OnYesClicked);
	}
	if (Button_PopupNo != nullptr)
	{
		Button_PopupNo->OnClicked.AddDynamic(this, &UUI_Store3::OnNoClicked);
	}

	ChangeGold();
	ChangePage();
}

void UUI_Store3::ChangeGold()
{
	FString NewWString = FString::FromInt(AForgottenKnightCharacter::MyGold);
	FText InWText = FText::FromString(NewWString + " G");
	Text_Gold->SetText(InWText);

	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->UpdateOutsideStat();
	}
}

void UUI_Store3::BuyItem(int itemtype, int itemindex, int price) // 목걸이0, 팔찌1, 반지2
{
	if (AForgottenKnightCharacter::MyGold >= price)
	{
		if (AForgottenKnightCharacter::MyAccessoryData[itemtype] == itemindex)
		{
			FString sStr = FString::Printf(TEXT("이미 보유한 장비입니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);

			return;
		}

		int ChangeMaxHP = 0;
		if (itemtype == 0) // 목걸이
		{
			if (itemindex == 1)
			{
				if (AForgottenKnightCharacter::MyAccessoryData[itemtype] == 3)
				{
					ChangeMaxHP = -20;
				}
				else
				{
					ChangeMaxHP = 30;
				}
			}
			else if (itemindex == 3)
			{
				if (AForgottenKnightCharacter::MyAccessoryData[itemtype] == 1)
				{
					ChangeMaxHP = 20;
				}
				else
				{
					ChangeMaxHP = 50;
				}
			}
			else
			{
				if (AForgottenKnightCharacter::MyAccessoryData[itemtype] == 1)
				{
					ChangeMaxHP = -30;
				}
				else if (AForgottenKnightCharacter::MyAccessoryData[itemtype] == 3)
				{
					ChangeMaxHP = -50;
				}
			}
		}
		if (itemtype == 1) // 팔찌
		{
			if (itemindex == 1)
			{
				if (AForgottenKnightCharacter::MyAccessoryData[itemtype] == 2)
				{
					ChangeMaxHP = -80;
				}
				else
				{
					ChangeMaxHP = -30;
				}
			}
			else if (itemindex == 2)
			{
				if (AForgottenKnightCharacter::MyAccessoryData[itemtype] == 1)
				{
					ChangeMaxHP = 80;
				}
				else
				{
					ChangeMaxHP = 50;
				}
			}
			else
			{
				if (AForgottenKnightCharacter::MyAccessoryData[itemtype] == 1)
				{
					ChangeMaxHP = 30;
				}
				else if (AForgottenKnightCharacter::MyAccessoryData[itemtype] == 2)
				{
					ChangeMaxHP = -50;
				}
			}
		}

		AForgottenKnightCharacter::MyMaxHealth += ChangeMaxHP;
		if (AForgottenKnightCharacter::MyMaxHealth <= 0)
		{
			AForgottenKnightCharacter::MyMaxHealth = 1;
			AForgottenKnightCharacter::MyCurrentHealth = 1;
		}
		else if (AForgottenKnightCharacter::MyMaxHealth < AForgottenKnightCharacter::MyCurrentHealth)
		{
			AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
		}

		AForgottenKnightCharacter::MyGold -= price;
		AForgottenKnightCharacter::MyAccessoryData[itemtype] = itemindex;
		ChangeGold();
		ChangePage();
	}
	else
	{
		FString sStr = FString::Printf(TEXT("골드가 부족합니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}
}

void UUI_Store3::ShowPopup(bool Swit)
{
	if (Swit)
	{
		Image_Popup->SetVisibility(ESlateVisibility::Visible);
		TextBlock_Popup->SetVisibility(ESlateVisibility::Visible);
		Button_PopupYes->SetVisibility(ESlateVisibility::Visible);
		Button_PopupNo->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_Popup->SetVisibility(ESlateVisibility::Collapsed);
		TextBlock_Popup->SetVisibility(ESlateVisibility::Collapsed);
		Button_PopupYes->SetVisibility(ESlateVisibility::Collapsed);
		Button_PopupNo->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUI_Store3::ChangePage()
{ // 목걸이 반지 팔찌 순
	FString NewWString;

	if (page == 1) // 1페이지 목걸이
	{
		myitemtype = 1;

		if (AForgottenKnightCharacter::MyAccessoryData[0] == 0)
		{
			Image_0->SetVisibility(ESlateVisibility::Collapsed);
			NewWString = FString::Printf(TEXT(" "));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("보유한 악세서리가 없습니다"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[0] == 1)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Necklace1);
			NewWString = FString::Printf(TEXT("루비 목걸이"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("최대체력 + 30"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[0] == 2)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Necklace2);
			NewWString = FString::Printf(TEXT("철 목걸이"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("받는 피해 1 감소"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[0] == 3)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Necklace3);
			NewWString = FString::Printf(TEXT("루비 목걸이+"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("최대체력 + 50"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[0] == 4)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Necklace4);
			NewWString = FString::Printf(TEXT("철 목걸이+"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("받는 피해 2 감소"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[0] == 5)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Necklace5);
			NewWString = FString::Printf(TEXT("진주 목걸이"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("전투 중 첫 받는 피해 무효화"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}

		Image_1->SetBrush(Necklace1);
		NewWString = FString::Printf(TEXT("루비 목걸이"));
		ItemName_1->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("최대체력 + 30"));
		TextMain_1->SetText(FText::FromString(NewWString));

		Image_2->SetBrush(Necklace2);
		NewWString = FString::Printf(TEXT("철 목걸이"));
		ItemName_2->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("받는 피해 1 감소"));
		TextMain_2->SetText(FText::FromString(NewWString));

		Image_3->SetBrush(Necklace3);
		NewWString = FString::Printf(TEXT("루비 목걸이+"));
		ItemName_3->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("최대체력 + 50"));
		TextMain_3->SetText(FText::FromString(NewWString));

		Image_4->SetBrush(Necklace4);
		NewWString = FString::Printf(TEXT("철 목걸이+"));
		ItemName_4->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("받는 피해 2 감소"));
		TextMain_4->SetText(FText::FromString(NewWString));

		Image_5->SetBrush(Necklace5);
		NewWString = FString::Printf(TEXT("진주 목걸이"));
		ItemName_5->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("전투 중 첫 받는 피해 무효화"));
		TextMain_5->SetText(FText::FromString(NewWString));
	}
	else if (page == 2) // 2페이지 팔찌
	{
		myitemtype = 2;

		if (AForgottenKnightCharacter::MyAccessoryData[1] == 0)
		{
			Image_0->SetVisibility(ESlateVisibility::Collapsed);
			NewWString = FString::Printf(TEXT(" "));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("보유한 악세서리가 없습니다"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[1] == 1)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Bracelet1);
			NewWString = FString::Printf(TEXT("철 팔찌"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("최대체력 - 30, 추가 대미지 + 2"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[1] == 2)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Bracelet2);
			NewWString = FString::Printf(TEXT("루비 팔찌"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("최대체력 + 50, 추가 대미지 - 1"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[1] == 3)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Bracelet3);
			NewWString = FString::Printf(TEXT("다이아 팔찌"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("전투 중 추가 회복량 + 5"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[1] == 4)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Bracelet4);
			NewWString = FString::Printf(TEXT("에메랄드 팔찌"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("적이 받는 부식 효과 2배"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[1] == 5)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Bracelet5);
			NewWString = FString::Printf(TEXT("토파즈 팔찌"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("적이 받는 약화 효과 2배"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}

		Image_1->SetBrush(Bracelet1);
		NewWString = FString::Printf(TEXT("철 팔찌"));
		ItemName_1->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("최대체력 - 30, 추가 대미지 + 2"));
		TextMain_1->SetText(FText::FromString(NewWString));

		Image_2->SetBrush(Bracelet2);
		NewWString = FString::Printf(TEXT("루비 팔찌"));
		ItemName_2->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("최대체력 + 50, 추가 대미지 - 1"));
		TextMain_2->SetText(FText::FromString(NewWString));

		Image_3->SetBrush(Bracelet3);
		NewWString = FString::Printf(TEXT("다이아 팔찌"));
		ItemName_3->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("전투 중 추가 회복량 + 5"));
		TextMain_3->SetText(FText::FromString(NewWString));

		Image_4->SetBrush(Bracelet4);
		NewWString = FString::Printf(TEXT("에메랄드 팔찌"));
		ItemName_4->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("적이 받는 부식 효과 2배"));
		TextMain_4->SetText(FText::FromString(NewWString));

		Image_5->SetBrush(Bracelet5);
		NewWString = FString::Printf(TEXT("토파즈 팔찌"));
		ItemName_5->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("적이 받는 약화 효과 2배"));
		TextMain_5->SetText(FText::FromString(NewWString));
	}
	else if (page == 3) // 3페이지 반지
	{
		myitemtype = 3;

		if (AForgottenKnightCharacter::MyAccessoryData[2] == 0)
		{
			Image_0->SetVisibility(ESlateVisibility::Collapsed);
			NewWString = FString::Printf(TEXT(" "));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("보유한 악세서리가 없습니다"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[2] == 1)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Ring1);
			NewWString = FString::Printf(TEXT("철 반지"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("추가 대미지 + 1"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[2] == 2)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Ring2);
			NewWString = FString::Printf(TEXT("루비 반지"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("공격 시 25%%확률로 출혈 1 부여"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[2] == 3)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Ring3);
			NewWString = FString::Printf(TEXT("에메랄드 반지"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("공격 시 25%%확률로 부식 1 부여"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[2] == 4)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Ring4);
			NewWString = FString::Printf(TEXT("토파즈 반지"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("공격 시 25%%확률로 약화 1 부여"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[2] == 5)
		{
			Image_0->SetVisibility(ESlateVisibility::Visible);
			Image_0->SetBrush(Ring5);
			NewWString = FString::Printf(TEXT("철 반지+"));
			ItemName_0->SetText(FText::FromString(NewWString));
			NewWString = FString::Printf(TEXT("추가 대미지 + 2"));
			TextMain_0->SetText(FText::FromString(NewWString));
		}

		Image_1->SetBrush(Ring1);
		NewWString = FString::Printf(TEXT("철 반지"));
		ItemName_1->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("추가 대미지 + 1"));
		TextMain_1->SetText(FText::FromString(NewWString));

		Image_2->SetBrush(Ring2);
		NewWString = FString::Printf(TEXT("루비 반지"));
		ItemName_2->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("공격 시 25%%확률로 출혈 1 부여"));
		TextMain_2->SetText(FText::FromString(NewWString));

		Image_3->SetBrush(Ring3);
		NewWString = FString::Printf(TEXT("에메랄드 반지"));
		ItemName_3->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("공격 시 25%%확률로 부식 1 부여"));
		TextMain_3->SetText(FText::FromString(NewWString));

		Image_4->SetBrush(Ring4);
		NewWString = FString::Printf(TEXT("토파즈 반지"));
		ItemName_4->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("공격 시 25%%확률로 약화 1 부여"));
		TextMain_4->SetText(FText::FromString(NewWString));

		Image_5->SetBrush(Ring5);
		NewWString = FString::Printf(TEXT("철 반지+"));
		ItemName_5->SetText(FText::FromString(NewWString));
		NewWString = FString::Printf(TEXT("추가 대미지 + 2"));
		TextMain_5->SetText(FText::FromString(NewWString));
	}

}

void UUI_Store3::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideStoreWidget(3, 1);
	}
}

void UUI_Store3::OnBuy1Clicked()
{
	itemnum = 1;
	ShowPopup(true);
}

void UUI_Store3::OnBuy2Clicked()
{
	itemnum = 2;
	ShowPopup(true);
}

void UUI_Store3::OnBuy3Clicked()
{
	itemnum = 3;
	ShowPopup(true);
}

void UUI_Store3::OnBuy4Clicked()
{
	itemnum = 4;
	ShowPopup(true);
}

void UUI_Store3::OnBuy5Clicked()
{
	itemnum = 5;
	ShowPopup(true);
}

void UUI_Store3::OnFrontClicked()
{
	if (page == 2)
	{
		page = 1;
	}
	else if (page == 3)
	{
		page = 2;
	}
	ChangePage();
}

void UUI_Store3::OnBehindClicked()
{
	if (page == 1)
	{
		page = 2;
	}
	else if (page == 2)
	{
		page = 3;
	}
	ChangePage();
}

void UUI_Store3::OnYesClicked()
{
	int pr = 10000;
	if (myitemtype == 1)
	{
		if (itemnum == 1)
		{
			pr = 50;
		}
		else if (itemnum == 2)
		{
			pr = 75;
		}
		else if (itemnum == 3)
		{
			pr = 75;
		}
		else if (itemnum == 4)
		{
			pr = 130;
		}
		else if (itemnum == 5)
		{
			pr = 200;
		}
	}
	else if (myitemtype == 2)
	{
		if (itemnum == 1)
		{
			pr = 100;
		}
		else if (itemnum == 2)
		{
			pr = 100;
		}
		else if (itemnum == 3)
		{
			pr = 200;
		}
		else if (itemnum == 4)
		{
			pr = 200;
		}
		else if (itemnum == 5)
		{
			pr = 200;
		}
	}
	else if (myitemtype == 3)
	{
		if (itemnum == 1)
		{
			pr = 100;
		}
		else if (itemnum == 2)
		{
			pr = 100;
		}
		else if (itemnum == 3)
		{
			pr = 150;
		}
		else if (itemnum == 4)
		{
			pr = 150;
		}
		else if (itemnum == 5)
		{
			pr = 200;
		}
	}

	BuyItem(myitemtype - 1, itemnum, pr);

	ShowPopup(false);
}

void UUI_Store3::OnNoClicked()
{
	ShowPopup(false);
}
