// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Event_Fountain.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "EngineUtils.h"

void UUI_Event_Fountain::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_Event_Fountain::OnBackClicked);
	}
	if (Button_Take != nullptr)
	{
		Button_Take->OnClicked.AddDynamic(this, &UUI_Event_Fountain::OnTakeClicked);
	}
	if (Button_PopBack != nullptr)
	{
		Button_PopBack->OnClicked.AddDynamic(this, &UUI_Event_Fountain::OnPopBackClicked);
	}
}

void UUI_Event_Fountain::Popup()
{
	AController_MainCharacter::EventFountainData[EventOrder] -= 1; // 섭취 수량 -1

	AForgottenKnightCharacter::MyCurrentHealth += 10;
	if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
	{
		AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
	}
	

	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->OutsideChangeStat();
	}

	PopupObjectControl();
}

void UUI_Event_Fountain::PopupObjectControl()
{
	Button_PopBack->SetRenderOpacity(1);
	Button_PopBack->SetVisibility(ESlateVisibility::Visible);
	Text_Popup->SetRenderOpacity(1);
	Text_Popup->SetVisibility(ESlateVisibility::Visible);
	Image_Popup->SetRenderOpacity(1);
	Image_Popup->SetVisibility(ESlateVisibility::Visible);

	FString sStr = FString::Printf(TEXT("체력을 10 회복하였습니다"));
	FText InText = FText::FromString(sStr);
	Text_Popup->SetText(InText);

	SetTextTake();
}

void UUI_Event_Fountain::SetTextTake()
{
	FString sStr = FString::Printf(TEXT("남은 기회 : %d번"), AController_MainCharacter::EventFountainData[EventOrder]);
	FText InText = FText::FromString(sStr);
	Text_Value->SetText(InText);
}

void UUI_Event_Fountain::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideEventFountainWidget();
	}
}

void UUI_Event_Fountain::OnTakeClicked()
{
	if (AController_MainCharacter::EventFountainData[EventOrder] > 0) // 섭취 수량이 1 이상이면
	{
		Popup();
	}
	else
	{
		FString sStr = FString::Printf(TEXT("남은 기회가 없습니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}
}

void UUI_Event_Fountain::OnPopBackClicked()
{
	Button_PopBack->SetRenderOpacity(0);
	Button_PopBack->SetVisibility(ESlateVisibility::Collapsed);
	Text_Popup->SetRenderOpacity(0);
	Text_Popup->SetVisibility(ESlateVisibility::Collapsed);
	Image_Popup->SetRenderOpacity(0);
	Image_Popup->SetVisibility(ESlateVisibility::Collapsed);
}
