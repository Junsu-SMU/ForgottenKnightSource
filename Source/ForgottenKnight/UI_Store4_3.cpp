// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Store4_3.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "UI_StoreCard.h"

void UUI_Store4_3::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_Store4_3::OnBackClicked);
	}
	if (Button_Front != nullptr)
	{
		Button_Front->OnClicked.AddDynamic(this, &UUI_Store4_3::OnFrontClicked);
	}
	if (Button_Upgrade != nullptr)
	{
		Button_Upgrade->OnClicked.AddDynamic(this, &UUI_Store4_3::OnBuyUpgradeClicked);
	}
	if (Button_Delete != nullptr)
	{
		Button_Delete->OnClicked.AddDynamic(this, &UUI_Store4_3::OnBuyDeleteClicked);
	}
	// 상점1 카드들
	BuyCard1->SetCard(2, 10, 1, 1);
	BuyCard2->SetCard(3, 10, 2, 1);
	BuyCard3->SetCard(4, 10, 3, 1);
	BuyCard4->SetCard(5, 10, 4, 1);
	BuyCard5->SetCard(7, 12, 5, 1);
	BuyCard6->SetCard(8, 12, 6, 1);
	BuyCard7->SetCard(9, 12, 7, 1);
	BuyCard8->SetCard(10, 13, 8, 1);
	BuyCard9->SetCard(11, 15, 9, 1);
	BuyCard10->SetCard(12, 15, 10, 1);
	BuyCard11->SetCard(13, 10, 11, 1);
	BuyCard12->SetCard(14, 10, 12, 1);
	BuyCard13->SetCard(15, 10, 13, 1);
	BuyCard14->SetCard(16, 10, 14, 1);
	BuyCard15->SetCard(17, 10, 15, 1);
	BuyCard16->SetCard(18, 10, 16, 1);
	// 상점2 카드들
	BuyCard17->SetCard(41, 10, 17, 1);
	BuyCard18->SetCard(42, 10, 18, 1);
	BuyCard19->SetCard(43, 10, 19, 1);
	BuyCard20->SetCard(6, 13, 20, 1);
	BuyCard21->SetCard(44, 12, 21, 1);
	BuyCard22->SetCard(45, 15, 22, 1);
	BuyCard23->SetCard(46, 13, 23, 1);
	BuyCard24->SetCard(47, 15, 24, 1);
	BuyCard25->SetCard(48, 12, 25, 1);
	BuyCard26->SetCard(49, 15, 26, 1);
	// 신규 카드들 9장
	BuyCard27->SetCard(19, 10, 27, 1);
	BuyCard28->SetCard(20, 10, 28, 1);
	BuyCard29->SetCard(21, 10, 29, 1);
	BuyCard30->SetCard(22, 10, 30, 1);
	BuyCard31->SetCard(23, 13, 31, 1);
	BuyCard32->SetCard(24, 10, 32, 1);
	BuyCard33->SetCard(25, 15, 33, 1);
	BuyCard34->SetCard(26, 15, 34, 1);
	BuyCard35->SetCard(27, 15, 35, 1);

	//고급카드 5레벨 이상 구입하게 바꿀 예정
	// 상점1 카드들
	BuyCard36->SetCard(31, 26, 36, 5);
	BuyCard37->SetCard(32, 20, 37, 5);
	BuyCard38->SetCard(33, 24, 38, 5);
	BuyCard39->SetCard(34, 20, 39, 5);
	// 상점2 카드들
	BuyCard40->SetCard(50, 20, 40, 5);
	BuyCard41->SetCard(51, 26, 41, 5);
	BuyCard42->SetCard(52, 24, 42, 5);
	BuyCard43->SetCard(53, 20, 43, 5);
	BuyCard44->SetCard(54, 20, 44, 5);
	// 신규 카드들 3장
	BuyCard45->SetCard(35, 30, 45, 5);
	BuyCard46->SetCard(36, 20, 46, 5);
	BuyCard47->SetCard(37, 20, 47, 5);

	//특수카드 10레벨 이상 구입하게 바꿀 예정
	BuyCard48->SetCard(83, 50, 48, 10);
	BuyCard49->SetCard(84, 50, 49, 10);

	BuyCard31->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard36->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard37->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard38->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard39->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard40->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard41->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard42->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard43->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard44->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard45->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard46->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard47->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard48->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard49->SetVisibility(ESlateVisibility::Collapsed);

	ChangeGold();
}

void UUI_Store4_3::ChangeGold()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	FString NewWString = FString::FromInt(AForgottenKnightCharacter::MyGold);
	FText InWText = FText::FromString(NewWString + " G");
	Text_Gold->SetText(InWText);
	NewWString = FString::FromInt(100);
	InWText = FText::FromString(NewWString + " G");
	Text_UpgradePrice->SetText(InWText);
	int x = 0;
	if (PlayerController->StoreOrderNum == 1)
	{
		x = (25 * AController_MainCharacter::StoreDeleteStack[0]) + 50;
	}
	else if (PlayerController->StoreOrderNum == 2)
	{
		x = (25 * AController_MainCharacter::StoreDeleteStack[1]) + 50;
	}
	else if (PlayerController->StoreOrderNum == 4)
	{
		x = (25 * AController_MainCharacter::StoreDeleteStack[2]) + 50;
	}
	NewWString = FString::FromInt(x);
	InWText = FText::FromString(NewWString + " G");
	Text_DeletePrice->SetText(InWText);
}

void UUI_Store4_3::PlayMessage(int Order)
{
	if (Order == 1)
	{
		FString sStr = FString::Printf(TEXT("골드가 부족합니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}
	else if (Order == 2)
	{
		FString sStr = FString::Printf(TEXT("남은 매물이 없습니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}
	else if (Order == 3)
	{
		FString sStr = FString::Printf(TEXT("레벨이 부족합니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}
}

void UUI_Store4_3::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideStoreWidget(4, 3);
	}
}

void UUI_Store4_3::OnFrontClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideStoreWidget(4, 3);
		PlayerController->ShowStoreWidget(4, 2);
	}
}

void UUI_Store4_3::OnBuyUpgradeClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowClickDeckWidget(1, -1); // 1강화
	}
}

void UUI_Store4_3::OnBuyDeleteClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowClickDeckWidget(2, -1); // 2제거
	}
}
