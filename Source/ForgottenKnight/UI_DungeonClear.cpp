// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_DungeonClear.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "Animation/WidgetAnimation.h"

void UUI_DungeonClear::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_DungeonClear::OnBackClicked);
	}
	if (Button_Go != nullptr)
	{
		Button_Go->OnClicked.AddDynamic(this, &UUI_DungeonClear::OnGoClicked);
	}
}

void UUI_DungeonClear::ChangeStat(int EXP, int Gold, int clear)
{
	ClearNum = clear;

	FString NewWString = FString::FromInt(EXP);
	FText InWText = FText::FromString(NewWString);
	Text_EXP->SetText(InWText);

	NewWString = FString::FromInt(Gold);
	InWText = FText::FromString(NewWString);
	Text_Gold->SetText(InWText);

	NewWString = FString::FromInt(AForgottenKnightCharacter::MyCurrentHealth);
	InWText = FText::FromString(NewWString);
	Text_HP->SetText(InWText);

	if (clear == 0)
	{
		FString sStr = FString::Printf(TEXT("\n아직 더 들어갈 수\n있습니다"));
		FText InText = FText::FromString(sStr);
		Text_Message->SetText(InText);

		Button_Back->SetIsEnabled(false);
		Button_Go->SetIsEnabled(true);
	}
	else if (clear == -1)
	{
		FString sStr = FString::Printf(TEXT("\n이미 던전을\n정복하셨습니다"));
		FText InText = FText::FromString(sStr);
		Text_Message->SetText(InText);

		Button_Back->SetIsEnabled(true);
		Button_Go->SetIsEnabled(false);
	}
	else if (clear == 1)
	{
		FString sStr = FString::Printf(TEXT("던전을 정복했습니다\n<추가 보상>\nEXP:50 Gold:100\n체력 30% 회복"));
		FText InText = FText::FromString(sStr);
		Text_Message->SetText(InText);

		Button_Back->SetIsEnabled(true);
		Button_Go->SetIsEnabled(false);
	}
	else if (clear == 2)
	{
		FString sStr = FString::Printf(TEXT("던전을 정복했습니다\n<추가 보상>\nEXP:70 Gold:150\n체력 30% 회복"));
		FText InText = FText::FromString(sStr);
		Text_Message->SetText(InText);

		Button_Back->SetIsEnabled(true);
		Button_Go->SetIsEnabled(false);
	}
	else if (clear == 3)
	{
		FString sStr = FString::Printf(TEXT("던전을 정복했습니다\n<추가 보상>\nEXP:100 Gold:500\n체력 50% 회복"));
		FText InText = FText::FromString(sStr);
		Text_Message->SetText(InText);

		Button_Back->SetIsEnabled(true);
		Button_Go->SetIsEnabled(false);
	}
	else if (clear == 4)
	{
		FString sStr = FString::Printf(TEXT("던전을 정복했습니다\n<추가 보상>\nEXP:200 Gold:750\n체력 100% 회복"));
		FText InText = FText::FromString(sStr);
		Text_Message->SetText(InText);

		Button_Back->SetIsEnabled(true);
		Button_Go->SetIsEnabled(false);
	}
}

void UUI_DungeonClear::OnBackClicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer != nullptr)
	{
		AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (PlayerController != nullptr)
		{
			PlayerController->HideDungeonClearWidget();
			if (AForgottenKnightCharacter::MyMapLive == 1)
			{
				UGameplayStatics::OpenLevel(this, FName("OutsideMap"));
			}
			else
			{
				UGameplayStatics::OpenLevel(this, FName("OutsideMap2"));
			}
		}
	}
}

void UUI_DungeonClear::OnGoClicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer != nullptr)
	{
		AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (PlayerController != nullptr)
		{
			PlayerController->HideDungeonClearWidget();

			if (MyPlayer->ActorHasTag("Dungeon1_1"))
			{
				UGameplayStatics::OpenLevel(this, FName("Dungeon1_2"));
			}
			else if (MyPlayer->ActorHasTag("Dungeon1_2"))
			{
				UGameplayStatics::OpenLevel(this, FName("Dungeon1_3"));
			}
			else if (MyPlayer->ActorHasTag("Dungeon2_1"))
			{
				UGameplayStatics::OpenLevel(this, FName("Dungeon2_2"));
			}
			else if (MyPlayer->ActorHasTag("Dungeon2_2"))
			{
				UGameplayStatics::OpenLevel(this, FName("Dungeon2_3"));
			}
			else if (MyPlayer->ActorHasTag("Dungeon3_1"))
			{
				UGameplayStatics::OpenLevel(this, FName("Dungeon3_2"));
			}
			else if (MyPlayer->ActorHasTag("Dungeon3_2"))
			{
				UGameplayStatics::OpenLevel(this, FName("Dungeon3_3"));
			}
			else if (MyPlayer->ActorHasTag("Dungeon4_1"))
			{
				UGameplayStatics::OpenLevel(this, FName("Dungeon4_2"));
			}
			else if (MyPlayer->ActorHasTag("Dungeon4_2"))
			{
				UGameplayStatics::OpenLevel(this, FName("Dungeon4_3"));
			}
		}
	}
}

void UUI_DungeonClear::ShowLvUp()
{
	PlayAnimation(Anim_LvUp);
}
