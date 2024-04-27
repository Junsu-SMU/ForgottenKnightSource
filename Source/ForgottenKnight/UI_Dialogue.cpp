// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Dialogue.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "Containers/UnrealString.h"
#include "MyGameInstance.h"
#include "DataTable_Dialogue.h"
#include "Animation/WidgetAnimation.h"

void UUI_Dialogue::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UUI_Dialogue::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (DetectingEnter) // 엔터를 쳤으면
	{
		if (!DialEnd)
		{
			delayrating += InDeltaTime; // 프레임마다 변수값 쌓이기
			Text_Enter->SetRenderOpacity(0.f);
		}
		else
		{
			ShowAllDial();
		}
	}

	if (delayrating > TypeSpeed) // 변수값이 typespeed값 이상 쌓이면 실행
	{
		delayrating = 0; // 변수값 초기화
		ShowText(); // 함수 실행
	}
}

void UUI_Dialogue::TakeDialogueData(int DataNum)
{
	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (nullptr == MyGameInstance) return;

	FDialogueTableRow* CurrentData = MyGameInstance->GetDialogueData(DataNum);
	if (CurrentData)
	{
		if (FlowIndex == 0)
		{
			Teller = CurrentData->StartTeller;
			InputString = CurrentData->StartDialogue;
			DialLength = 8;
		}
		else if (FlowIndex == 1)
		{
			Teller = CurrentData->Teller1;
			InputString = CurrentData->Dialogue1;
			DialLength = 9;
		}
		else if (FlowIndex == 2)
		{
			Teller = CurrentData->Teller2;
			InputString = CurrentData->Dialogue2;
			DialLength = 13;
		}
		else if (FlowIndex == 3)
		{
			Teller = CurrentData->StartTeller;
			InputString = CurrentData->StartDialogue;
			DialLength = 5;
		}
		else if (FlowIndex == 4)
		{
			Teller = CurrentData->Teller1;
			InputString = CurrentData->Dialogue1;
			DialLength = 6;
		}
		else if (FlowIndex == 5)
		{
			Teller = CurrentData->Teller2;
			InputString = CurrentData->Dialogue2;
			DialLength = 10;
		}
		else if (FlowIndex == 6)
		{
			Teller = CurrentData->StartTeller;
			InputString = CurrentData->StartDialogue;
			DialLength = 3;
		}
		else if (FlowIndex == 7)
		{
			Teller = CurrentData->Teller1;
			InputString = CurrentData->Dialogue1;
			DialLength = 2;
		}
	}

	Text_Name->SetText(FText::FromString(Teller));
}

void UUI_Dialogue::ShowText()
{
	if (FirstDataLoad)
	{
		FirstDataLoad = false;

		if (FlowIndex == 0 || FlowIndex == 1 || FlowIndex == 2)
		{
			StartIndex = 0;
		}
		else if (FlowIndex == 3 || FlowIndex == 4 || FlowIndex == 5)
		{
			StartIndex = 20;
		}
		else if (FlowIndex == 6 || FlowIndex == 7)
		{
			StartIndex = 30;
		}
	}

	TakeDialogueData(StartIndex + DialIndex); // 문장을 inputstring에 전달
	DisPlayedString += InputString[TextIndex];
	FString sStr = FString::Printf(TEXT("%s"), *DisPlayedString);
	FText InText = FText::FromString(sStr);
	Text_Talk->SetText(InText); // 텍스트블록에 넣기
	TextIndex += 1; // 문자 인덱스 값 ++

	if (TextIndex == InputString.Len()) // 문장 안에 모든 문자를 보였다면
	{
		TextIndex = 0; // 텍스트 
		DialIndex += 1; // 다음 문장 준비
		DisPlayedString = {""};
		DetectingEnter = false; // 엔터 키 다시 쳐야 다음 문장 실행

		if (DialLength == DialIndex) // 모든 문장을 보였다면
		{
			DialEnd = true;
		}

		PlayAnimation(Anim_ShowEnd);

	}
}

void UUI_Dialogue::ShowEntireText()
{
	TakeDialogueData(StartIndex + DialIndex);
	FString sStr = FString::Printf(TEXT("%s"), *InputString);
	FText InText = FText::FromString(sStr);
	Text_Talk->SetText(InText); // 텍스트블록에 넣기
	TextIndex = 0; // 텍스트 
	DialIndex += 1; // 다음 문장 준비
	DisPlayedString = {""};
	DetectingEnter = false; // 엔터 키 다시 쳐야 다음 문장 실행

	if (DialLength == DialIndex) // 모든 문장을 보였다면
	{
		DialEnd = true;
	}

	PlayAnimation(Anim_ShowEnd);

}

void UUI_Dialogue::ShowAllDial()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideDialogueWidget();
	}
}
