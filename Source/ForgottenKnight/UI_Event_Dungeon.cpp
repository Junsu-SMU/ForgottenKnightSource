// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Event_Dungeon.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"

void UUI_Event_Dungeon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_Event_Dungeon::OnBackClicked);
	}
	if (Button_Enter != nullptr)
	{
		Button_Enter->OnClicked.AddDynamic(this, &UUI_Event_Dungeon::OnEnterClicked);
	}

	MyStage = 0;
}

void UUI_Event_Dungeon::ChangeStat(int Stage)
{
	MyStage = Stage;

	if (Stage == 1)
	{
		if (AController_MainCharacter::EventDungeonData[1] != 3)
		{
			FString NewWString = FString::FromInt(100);
			FText InWText = FText::FromString(NewWString + " G");
			Text_Gold->SetText(InWText);

			NewWString = FString::FromInt(50);
			InWText = FText::FromString(NewWString);
			Text_EXP->SetText(InWText);

			InWText = FText::FromString("30%");
			Text_Heal->SetText(InWText);
		}
		else
		{
			FString NewWString = FString::FromInt(0);
			FText InWText = FText::FromString(NewWString + " G");
			Text_Gold->SetText(InWText);

			NewWString = FString::FromInt(0);
			InWText = FText::FromString(NewWString);
			Text_EXP->SetText(InWText);

			InWText = FText::FromString("0");
			Text_Heal->SetText(InWText);

			Button_Enter->SetIsEnabled(false);
		}


		if (AController_MainCharacter::EventDungeonData[1] == 0)
		{
			FText InWText = FText::FromString("0 / 3");
			Text_Clear->SetText(InWText);
		}
		else if (AController_MainCharacter::EventDungeonData[1] == 1)
		{
			FText InWText = FText::FromString("1 / 3");
			Text_Clear->SetText(InWText);
		}
		else if (AController_MainCharacter::EventDungeonData[1] == 2)
		{
			FText InWText = FText::FromString("2 / 3");
			Text_Clear->SetText(InWText);
		}
		else if (AController_MainCharacter::EventDungeonData[1] == 3)
		{
			FText InWText = FText::FromString("3 / 3");
			Text_Clear->SetText(InWText);
		}
	}
	else if (Stage == 2)
	{
		if (AController_MainCharacter::EventDungeonData[2] != 3)
		{
			FString NewWString = FString::FromInt(150);
			FText InWText = FText::FromString(NewWString + " G");
			Text_Gold->SetText(InWText);

			NewWString = FString::FromInt(70);
			InWText = FText::FromString(NewWString);
			Text_EXP->SetText(InWText);

			InWText = FText::FromString("30%");
			Text_Heal->SetText(InWText);
		}
		else
		{
			FString NewWString = FString::FromInt(0);
			FText InWText = FText::FromString(NewWString + " G");
			Text_Gold->SetText(InWText);

			NewWString = FString::FromInt(0);
			InWText = FText::FromString(NewWString);
			Text_EXP->SetText(InWText);

			InWText = FText::FromString("0");
			Text_Heal->SetText(InWText);

			Button_Enter->SetIsEnabled(false);
		}


		if (AController_MainCharacter::EventDungeonData[2] == 0)
		{
			FText InWText = FText::FromString("0 / 3");
			Text_Clear->SetText(InWText);
		}
		else if (AController_MainCharacter::EventDungeonData[2] == 1)
		{
			FText InWText = FText::FromString("1 / 3");
			Text_Clear->SetText(InWText);
		}
		else if (AController_MainCharacter::EventDungeonData[2] == 2)
		{
			FText InWText = FText::FromString("2 / 3");
			Text_Clear->SetText(InWText);
		}
		else if (AController_MainCharacter::EventDungeonData[2] == 3)
		{
			FText InWText = FText::FromString("3 / 3");
			Text_Clear->SetText(InWText);
		}
	}
	else if (Stage == 3)
	{
		if (AController_MainCharacter::EventDungeonData[3] != 3)
		{
			FString NewWString = FString::FromInt(500);
			FText InWText = FText::FromString(NewWString + " G");
			Text_Gold->SetText(InWText);

			NewWString = FString::FromInt(100);
			InWText = FText::FromString(NewWString);
			Text_EXP->SetText(InWText);

			InWText = FText::FromString("50%");
			Text_Heal->SetText(InWText);
		}
		else
		{
			FString NewWString = FString::FromInt(0);
			FText InWText = FText::FromString(NewWString + " G");
			Text_Gold->SetText(InWText);

			NewWString = FString::FromInt(0);
			InWText = FText::FromString(NewWString);
			Text_EXP->SetText(InWText);

			InWText = FText::FromString("0");
			Text_Heal->SetText(InWText);

			Button_Enter->SetIsEnabled(false);
		}


		if (AController_MainCharacter::EventDungeonData[3] == 0)
		{
			FText InWText = FText::FromString("0 / 3");
			Text_Clear->SetText(InWText);
		}
		else if (AController_MainCharacter::EventDungeonData[3] == 1)
		{
			FText InWText = FText::FromString("1 / 3");
			Text_Clear->SetText(InWText);
		}
		else if (AController_MainCharacter::EventDungeonData[3] == 2)
		{
			FText InWText = FText::FromString("2 / 3");
			Text_Clear->SetText(InWText);
		}
		else if (AController_MainCharacter::EventDungeonData[3] == 3)
		{
			FText InWText = FText::FromString("3 / 3");
			Text_Clear->SetText(InWText);
		}
	}
	else if (Stage == 4)
	{
		if (AController_MainCharacter::EventDungeonData[4] != 3)
		{
			FString NewWString = FString::FromInt(750);
			FText InWText = FText::FromString(NewWString + " G");
			Text_Gold->SetText(InWText);

			NewWString = FString::FromInt(200);
			InWText = FText::FromString(NewWString);
			Text_EXP->SetText(InWText);

			InWText = FText::FromString("100%");
			Text_Heal->SetText(InWText);
		}
		else
		{
			FString NewWString = FString::FromInt(0);
			FText InWText = FText::FromString(NewWString + " G");
			Text_Gold->SetText(InWText);

			NewWString = FString::FromInt(0);
			InWText = FText::FromString(NewWString);
			Text_EXP->SetText(InWText);

			InWText = FText::FromString("0");
			Text_Heal->SetText(InWText);

			Button_Enter->SetIsEnabled(false);
		}


		if (AController_MainCharacter::EventDungeonData[4] == 0)
		{
			FText InWText = FText::FromString("0 / 3");
			Text_Clear->SetText(InWText);
		}
		else if (AController_MainCharacter::EventDungeonData[4] == 1)
		{
			FText InWText = FText::FromString("1 / 3");
			Text_Clear->SetText(InWText);
		}
		else if (AController_MainCharacter::EventDungeonData[4] == 2)
		{
			FText InWText = FText::FromString("2 / 3");
			Text_Clear->SetText(InWText);
		}
		else if (AController_MainCharacter::EventDungeonData[4] == 3)
		{
			FText InWText = FText::FromString("3 / 3");
			Text_Clear->SetText(InWText);
		}
	}
}

void UUI_Event_Dungeon::OnEnterClicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyStage == 1 || MyStage == 2)
	{
		AForgottenKnightCharacter::MyStartTransform = MyPlayer->GetTransform();
	}
	else if (MyStage == 3 || MyStage == 4)
	{
		AForgottenKnightCharacter::MyStartTransform2 = MyPlayer->GetTransform();
	}

	if (MyStage == 1)
	{
		UGameplayStatics::OpenLevel(this, FName("Dungeon1_1"));
	}
	else if (MyStage == 2)
	{
		UGameplayStatics::OpenLevel(this, FName("Dungeon2_1"));
	}
	else if (MyStage == 3)
	{
		UGameplayStatics::OpenLevel(this, FName("Dungeon3_1"));
	}
	else if (MyStage == 4)
	{
		UGameplayStatics::OpenLevel(this, FName("Dungeon4_1"));
	}
}

void UUI_Event_Dungeon::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideEventDungeonWidget();
	}
}
