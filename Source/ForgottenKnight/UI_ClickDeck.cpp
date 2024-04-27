// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ClickDeck.h"
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
#include "Animation/WidgetAnimation.h"

void UUI_ClickDeck::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ButtonBack != nullptr)
	{
		ButtonBack->OnClicked.AddDynamic(this, &UUI_ClickDeck::OnButtonBackClicked);
	}
	if (Button_Yes != nullptr)
	{
		Button_Yes->OnClicked.AddDynamic(this, &UUI_ClickDeck::OnButtonYesClicked);
	}
	if (Button_No != nullptr)
	{
		Button_No->OnClicked.AddDynamic(this, &UUI_ClickDeck::OnButtonNoClicked);
	}

	int CardNum = AController_MainCharacter::MyDeck.Num();

	for (int x = 0; x < CardNum; x++)
	{
		if (CardWidget != nullptr)
		{
			UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
			UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
			NewCard->MyOrder = x;
			UPanelSlot* CardSlot = WrapBox_Card->AddChild(NewCard);
			NewCard->InsertImage(AController_MainCharacter::MyDeck[x]);
			UWrapBoxSlot* BoxSlot = Cast<UWrapBoxSlot>(CardSlot);
			BoxSlot->SetPadding(10);
		}
	}
	ChangeGold();
	HidePopup();
}

void UUI_ClickDeck::SetMessage(int UseNum, int StoreOrder)
{
	MyUseNum = UseNum; MyStoreOrder = StoreOrder;
	if (UseNum == 1)
	{
		FString sStr = FString::Printf(TEXT("강화할 카드를 선택하세요"));
		Text_Message->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("그 카드를 강화하시겠습니까?"));
		Text_PopupMessage->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("소모 재화 : 100 G"));
		Text_PopupGold->SetText(FText::FromString(sStr));
		MyPrice = 100;
	}
	else if (UseNum == 2)
	{
		FString sStr = FString::Printf(TEXT("제거할 카드를 선택하세요"));
		Text_Message->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("그 카드를 제거하시겠습니까?"));
		Text_PopupMessage->SetText(FText::FromString(sStr));
		int x = 0;
		if (StoreOrder == 1)
		{
			x = (25 * AController_MainCharacter::StoreDeleteStack[0]) + 50;
		}
		else if (StoreOrder == 2)
		{
			x = (25 * AController_MainCharacter::StoreDeleteStack[1]) + 50;
		}
		else if (StoreOrder == 4)
		{
			x = (25 * AController_MainCharacter::StoreDeleteStack[2]) + 50;
		}
		sStr = FString::Printf(TEXT("소모 재화 : %d G"), x);
		Text_PopupGold->SetText(FText::FromString(sStr));
		MyPrice = x;
	}
	else if (UseNum == 3)
	{
		FString sStr = FString::Printf(TEXT("강화할 카드를 선택하세요"));
		Text_Message->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("그 카드를 강화하시겠습니까?"));
		Text_PopupMessage->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("소모 재화 : 0 G"));
		Text_PopupGold->SetText(FText::FromString(sStr));
		MyPrice = 0;
	}
	else if (UseNum == 4)
	{
		FString sStr = FString::Printf(TEXT("제거할 카드를 선택하세요"));
		Text_Message->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("그 카드를 제거하시겠습니까?"));
		Text_PopupMessage->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("소모 재화 : 0 G"));
		Text_PopupGold->SetText(FText::FromString(sStr));
		MyPrice = 0;
	}
}

