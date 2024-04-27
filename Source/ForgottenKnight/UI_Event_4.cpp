// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Event_4.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"

void UUI_Event_4::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_Event_4::OnBackClicked);
	}
	if (Button_Upgrade != nullptr)
	{
		Button_Upgrade->OnClicked.AddDynamic(this, &UUI_Event_4::OnUpgradeClicked);
	}
}

void UUI_Event_4::UpdateStat()
{
	if (AController_MainCharacter::Event4Data[EventOrder])
	{
		Button_Upgrade->SetIsEnabled(true);
	}
	else
	{
		Button_Upgrade->SetIsEnabled(false);
	}

	if (EventOrder == 1)
	{
		FString sStr = FString::Printf(TEXT("마물을 피해 숨어있던 대장장이와 만났다\n장비의 강화를 싸게 해주겠다고 한다\n(강화레벨 3이하만 가능)"));
		FText InText = FText::FromString(sStr);
		Text_Stage->SetText(InText);
	}
	else
	{
		FString sStr = FString::Printf(TEXT("마물을 피해 숨어있던 대장장이와 만났다\n장비의 강화를 싸게 해주겠다고 한다"));
		FText InText = FText::FromString(sStr);
		Text_Stage->SetText(InText);
	}
}

void UUI_Event_4::OnUpgradeClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowEvent4PopupWidget(EventOrder);
		PlayerController->HideEvent4Widget();
	}
}

void UUI_Event_4::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideEvent4Widget();
	}
}
