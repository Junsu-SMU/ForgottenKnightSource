// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Event_3.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"

void UUI_Event_3::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_Event_3::OnBackClicked);
	}
	if (Button_Upgrade != nullptr)
	{
		Button_Upgrade->OnClicked.AddDynamic(this, &UUI_Event_3::OnUpgradeClicked);
	}
	if (Button_Delete != nullptr)
	{
		Button_Delete->OnClicked.AddDynamic(this, &UUI_Event_3::OnDeleteClicked);
	}
}

void UUI_Event_3::UpdateStat()
{
	if (AController_MainCharacter::Event3Data[EventOrder] > 0)
	{
		Button_Upgrade->SetIsEnabled(true);
		Button_Delete->SetIsEnabled(true);
	}
	else
	{
		Button_Upgrade->SetIsEnabled(false);
		Button_Delete->SetIsEnabled(false);
	}
}

void UUI_Event_3::OnUpgradeClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowClickDeckWidget(3, EventOrder);
	}
}

void UUI_Event_3::OnDeleteClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowClickDeckWidget(4, EventOrder);
	}
}

void UUI_Event_3::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideEvent3Widget();
	}
}