void UUI_ClickDeck::ShowPopup(int CardNum, int CardOrder)
{
	if (MyUseNum == 1 || MyUseNum == 3) // 카드 강화 시
	{
		if (CardNum > 100) // 이미 강화된 카드를 눌렀으면
		{
			FString sStr = FString::Printf(TEXT("이미 강화한 카드입니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
			return;
		}
	}
	else if (MyUseNum == 2 || MyUseNum == 4) // 카드 제거 시
	{
		if (AController_MainCharacter::MyDeck.Num() < 11) // 덱 카드가 10장 이하면
		{
			FString sStr = FString::Printf(TEXT("덱에 카드가 10장 이하입니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
			return;
		}
	}

	MyCardNum = CardNum;
	MyCardOrder = CardOrder;
	Background_Popup->SetVisibility(ESlateVisibility::Visible);
	Text_PopupMessage->SetVisibility(ESlateVisibility::Visible);
	Text_PopupGold->SetVisibility(ESlateVisibility::Visible);
	Button_Yes->SetVisibility(ESlateVisibility::Visible);
	Button_No->SetVisibility(ESlateVisibility::Visible);
	CurrentCard->SetVisibility(ESlateVisibility::HitTestInvisible);
	CurrentCard->InsertImage(CardNum);

	if (MyUseNum == 1 || MyUseNum == 3) // 카드 강화 시
	{
		CurrentCard->SetRenderTranslation(FVector2D(0, 0));
		ChangeCard->SetVisibility(ESlateVisibility::HitTestInvisible);
		ChangeCard->InsertImage(CardNum + 100);
		Image_Arrow->SetVisibility(ESlateVisibility::Visible);
	}
	else if (MyUseNum == 2 || MyUseNum == 4) // 카드 제거 시
	{
		CurrentCard->SetRenderTranslation(FVector2D(230.f, 0));
	}
}

void UUI_ClickDeck::HidePopup()
{
	MyCardNum = 0;
	MyCardOrder = 0;
	Background_Popup->SetVisibility(ESlateVisibility::Collapsed);
	Text_PopupMessage->SetVisibility(ESlateVisibility::Collapsed);
	Text_PopupGold->SetVisibility(ESlateVisibility::Collapsed);
	Button_Yes->SetVisibility(ESlateVisibility::Collapsed);
	Button_No->SetVisibility(ESlateVisibility::Collapsed);
	CurrentCard->SetVisibility(ESlateVisibility::Collapsed);
	ChangeCard->SetVisibility(ESlateVisibility::Collapsed);
	Image_Arrow->SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_ClickDeck::FixDeck()
{
	int k = WrapBox_Card->GetChildrenCount();
	for (int x = 0; x < k; x++)
	{
		WrapBox_Card->RemoveChildAt(0);
	}

	int CardNum = AController_MainCharacter::MyDeck.Num();

	for (int x = 0; x < CardNum; x++)
	{
		if (CardWidget != nullptr)
		{
			UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
			UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
			NewCard->MyOrder = x;
			UPanelSlot* CardSlot = WrapBox_Card->AddChild(NewCard);
			NewCard->InsertImage(AController_MainCharacter::MyDeck[x]);
			UWrapBoxSlot* BoxSlot = Cast<UWrapBoxSlot>(CardSlot);
			BoxSlot->SetPadding(10);
		}
	}
}

void UUI_ClickDeck::ChangeGold()
{
	FString NewWString = FString::FromInt(AForgottenKnightCharacter::MyGold);
	FText InWText = FText::FromString(NewWString + " G");
	Text_Gold->SetText(InWText);
}

void UUI_ClickDeck::OnButtonBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideClickDeckWidget();
	}
}

void UUI_ClickDeck::OnButtonYesClicked()
{
	if (AForgottenKnightCharacter::MyGold >= MyPrice)
	{
		if (MyUseNum == 1) // 강화 기능
		{
			if (AController_MainCharacter::MyDeck[MyCardOrder] <= 100)
			{
				AForgottenKnightCharacter::MyGold -= MyPrice;
				AController_MainCharacter::MyDeck[MyCardOrder] += 100; // 카드 강화는 +100

				AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
				PlayerController->StoreChangeGold();
			}
		}
		else if (MyUseNum == 2) // 제거 기능
		{
			if (AController_MainCharacter::MyDeck.Num() >= 11) // 덱 카드가 11장 이상이어야 카드 제거 가능
			{
				AForgottenKnightCharacter::MyGold -= MyPrice;
				AController_MainCharacter::MyDeck.RemoveAt(MyCardOrder);
				if (MyStoreOrder < 3)
				{
					AController_MainCharacter::StoreDeleteStack[MyStoreOrder - 1] += 1; // 스택 올리기
				}
				else
				{
					AController_MainCharacter::StoreDeleteStack[MyStoreOrder - 2] += 1; // 스택 올리기
				}
				SetMessage(2, MyStoreOrder);
				AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
				PlayerController->StoreChangeGold();
			}
		}
		else if (MyUseNum == 3) // 무료 강화 기능
		{
			if (AController_MainCharacter::MyDeck[MyCardOrder] <= 100 && AController_MainCharacter::Event3Data[EventOrder] > 0)
			{
				AController_MainCharacter::MyDeck[MyCardOrder] += 100; // 카드 강화는 +100
				AController_MainCharacter::Event3Data[EventOrder] -= 1;

				AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
				if (PlayerController != nullptr)
				{
					PlayerController->StoreChangeGold();
					PlayerController->HideClickDeckWidget();
				}
			}
		}
		else if (MyUseNum == 4) // 무료 제거 기능
		{
			if (AController_MainCharacter::MyDeck.Num() >= 11 && AController_MainCharacter::Event3Data[EventOrder] > 0) // 덱 카드가 11장 이상이어야 카드 제거 가능
			{
				AController_MainCharacter::MyDeck.RemoveAt(MyCardOrder);
				AController_MainCharacter::Event3Data[EventOrder] -= 1;

				AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
				if (PlayerController != nullptr)
				{
					PlayerController->StoreChangeGold();
					PlayerController->HideClickDeckWidget();
				}
				
			}
		}
	}
	else
	{
		FString sStr = FString::Printf(TEXT("골드가 부족합니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}
	ChangeGold();
	FixDeck();
	HidePopup();
}

void UUI_ClickDeck::OnButtonNoClicked()
{
	HidePopup();
}
