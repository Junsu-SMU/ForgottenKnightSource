// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Inventory_Cards.h"
#include "UI_PlayCard.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Styling/SlateBrush.h"
#include "Controller_MainCharacter.h"
#include "ForgottenKnightCharacter.h"

void UUI_Inventory_Cards::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ButtonBack != nullptr)
	{
		ButtonBack->OnClicked.AddDynamic(this, &UUI_Inventory_Cards::OnButtonBackClicked);
	}

	int CardNum = AController_MainCharacter::MyDeck.Num();

	for (int x = 0; x < CardNum; x++)
	{
		if (CardWidget != nullptr)
		{
			UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
			UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
			UPanelSlot* CardSlot = WrapBox_Card->AddChild(NewCard);
			NewCard->InsertImage(AController_MainCharacter::MyDeck[x]);
			UWrapBoxSlot* BoxSlot = Cast<UWrapBoxSlot>(CardSlot);
			BoxSlot->SetPadding(10);
		}
	}

}

void UUI_Inventory_Cards::OnButtonBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideInventoryCardsWidget();
	}
}
