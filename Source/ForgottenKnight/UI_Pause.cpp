// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Pause.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Controller_MainCharacter.h"
#include "ForgottenKnightCharacter.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UUI_Pause::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Resume != nullptr)
	{
		Button_Resume->OnClicked.AddDynamic(this, &UUI_Pause::OnButtonResumeClicked);
	}
	if (Button_Save != nullptr)
	{
		Button_Save->OnClicked.AddDynamic(this, &UUI_Pause::OnButtonSaveClicked);
	}
	if (Button_Help != nullptr)
	{
		Button_Help->OnClicked.AddDynamic(this, &UUI_Pause::OnButtonHelpClicked);
	}
	if (Button_Main != nullptr)
	{
		Button_Main->OnClicked.AddDynamic(this, &UUI_Pause::OnButtonMainClicked);
	}
	if (Button_Quit != nullptr)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UUI_Pause::OnButtonQuitClicked);
	}
	if (Button_Popup != nullptr)
	{
		Button_Popup->OnClicked.AddDynamic(this, &UUI_Pause::OnButtonPopupClicked);
	}
	if (Button_Tutorial != nullptr)
	{
		Button_Tutorial->OnClicked.AddDynamic(this, &UUI_Pause::OnButtonTutorialClicked);
	}
	if (Button_Cheat != nullptr)
	{
		Button_Cheat->OnClicked.AddDynamic(this, &UUI_Pause::OnButtonCheatClicked); // 치트 나중에 삭제
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer->Savable)
	{
		Button_Save->SetIsEnabled(true);
		Button_Cheat->SetIsEnabled(true);
	}
	else
	{
		Button_Save->SetIsEnabled(false);
		Button_Cheat->SetIsEnabled(false);
	}
}

void UUI_Pause::OnButtonCheatClicked() // 치트 나중에 삭제
{
	AForgottenKnightCharacter::MyCurrentEXP += 1000;
	AForgottenKnightCharacter::MyGold += 1000;
	for (int x = 0; x < AController_MainCharacter::MyProgress.Num(); x++)
	{
		AController_MainCharacter::MyProgress[x] = true;
	}

	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->SetLevelEXP();
		PlayerController->OutsideChangeStat();
	}
}

void UUI_Pause::OnButtonResumeClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HidePauseWidget();
	}
}

void UUI_Pause::OnButtonSaveClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowSaveSlotWidget();
	}
}

void UUI_Pause::OnButtonTutorialClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowTutorialWidget();
	}
}

void UUI_Pause::OnButtonMainClicked()
{
	UGameplayStatics::OpenLevel(this, FName("StartMap"));
}

void UUI_Pause::OnButtonQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this,
		nullptr,
		EQuitPreference::Quit,
		true);
}

void UUI_Pause::OnButtonHelpClicked()
{
	Image_Popup->SetVisibility(ESlateVisibility::Visible);
	Text_Popup->SetVisibility(ESlateVisibility::Visible);
	Button_Popup->SetVisibility(ESlateVisibility::Visible);
	Button_Tutorial->SetVisibility(ESlateVisibility::Visible);
}

void UUI_Pause::OnButtonPopupClicked()
{
	Image_Popup->SetVisibility(ESlateVisibility::Collapsed);
	Text_Popup->SetVisibility(ESlateVisibility::Collapsed);
	Button_Popup->SetVisibility(ESlateVisibility::Collapsed);
	Button_Tutorial->SetVisibility(ESlateVisibility::Collapsed);
}
