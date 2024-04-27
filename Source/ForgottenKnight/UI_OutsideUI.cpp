// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_OutsideUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Controller_MainCharacter.h"
#include "ForgottenKnightCharacter.h"

void UUI_OutsideUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ButtonInventory != nullptr)
	{
		ButtonInventory->OnClicked.AddDynamic(this, &UUI_OutsideUI::OnButtonInventoryClicked);
	}

	if (Button_Pause != nullptr)
	{
		Button_Pause->OnClicked.AddDynamic(this, &UUI_OutsideUI::OnButtonPauseClicked);
	}

	ChangeStat();

}

void UUI_OutsideUI::ChangeStat()
{
	FString NewWString = FString::FromInt(AForgottenKnightCharacter::MyLevel);
	FText InWText = FText::FromString(NewWString);
	Text_Level->SetText(InWText);

	NewWString = FString::FromInt(AForgottenKnightCharacter::MyGold);
	InWText = FText::FromString(NewWString + " G");
	Text_Gold->SetText(InWText);

	NewWString = FString::FromInt(AForgottenKnightCharacter::MyCurrentHealth);
	FString NewCString = FString::FromInt(AForgottenKnightCharacter::MyMaxHealth);
	InWText = FText::FromString(NewWString + " / " + NewCString);
	Text_Health->SetText(InWText);
	float PerW = float(AForgottenKnightCharacter::MyCurrentHealth) / float(AForgottenKnightCharacter::MyMaxHealth);
	Progressbar_Health->SetPercent(PerW);

	NewWString = FString::FromInt(AForgottenKnightCharacter::MyCurrentEXP);
	NewCString = FString::FromInt(AForgottenKnightCharacter::MyMaxEXP);
	InWText = FText::FromString(NewWString + " / " + NewCString);
	Text_EXP->SetText(InWText);
	PerW = float(AForgottenKnightCharacter::MyCurrentEXP) / float(AForgottenKnightCharacter::MyMaxEXP);
	Progressbar_EXP->SetPercent(PerW);
}

void UUI_OutsideUI::OnButtonInventoryClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowInventoryWidget();
	}
}

void UUI_OutsideUI::OnButtonPauseClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowPauseWidget();
	}
}
