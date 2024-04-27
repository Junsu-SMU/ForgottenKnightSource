// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_StoreCard.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "UI_PlayCard.h"

void UUI_StoreCard::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Buy != nullptr)
	{
		Button_Buy->OnClicked.AddDynamic(this, &UUI_StoreCard::OnBuyClicked);
	}

}

void UUI_StoreCard::OnBuyClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		if (PlayerController->StoreOrderNum == 1)
		{
			if (AForgottenKnightCharacter::MyGold >= MyPrice) // 골드 체크
			{
				if (AController_MainCharacter::Store1_3Count[MyIndexNum - 1] || MyCardLevel < 2) // 5, 10레벨 카드 구매 여부 확인
				{
					if (MyCardLevel <= AForgottenKnightCharacter::MyLevel) // 레벨 체크
					{
						AForgottenKnightCharacter::MyGold -= MyPrice;
						AController_MainCharacter::MyDeck.Add(MyCardNum);
						AController_MainCharacter::Store1_3Count[MyIndexNum - 1] = false;
						PlayerController->StoreChangeGold();
						ChangeStat();
					}
					else
					{
						PlayerController->PlayUIMessage(13, 3);
					}
				}
				else
				{
					PlayerController->PlayUIMessage(13, 2);
				}
			}
			else
			{
				PlayerController->PlayUIMessage(13, 1);
			}
		}
		else if (PlayerController->StoreOrderNum == 2)
		{
			if (AForgottenKnightCharacter::MyGold >= MyPrice)
			{
				if (AController_MainCharacter::Store2_3Count[MyIndexNum - 1] || MyCardLevel < 2)
				{
					if (MyCardLevel <= AForgottenKnightCharacter::MyLevel) // 레벨 체크
					{
						AForgottenKnightCharacter::MyGold -= MyPrice;
						AController_MainCharacter::MyDeck.Add(MyCardNum);
						AController_MainCharacter::Store2_3Count[MyIndexNum - 1] = false;
						PlayerController->StoreChangeGold();
						ChangeStat();
					}
					else
					{
						PlayerController->PlayUIMessage(23, 3);
					}
				}
				else
				{
					PlayerController->PlayUIMessage(23, 2);
				}
			}
			else
			{
				PlayerController->PlayUIMessage(23, 1);
			}
		}
		else if (PlayerController->StoreOrderNum == 4)
		{
			if (AForgottenKnightCharacter::MyGold >= MyPrice)
			{
				if (AController_MainCharacter::Store4_3Count[MyIndexNum - 1] || MyCardLevel < 2)
				{
					if (MyCardLevel <= AForgottenKnightCharacter::MyLevel) // 레벨 체크
					{
						AForgottenKnightCharacter::MyGold -= MyPrice;
						AController_MainCharacter::MyDeck.Add(MyCardNum);
						AController_MainCharacter::Store4_3Count[MyIndexNum - 1] = false;
						PlayerController->StoreChangeGold();
						ChangeStat();
					}
					else
					{
						PlayerController->PlayUIMessage(43, 3);
					}
				}
				else
				{
					PlayerController->PlayUIMessage(43, 2);
				}
			}
			else
			{
				PlayerController->PlayUIMessage(43, 1);
			}
		}
	}

	
}

void UUI_StoreCard::SetCard(int CardNum, int Price, int IndexNum, int CardLevel)
{
	if (CardImage != nullptr)
	{
		CardImage->InsertImage(CardNum);
	}
	MyPrice = Price;
	MyCardNum = CardNum;
	MyIndexNum = IndexNum;
	MyCardLevel = CardLevel;

	FString NewWString = FString::FromInt(Price);
	FText InWText = FText::FromString(NewWString + " G");
	Text_Price->SetText(InWText);

	ChangeStat();
}

void UUI_StoreCard::ChangeStat()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController->StoreOrderNum == 1)
	{
		if (MyCardLevel > AForgottenKnightCharacter::MyLevel)
		{
			FString sStr = FString::Printf(TEXT("구매 불가"));
			Text_Buy->SetText(FText::FromString(sStr));
			Button_Buy->SetIsEnabled(false);
		}
		else if (AController_MainCharacter::Store1_3Count[MyIndexNum - 1])
		{
			FString sStr = FString::Printf(TEXT("구매"));
			Text_Buy->SetText(FText::FromString(sStr));
			Button_Buy->SetIsEnabled(true);
		}
		else
		{
			FString sStr = FString::Printf(TEXT("매진"));
			Text_Buy->SetText(FText::FromString(sStr));
			Button_Buy->SetIsEnabled(false);
		}
	}
	else if (PlayerController->StoreOrderNum == 2)
	{
		if (MyCardLevel > AForgottenKnightCharacter::MyLevel)
		{
			FString sStr = FString::Printf(TEXT("구매 불가"));
			Text_Buy->SetText(FText::FromString(sStr));
			Button_Buy->SetIsEnabled(false);
		}
		else if (AController_MainCharacter::Store2_3Count[MyIndexNum - 1])
		{
			FString sStr = FString::Printf(TEXT("구매"));
			Text_Buy->SetText(FText::FromString(sStr));
			Button_Buy->SetIsEnabled(true);
		}
		else
		{
			FString sStr = FString::Printf(TEXT("매진"));
			Text_Buy->SetText(FText::FromString(sStr));
			Button_Buy->SetIsEnabled(false);
		}
	}
	else if (PlayerController->StoreOrderNum == 4)
	{
		if (MyCardLevel > AForgottenKnightCharacter::MyLevel)
		{
			FString sStr = FString::Printf(TEXT("구매 불가"));
			Text_Buy->SetText(FText::FromString(sStr));
			Button_Buy->SetIsEnabled(false);
		}
		else if (AController_MainCharacter::Store4_3Count[MyIndexNum - 1])
		{
			FString sStr = FString::Printf(TEXT("구매"));
			Text_Buy->SetText(FText::FromString(sStr));
			Button_Buy->SetIsEnabled(true);
		}
		else
		{
			FString sStr = FString::Printf(TEXT("매진"));
			Text_Buy->SetText(FText::FromString(sStr));
			Button_Buy->SetIsEnabled(false);
		}
	}
}
