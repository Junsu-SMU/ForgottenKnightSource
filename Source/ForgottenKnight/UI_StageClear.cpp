// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_StageClear.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "Animation/WidgetAnimation.h"

void UUI_StageClear::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_StageClear::OnBackClicked);
	}

}

void UUI_StageClear::ChangeStat(int EXP, int Gold)
{
	FString NewWString = FString::FromInt(EXP);
	FText InWText = FText::FromString(NewWString);
	Text_EXP->SetText(InWText);

	NewWString = FString::FromInt(Gold);
	InWText = FText::FromString(NewWString);
	Text_Gold->SetText(InWText);
}

void UUI_StageClear::OnBackClicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer != nullptr)
	{
		AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (PlayerController != nullptr)
		{
			PlayerController->HideStageClearWidget();

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

void UUI_StageClear::ShowLvUp()
{
	PlayAnimation(Anim_LvUp);
}
