// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Equipment.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Styling/SlateBrush.h"
#include "Controller_MainCharacter.h"
#include "ForgottenKnightCharacter.h"

void UUI_Equipment::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ButtonBack != nullptr)
	{
		ButtonBack->OnClicked.AddDynamic(this, &UUI_Equipment::OnButtonBackClicked);
	}
	
	FString sStr = FString::Printf(TEXT("철검+%d"), AForgottenKnightCharacter::MySwordLv-1);
	if (AForgottenKnightCharacter::MySwordLv == 1)
	{
		sStr = FString::Printf(TEXT("철검"));
	}
	FText InText = FText::FromString(sStr);
	Text_SwordLv->SetText(InText);

	sStr = FString::Printf(TEXT("철갑옷+%d"), AForgottenKnightCharacter::MySheildLv-1);
	if (AForgottenKnightCharacter::MySwordLv == 1)
	{
		sStr = FString::Printf(TEXT("철갑옷"));
	}
	InText = FText::FromString(sStr);
	Text_SheildLv->SetText(InText);

	if (AForgottenKnightCharacter::MyAccessoryData[0] == 1)
	{
		Image_Necklace->SetBrush(Necklace1);
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[0] == 2)
	{
		Image_Necklace->SetBrush(Necklace2);
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[0] == 3)
	{
		Image_Necklace->SetBrush(Necklace3);
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[0] == 4)
	{
		Image_Necklace->SetBrush(Necklace4);
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[0] == 5)
	{
		Image_Necklace->SetBrush(Necklace5);
	}

	if (AForgottenKnightCharacter::MyAccessoryData[1] == 1)
	{
		Image_Bracelet->SetBrush(Bracelet1);
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[1] == 2)
	{
		Image_Bracelet->SetBrush(Bracelet2);
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[1] == 3)
	{
		Image_Bracelet->SetBrush(Bracelet3);
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[1] == 4)
	{
		Image_Bracelet->SetBrush(Bracelet4);
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[1] == 5)
	{
		Image_Bracelet->SetBrush(Bracelet5);
	}

	if (AForgottenKnightCharacter::MyAccessoryData[2] == 1)
	{
		Image_Ring->SetBrush(Ring1);
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[2] == 2)
	{
		Image_Ring->SetBrush(Ring2);
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[2] == 3)
	{
		Image_Ring->SetBrush(Ring3);
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[2] == 4)
	{
		Image_Ring->SetBrush(Ring4);
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[2] == 5)
	{
		Image_Ring->SetBrush(Ring5);
	}

}

