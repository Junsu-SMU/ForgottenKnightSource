// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ShowScript.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "Kismet/GameplayStatics.h"

void UUI_ShowScript::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UUI_ShowScript::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UUI_ShowScript::showpage()
{
	if (whatpurpose == 1) // 프롤로그 페이지
	{
		DialLength = 4;

		if (DialIndex == 0)
		{
			InputString = FString::Printf(TEXT("대륙 구석의 한 작은 왕국.\n그곳에는 소심하지만 착한 왕과 그런 왕을 바로 옆에서 항상 보필해주던 왕국 최고의 기사가 있었다.\n그러던 어느 날 왕국 시찰을 나간 왕의 귀에 스쳐지나간 잡담 한마디.\n\n\"저 기사가 없었다면 진작 왕은 바뀌었을 것이다.\""));
		}
		else if (DialIndex == 1)
		{
			InputString = FString::Printf(TEXT("누구나 할 수 있는 아무것도 아닌 한마디였지만 그 한마디가 기묘하게 왕의 마음을 사로잡았다.\n\n'만약 이 기사가 나를 공격한다면 그 누가 막을 수 있는가.'\n\n처음엔 그냥 흘려넘겼던 생각이지만 시간이 지나도 그러한 생각은 사라지지 않고 점점 왕의 마음속에서 부풀어만 갔다.\n바로옆에서 항상 지켜주던 기사의 존재가 오히려 항상 자신을 노리는 단두대의 칼날로 여겨졌던 것이다."));
		}
		else if (DialIndex == 2)
		{
			InputString = FString::Printf(TEXT("결국 심약한 왕은 견디지 못하고 충직한 기사를 왕국 구석의 산속으로 유배 보낸다.\n기사는 왕의 명령에 거스르지 않고 조용히 산속으로 들어가\n언젠가 다시 왕이 자신을 필요로 할 때를 대비하여 수련을 하며 보냈다.\n\n그리고 그로부터 수년 후, 기사의 앞에 마치 지능을 가진듯이 공격해오는 특이한 마물이 나타난다."));
		}
		else if (DialIndex == 3)
		{
			InputString = FString::Printf(TEXT("기사는 왕국에 무언가 이변이 일어났음을 직감하고 수상한 마물의 근원을 찾아 다시 한번 검을 든다."));
		}
	}
	else if (whatpurpose == 2) // 엔딩 페이지
	{
		DialLength = 1;

		if (DialIndex == 0)
		{
			InputString = FString::Printf(TEXT("기사는 살아남은 병사들을 모아서 남은 마물을 처리하며 다시 한 번 왕국에 평화를 가져왔다.\n그리고 그러한 기사의 헌신을 본 백성들은 한마음으로 기사를 새로운 왕으로 추대했다.\n처음에는 거절했던 기사지만 계속된 설득과 왕의 타락에 대한 약간의 부채감으로 결국 왕을 받아들인다.\n기사는 왕이 되어 다시 한 번 왕국의 부흥을 위해 일하기로 한다."));
		}
	}
}

void UUI_ShowScript::ShowText()
{
	showpage();
	DisPlayedString += InputString[TextIndex];
	FString sStr = FString::Printf(TEXT("%s"), *DisPlayedString);
	FText InText = FText::FromString(sStr);
	Text_Talk->SetText(InText); // 텍스트블록에 넣기
	TextIndex += 1; // 문자 인덱스 값 ++

	if (TextIndex == InputString.Len()) // 문장 안에 모든 문자를 보였다면
	{
		TextIndex = 0; // 텍스트 
		DialIndex += 1; // 다음 문장 준비
		DisPlayedString = { "" };
		DetectingEnter = false; // 엔터 키 다시 쳐야 다음 문장 실행

		if (DialLength == DialIndex) // 모든 문장을 보였다면
		{
			DialEnd = true;
		}

		PlayAnimation(Anim_ShowEnd);

	}
}

void UUI_ShowScript::ShowEntireText()
{
	FString sStr = FString::Printf(TEXT("%s"), *InputString);
	FText InText = FText::FromString(sStr);
	Text_Talk->SetText(InText); // 텍스트블록에 넣기
	TextIndex = 0; // 텍스트 
	DialIndex += 1; // 다음 문장 준비
	DisPlayedString = { "" };
	DetectingEnter = false; // 엔터 키 다시 쳐야 다음 문장 실행

	if (DialLength == DialIndex) // 모든 문장을 보였다면
	{
		DialEnd = true;
	}

	PlayAnimation(Anim_ShowEnd);

}

void UUI_ShowScript::ShowAllDial()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideScriptWidget();
	}
}
