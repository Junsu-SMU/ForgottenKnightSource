// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Store2_3.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "UI_StoreCard.h"

void UUI_Store2_3::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_Store2_3::OnBackClicked);
	}
	if (Button_Front != nullptr)
	{
		Button_Front->OnClicked.AddDynamic(this, &UUI_Store2_3::OnFrontClicked);
	}
	if (Button_Upgrade != nullptr)
	{
		Button_Upgrade->OnClicked.AddDynamic(this, &UUI_Store2_3::OnBuyUpgradeClicked);
	}
	if (Button_Delete != nullptr)
	{
		Button_Delete->OnClicked.AddDynamic(this, &UUI_Store2_3::OnBuyDeleteClicked);
	}

	BuyCard1->SetCard(41, 10, 1, 1);
	BuyCard2->SetCard(42, 10, 2, 1);
	BuyCard3->SetCard(43, 10, 3, 1);
	BuyCard4->SetCard(6, 13, 4, 1);
	BuyCard5->SetCard(44, 12, 5, 1);
	BuyCard6->SetCard(45, 15, 6, 1);
	BuyCard7->SetCard(46, 13, 7, 1);
	BuyCard8->SetCard(47, 15, 8, 1);
	BuyCard9->SetCard(48, 12, 9, 1);
	BuyCard10->SetCard(49, 15, 10, 1);
	//고급카드 5레벨 이상 구입하게 바꿀 예정
	BuyCard11->SetCard(50, 20, 11, 5);
	BuyCard12->SetCard(51, 26, 12, 5);
	BuyCard13->SetCard(52, 24, 13, 5);
	BuyCard14->SetCard(53, 20, 14, 5);
	BuyCard15->SetCard(54, 20, 15, 5);
	//특수카드 10레벨 이상 구입하게 바꿀 예정
	BuyCard16->SetCard(81, 50, 16, 10);
	BuyCard17->SetCard(82, 50, 17, 10);

	BuyCard11->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard12->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard13->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard14->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard15->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard16->SetVisibility(ESlateVisibility::Collapsed);
	BuyCard17->SetVisibility(ESlateVisibility::Collapsed);

	ChangeGold();
}

void UUI_Store2_3::ChangeGold()
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

void UUI_Store2_3::PlayMessage(int Order)
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

void UUI_Store2_3::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideStoreWidget(2, 3);
	}
}

void UUI_Store2_3::OnFrontClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideStoreWidget(2, 3);
		PlayerController->ShowStoreWidget(2, 2);
	}
}

void UUI_Store2_3::OnBuyUpgradeClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowClickDeckWidget(1, -1); // 1강화
	}
}

void UUI_Store2_3::OnBuyDeleteClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowClickDeckWidget(2, -1); // 2제거
	}
}
