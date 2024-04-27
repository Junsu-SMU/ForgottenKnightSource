// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_HandPopup.h"
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

void UUI_HandPopup::SetDeck(TArray<int32> Deck)
{
	WrapBox_Card->ClearChildren();

	int CardNum = Deck.Num();

	for (int x = 0; x < CardNum; x++)
	{
		if (CardWidget != nullptr)
		{
			UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
			UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
			UPanelSlot* CardSlot = WrapBox_Card->AddChild(NewCard);
			NewCard->InsertImage(Deck[x]);
			UWrapBoxSlot* BoxSlot = Cast<UWrapBoxSlot>(CardSlot);
			BoxSlot->SetPadding(10);
		}
	}
}
