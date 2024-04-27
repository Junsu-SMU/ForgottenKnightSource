// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Store1_3.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "UI_StoreCard.h"

void UUI_Store1_3::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_Store1_3::OnBackClicked);
	}
	if (Button_Front != nullptr)
	{
		Button_Front->OnClicked.AddDynamic(this, &UUI_Store1_3::OnFrontClicked);
	}
	if (Button_Upgrade != nullptr)
	{
		Button_Upgrade->OnClicked.AddDynamic(this, &UUI_Store1_3::OnBuyUpgradeClicked);
	}
	if (Button_Delete != nullptr)
	{
		Button_Delete->OnClicked.AddDynamic(this, &UUI_Store1_3::OnBuyDeleteClicked);
	}

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
	//고급카드 5레벨 이상 구입하게 바꿀 예정
	BuyCard17->SetCard(31, 26, 17, 5);
	BuyCard18->SetCard(32, 20, 18, 5);
	BuyCard19->SetCard(33, 24, 19, 5);
	BuyCard20->SetCard(34, 20, 20, 5);

	BuyCard17->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard18->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard19->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard20->SetVisibility(ESlateVisibility::Collapsed);

	ChangeGold();
}

void UUI_Store1_3::ChangeGold()
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
	else if (PlayerController->StoreOrderNum == 3)
	{
		x = (25 * AController_MainCharacter::StoreDeleteStack[2]) + 50;
	}
	NewWString = FString::FromInt(x);
	InWText = FText::FromString(NewWString + " G");
	Text_DeletePrice->SetText(InWText);
}

void UUI_Store1_3::PlayMessage(int Order)
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

void UUI_Store1_3::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideStoreWidget(1, 3);
	}
}

void UUI_Store1_3::OnFrontClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideStoreWidget(1, 3);
		PlayerController->ShowStoreWidget(1, 2);
	}
}

void UUI_Store1_3::OnBuyUpgradeClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowClickDeckWidget(1, -1); // 1강화
	}
}

void UUI_Store1_3::OnBuyDeleteClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowClickDeckWidget(2, -1); // 2제거
	}
}