void UUI_Equipment::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Image_Sword->IsHovered())
	{
		FString sStr;
		if (AForgottenKnightCharacter::MySwordLv == 1)
		{
			sStr = FString::Printf(TEXT("\n철검\n\n추가 효과 : X"));
		}
		else
		{
			sStr = FString::Printf(TEXT("\n철검+%d\n\n추가 효과 : 추가 대미지 +%d"), AForgottenKnightCharacter::MySwordLv-1, 2*(AForgottenKnightCharacter::MySwordLv-1));
		}
		FText InText = FText::FromString(sStr);
		Text_Message->SetText(InText);
	}
	else if (Image_Sheild->IsHovered())
	{
		FString sStr;
		if (AForgottenKnightCharacter::MySheildLv == 1)
		{
			sStr = FString::Printf(TEXT("\n철갑옷\n\n추가 효과 : X"));
		}
		else if (AForgottenKnightCharacter::MySheildLv == 2)
		{
			sStr = FString::Printf(TEXT("\n철갑옷+1\n\n추가 효과 : 받는 대미지 -1"));
		}
		else
		{
			sStr = FString::Printf(TEXT("철갑옷+%d\n\n추가 효과 :\n받는 대미지 -%d, 숨 고르기 효과 +%d"), AForgottenKnightCharacter::MySheildLv-1, AForgottenKnightCharacter::MySheildLv-1, AForgottenKnightCharacter::MySheildLv-1/2);
		}
		FText InText = FText::FromString(sStr);
		Text_Message->SetText(InText);
	}
	else if (Image_Passive->IsHovered())
	{
		FString sStr = FString::Printf(TEXT("\n응급 처치\n\n효과 : 전투 후 체력을 3 회복합니다"));
		FText InText = FText::FromString(sStr);
		Text_Message->SetText(InText);
	}
	else if (Image_Necklace->IsHovered())
	{
		if (AForgottenKnightCharacter::MyAccessoryData[0] == 0)
		{
			FString sStr = FString::Printf(TEXT("\n\n장착한 목걸이가 존재하지 않습니다"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[0] == 1)
		{
			FString sStr = FString::Printf(TEXT("\n루비 목걸이\n\n효과 : 최대체력 +30"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Necklace->SetBrush(Necklace1);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[0] == 2)
		{
			FString sStr = FString::Printf(TEXT("\n철 목걸이\n\n효과 : 받는 대미지 -1"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Necklace->SetBrush(Necklace2);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[0] == 3)
		{
			FString sStr = FString::Printf(TEXT("\n루비 목걸이+\n\n효과 : 최대체력 +50"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Necklace->SetBrush(Necklace3);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[0] == 4)
		{
			FString sStr = FString::Printf(TEXT("\n철 목걸이+\n\n효과 : 받는 대미지 -2"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Necklace->SetBrush(Necklace4);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[0] == 5)
		{
			FString sStr = FString::Printf(TEXT("\n진주 목걸이\n\n효과 : 첫 피해 무효화"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Necklace->SetBrush(Necklace5);
		}
	}
	else if (Image_Bracelet->IsHovered())
	{
		if (AForgottenKnightCharacter::MyAccessoryData[1] == 0)
		{
			FString sStr = FString::Printf(TEXT("\n\n장착한 팔찌가 존재하지 않습니다"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[1] == 1)
		{
			FString sStr = FString::Printf(TEXT("\n철 팔찌\n\n효과 : 최대체력 -30\n추가 대미지 +2"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Bracelet->SetBrush(Bracelet1);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[1] == 2)
		{
			FString sStr = FString::Printf(TEXT("\n루비 팔찌\n\n효과 : 최대체력 +50\n추가 대미지 -1"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Bracelet->SetBrush(Bracelet2);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[1] == 3)
		{
			FString sStr = FString::Printf(TEXT("\n다이아 팔찌\n\n효과 : 모든 회복량 +5"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Bracelet->SetBrush(Bracelet3);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[1] == 4)
		{
			FString sStr = FString::Printf(TEXT("\n에메랄드 팔찌\n\n효과 : 적에게 가하는 부식 효과 2배"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Bracelet->SetBrush(Bracelet4);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[1] == 5)
		{
			FString sStr = FString::Printf(TEXT("\n토파즈 팔찌\n\n효과 : 적에게 가하는 약화 효과 2배"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Bracelet->SetBrush(Bracelet5);
		}
	}
	else if (Image_Ring->IsHovered())
	{
		if (AForgottenKnightCharacter::MyAccessoryData[2] == 0)
		{
			FString sStr = FString::Printf(TEXT("\n\n장착한 반지가 존재하지 않습니다"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[2] == 1)
		{
			FString sStr = FString::Printf(TEXT("\n철 반지\n\n효과 : 추가 대미지 +1"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Ring->SetBrush(Ring1);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[2] == 2)
		{
			FString sStr = FString::Printf(TEXT("\n루비 반지\n\n효과 : 공격 시 25%% 확률로\n출혈 1 부여"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Ring->SetBrush(Ring2);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[2] == 3)
		{
			FString sStr = FString::Printf(TEXT("\n에메랄드 반지\n\n효과 : 공격 시 25%% 확률로\n부식 1 부여"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Ring->SetBrush(Ring3);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[2] == 4)
		{
			FString sStr = FString::Printf(TEXT("\n토파즈 반지\n\n효과 : 공격 시 25%% 확률로\n약화 1 부여"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Ring->SetBrush(Ring4);
		}
		else if (AForgottenKnightCharacter::MyAccessoryData[2] == 5)
		{
			FString sStr = FString::Printf(TEXT("\n철 반지+\n\n효과 : 추가 대미지 +2"));
			FText InText = FText::FromString(sStr);
			Text_Message->SetText(InText);
			Image_Ring->SetBrush(Ring5);
		}
	}
	else
	{
		FString sStr = FString::Printf(TEXT(" "));
		FText InText = FText::FromString(sStr);
		Text_Message->SetText(InText);
	}

}

void UUI_Equipment::OnButtonBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideEquipmentWidget();
	}
}
