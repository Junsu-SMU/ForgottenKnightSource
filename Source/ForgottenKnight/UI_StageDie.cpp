// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_StageDie.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"

void UUI_StageDie::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_StageDie::OnBackClicked);
	}

	if (Button_Replay != nullptr)
	{
		Button_Replay->OnClicked.AddDynamic(this, &UUI_StageDie::OnReplayClicked);
	}

}

void UUI_StageDie::OnBackClicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer != nullptr)
	{
		AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (PlayerController != nullptr)
		{
			PlayerController->HideStageDieWidget();
			AForgottenKnightCharacter::MyCurrentHealth = NewHP;
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

void UUI_StageDie::OnReplayClicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer != nullptr)
	{
		AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (PlayerController != nullptr)
		{
			PlayerController->HideStageDieWidget();
			AForgottenKnightCharacter::MyCurrentHealth = NewHP;
			UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
		}
	}
}
