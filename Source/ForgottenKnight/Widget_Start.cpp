// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Start.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "MySaveGame.h"

void UWidget_Start::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Start != nullptr)
	{
		Button_Start->OnClicked.AddDynamic(this, &UWidget_Start::OnStartClicked);
	}

	if (Button_Load != nullptr)
	{
		Button_Load->OnClicked.AddDynamic(this, &UWidget_Start::OnLoadClicked);
	}

	if (Button_Quit != nullptr)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UWidget_Start::OnQuitClicked);
	}
	int k = 0;
	UMySaveGame* NewPlayerData = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	for (int i = 1; i < 4; i++)
	{
		NewPlayerData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot((TEXT("%s"), FString::FromInt(i)), 0));
		if (NewPlayerData != nullptr)
		{
			k += 1;
		}
	}

	if (k == 0)
	{
		Button_Load->SetIsEnabled(false);
	}

}

void UWidget_Start::OnStartClicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer != nullptr)
	{
		MyPlayer->LoadNewGame();
	}
}

void UWidget_Start::OnLoadClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowLoadSlotWidget();
	}
}

void UWidget_Start::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this,
		nullptr,
		EQuitPreference::Quit,
		true);
}
