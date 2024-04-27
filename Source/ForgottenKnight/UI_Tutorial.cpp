// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Tutorial.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/CheckBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

void UUI_Tutorial::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ButtonBack != nullptr)
	{
		ButtonBack->OnClicked.AddDynamic(this, &UUI_Tutorial::OnButtonBackClicked);
	}
	if (Button_Next != nullptr)
	{
		Button_Next->OnClicked.AddDynamic(this, &UUI_Tutorial::OnButtonNextClicked);
	}
	if (Button_Front != nullptr)
	{
		Button_Front->OnClicked.AddDynamic(this, &UUI_Tutorial::OnButtonFrontClicked);
	}

	ShowPage(1);
}

void UUI_Tutorial::ShowPage(int page)
{
	FString sStr = FString::Printf(TEXT("%d / 6"), page);
	Text_Page->SetText(FText::FromString(sStr));

	if (page == 1)
	{
		sStr = FString::Printf(TEXT("\n\n\n\n적들은 수의 우위를 살려 동시에 공격을 해옵니다!\n그에 비해 당신은 혼자이기 때문에 공격할 순서를 정해야 합니다.\n하지만 뛰어난 검사인 당신은 다수와 싸우는 법을 잘 알고 있습니다."));
		Text_Dialogue->SetText(FText::FromString(sStr));
	}
	else if (page == 2)
	{
		sStr = FString::Printf(TEXT("\n\n\n당신의 첫 번째 공격은 뒤이어 올 상대의 공격을 방해하는 선제공격입니다.\n선제공격에 사용한 공격은 적에게 100%%의 피해를 입히며 적보다 먼저 공격합니다.\n또한, 공격을 방해받은 적은 자세가 흐트러져 50%%의 피해만을 입히게 됩니다.\n선제공격 : 가하는 피해 100%%, 받는 피해 50%%"));
		Text_Dialogue->SetText(FText::FromString(sStr));
	}
	else if (page == 3)
	{
		sStr = FString::Printf(TEXT("\n\n\n당신의 두 번째 공격은 상대와 공격타이밍을 맞춰 상대의 공격을 방어하고 때로 역으로 공격을 가하는 패링입니다.\n패링에 사용한 공격은 상대의 공격과 겨루기를 통해 데미지를 감산하여 데미지가 높은쪽의 공격만 성공합니다.\n패링에 성공할 시 대부분의 상태이상을 방어할 수 있습니다.\n패링 : 나의 공격 - 상대의 공격후 높은 쪽 공격만 성공"));
		Text_Dialogue->SetText(FText::FromString(sStr));
	}
	else if (page == 4)
	{
		sStr = FString::Printf(TEXT("\n\n\n당신의 세 번째 공격은 공격에 성공한 상대가 방심한 타이밍을 노려서 강력한 공격을 하는 카운터입니다.\n카운터에 사용한 공격은 적에게 200%%의 피해를 입히며 적보다 늦게 공격합니다.\n살을 내주고 뼈를 취하는 전략으로, 적의 피해또한 100%%를 그대로 받게됩니다.\n카운터 : 가하는 피해 200%%, 받는 피해 100%%"));
		Text_Dialogue->SetText(FText::FromString(sStr));
	}
	else if (page == 5)
	{
		sStr = FString::Printf(TEXT("\n마지막으로 당신은 전투 중에 스스로 호흡을 가다듬어 다음 공격의 피해를 증가 시킬 수 있습니다.\n숨고르기는 공격턴 하나를 소모하여 사용하는 기술로, 어떠한 피해도 주지 않지만 다음 공격의 피해를 증가시키며 중첩되지 않는 숨고르기 버프를 얻습니다.\n적이 셋 있을때에는 사용하기 어려운 기술이지만 빠르게 적 하나를 정리했다면\n숨고르기를 적극적으로 활용하여 남은적들을 훨씬 쉽게 처리할 수 있습니다.\n숨고르기 : 공격 턴 하나 소모, 다음 가하는 피해 n증가, 중첩 불가"));
		Text_Dialogue->SetText(FText::FromString(sStr));
	}
	else if (page == 6)
	{
		sStr = FString::Printf(TEXT("\n\n\n\n\n이상의 기술을 적극적으로 활용하여 왕국에 평화를 가져다 주세요!"));
		Text_Dialogue->SetText(FText::FromString(sStr));
	}
}

void UUI_Tutorial::OnButtonBackClicked()
{
	if (CheckBox_0->CheckedState == ECheckBoxState::Checked)
	{
		AController_MainCharacter::Tutorial = true;
	}
	else if (CheckBox_0->CheckedState == ECheckBoxState::Unchecked)
	{
		AController_MainCharacter::Tutorial = false;
	}

	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideTutorialWidget();
	}
}

void UUI_Tutorial::OnButtonNextClicked()
{
	thispage += 1;
	if (thispage > 6)
	{
		thispage = 6;
	}
	ShowPage(thispage);
}

void UUI_Tutorial::OnButtonFrontClicked()
{
	thispage -= 1;
	if (thispage < 1)
	{
		thispage = 1;
	}
	ShowPage(thispage);
}
