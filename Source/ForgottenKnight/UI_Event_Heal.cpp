// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Event_Heal.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "EngineUtils.h"
#include "iostream"
#include "random"

std::random_device healrng;
std::uniform_int_distribution<int> Rand(1, 5); // [1, 5] 범위

void UUI_Event_Heal::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_Event_Heal::OnBackClicked);
	}
	if (Button_Take != nullptr)
	{
		Button_Take->OnClicked.AddDynamic(this, &UUI_Event_Heal::OnTakeClicked);
	}
	if (Button_PopBack != nullptr)
	{
		Button_PopBack->OnClicked.AddDynamic(this, &UUI_Event_Heal::OnPopBackClicked);
	}
}

void UUI_Event_Heal::Popup(bool take)
{
	AController_MainCharacter::EventHealData[EventOrder] -= 1; // 섭취 수량 -1

	if (take) // 안전한 사과 섭취 시 (체력 +5)
	{
		AForgottenKnightCharacter::MyCurrentHealth += 5;
		if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
		{
			AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
		}
	}
	else // 독사과 섭취 시 (체력 -5)
	{
		AForgottenKnightCharacter::MyCurrentHealth -= 5;
		if (AForgottenKnightCharacter::MyCurrentHealth <= 0)
		{
			AForgottenKnightCharacter::MyCurrentHealth = 1; // 체력 최소 1 남음
		}
	}

	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->OutsideChangeStat();
	}
	PopupObjectControl(take);
}

void UUI_Event_Heal::PopupObjectControl(bool Switching)
{
	Button_PopBack->SetRenderOpacity(1);
	Button_PopBack->SetVisibility(ESlateVisibility::Visible);
	Text_Popup->SetRenderOpacity(1);
	Text_Popup->SetVisibility(ESlateVisibility::Visible);
	Image_Popup->SetRenderOpacity(1);
	Image_Popup->SetVisibility(ESlateVisibility::Visible);

	if (Switching)
	{
		FString sStr = FString::Printf(TEXT("체력을 5 회복하였습니다"));
		FText InText = FText::FromString(sStr);
		Text_Popup->SetText(InText);
	}
	else
	{
		FString sStr = FString::Printf(TEXT("체력을 5 소모하였습니다"));
		FText InText = FText::FromString(sStr);
		Text_Popup->SetText(InText);
	}

	SetTextTake();
}

void UUI_Event_Heal::SetTextTake()
{
	FString sStr = FString::Printf(TEXT("남은 열매 개수 : %d"), AController_MainCharacter::EventHealData[EventOrder]);
	FText InText = FText::FromString(sStr);
	Text_Value->SetText(InText);
}

void UUI_Event_Heal::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideEventHealWidget();
	}
}

void UUI_Event_Heal::OnTakeClicked()
{
	if (AController_MainCharacter::EventHealData[EventOrder] > 0) // 섭취 수량이 1 이상이면
	{
		int x = Rand(healrng);
		if (x == 1) // 1이면 독사과
		{
			Popup(false);
		}
		else
		{
			Popup(true);
		}
	}
	else
	{
		FString sStr = FString::Printf(TEXT("남은 열매가 없습니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}
}

void UUI_Event_Heal::OnPopBackClicked()
{
	Button_PopBack->SetRenderOpacity(0);
	Button_PopBack->SetVisibility(ESlateVisibility::Collapsed);
	Text_Popup->SetRenderOpacity(0);
	Text_Popup->SetVisibility(ESlateVisibility::Collapsed);
	Image_Popup->SetRenderOpacity(0);
	Image_Popup->SetVisibility(ESlateVisibility::Collapsed);
}
