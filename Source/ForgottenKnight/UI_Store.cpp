// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Store.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"

void UUI_Store::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_Store::OnBackClicked);
	}
	if (Button_Behind != nullptr)
	{
		Button_Behind->OnClicked.AddDynamic(this, &UUI_Store::OnBehindClicked);
	}
	if (Button_BuyHeal != nullptr)
	{
		Button_BuyHeal->OnClicked.AddDynamic(this, &UUI_Store::OnBuyHealClicked);
	}
	if (Button_BuyHeal2 != nullptr)
	{
		Button_BuyHeal2->OnClicked.AddDynamic(this, &UUI_Store::OnBuyHeal2Clicked);
	}
	if (Button_BuyHeal3 != nullptr)
	{
		Button_BuyHeal3->OnClicked.AddDynamic(this, &UUI_Store::OnBuyHeal3Clicked);
	}
	if (Button_BuyMaxHeal != nullptr)
	{
		Button_BuyMaxHeal->OnClicked.AddDynamic(this, &UUI_Store::OnBuyMaxHealClicked);
	}
	if (Button_BuyMaxHeal2 != nullptr)
	{
		Button_BuyMaxHeal2->OnClicked.AddDynamic(this, &UUI_Store::OnBuyMaxHeal2Clicked);
	}
	if (Button_BuyMaxHeal3 != nullptr)
	{
		Button_BuyMaxHeal3->OnClicked.AddDynamic(this, &UUI_Store::OnBuyMaxHeal3Clicked);
	}
	if (Button_BuySpecial != nullptr)
	{
		Button_BuySpecial->OnClicked.AddDynamic(this, &UUI_Store::OnBuySpecialClicked);
	}
	if (Button_BuySpecial2 != nullptr)
	{
		Button_BuySpecial2->OnClicked.AddDynamic(this, &UUI_Store::OnBuySpecial2Clicked);
	}
	if (Button_BuySpecial3 != nullptr)
	{
		Button_BuySpecial3->OnClicked.AddDynamic(this, &UUI_Store::OnBuySpecial3Clicked);
	}

	ChangeGold();
}

void UUI_Store::ChangeGold()
{
	FString NewWString = FString::FromInt(AForgottenKnightCharacter::MyGold);
	FText InWText = FText::FromString(NewWString + " G");
	Text_Gold->SetText(InWText);
	NewWString = FString::FromInt(AController_MainCharacter::Store1_1Count[0]);
	InWText = FText::FromString(NewWString);
	Text_Count0->SetText(InWText);
	NewWString = FString::FromInt(AController_MainCharacter::Store1_1Count[1]);
	InWText = FText::FromString(NewWString);
	Text_Count1->SetText(InWText);
	NewWString = FString::FromInt(AController_MainCharacter::Store1_1Count[2]);
	InWText = FText::FromString(NewWString);
	Text_Count2->SetText(InWText);
	NewWString = FString::FromInt(AController_MainCharacter::Store1_1Count[3]);
	InWText = FText::FromString(NewWString);
	Text_Count3->SetText(InWText);
	NewWString = FString::FromInt(AController_MainCharacter::Store1_1Count[4]);
	InWText = FText::FromString(NewWString);
	Text_Count4->SetText(InWText);
	NewWString = FString::FromInt(AController_MainCharacter::Store1_1Count[5]);
	InWText = FText::FromString(NewWString);
	Text_Count5->SetText(InWText);
	NewWString = FString::FromInt(AController_MainCharacter::Store1_1Count[6]);
	InWText = FText::FromString(NewWString);
	Text_Count6->SetText(InWText);
	NewWString = FString::FromInt(AController_MainCharacter::Store1_1Count[7]);
	InWText = FText::FromString(NewWString);
	Text_Count7->SetText(InWText);
	NewWString = FString::FromInt(AController_MainCharacter::Store1_1Count[8]);
	InWText = FText::FromString(NewWString);
	Text_Count8->SetText(InWText);

	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->UpdateOutsideStat();
	}
}

void UUI_Store::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideStoreWidget(1, 1);
	}
}

void UUI_Store::OnBehindClicked() // 미완
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideStoreWidget(1, 1);
		PlayerController->ShowStoreWidget(1, 2);
	}
}

void UUI_Store::BuyHeal(int price, int count, int value)
{
	if (AForgottenKnightCharacter::MyGold >= price)
	{
		if (AController_MainCharacter::Store1_1Count[count] > 0)
		{
			AForgottenKnightCharacter::MyGold -= price;
			AForgottenKnightCharacter::MyCurrentHealth += value;
			AController_MainCharacter::Store1_1Count[count] -= 1;
			if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
			{
				AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
			}
			ChangeGold();
		}
		else
		{
			FString sStr = FString::Printf(TEXT("남은 매물이 없습니다"));
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

void UUI_Store::BuyMaxHeal(int price, int count, int value)
{
	UE_LOG(LogTemp, Log, TEXT("3434"));
	if (AForgottenKnightCharacter::MyGold >= price)
	{
		UE_LOG(LogTemp, Log, TEXT("4545"));
		if (AController_MainCharacter::Store1_1Count[count] > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("5656"));
			AForgottenKnightCharacter::MyGold -= price;
			AForgottenKnightCharacter::MyMaxHealth += value;
			AController_MainCharacter::Store1_1Count[count] -= 1;

			ChangeGold();
		}
		else
		{
			FString sStr = FString::Printf(TEXT("남은 매물이 없습니다"));
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

void UUI_Store::OnBuyHealClicked()
{
	BuyHeal(5, 0, 5);
}

void UUI_Store::OnBuyHeal2Clicked()
{
	BuyHeal(9, 1, 10);
}

void UUI_Store::OnBuyHeal3Clicked()
{
	BuyHeal(13, 2, 15);
}

void UUI_Store::OnBuyMaxHealClicked()
{
	BuyMaxHeal(8, 3, 5);
}

void UUI_Store::OnBuyMaxHeal2Clicked()
{
	BuyMaxHeal(18, 4, 10);
}

void UUI_Store::OnBuyMaxHeal3Clicked()
{
	BuyMaxHeal(27, 5, 15);
}

void UUI_Store::OnBuySpecialClicked()
{
	BuyHeal(7, 6, 8);
}

void UUI_Store::OnBuySpecial2Clicked()
{
	BuyHeal(24, 7, 30);
}

void UUI_Store::OnBuySpecial3Clicked()
{
	UE_LOG(LogTemp, Log, TEXT("2323"));
	BuyMaxHeal(35, 8, 20);
}
