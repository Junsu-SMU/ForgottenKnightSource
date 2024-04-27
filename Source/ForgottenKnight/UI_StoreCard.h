// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_StoreCard.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_StoreCard : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBuyClicked();

public:

	virtual void NativeOnInitialized() override;

	void SetCard(int CardNum, int Price, int IndexNum, int CardLevel);
	void ChangeStat();

	int MyPrice = 0;
	int MyCardNum = 0;
	int MyIndexNum = 0;
	int MyStoreOrder = 0;
	int MyCardLevel = 0;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Buy;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Buy;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Price;
	UPROPERTY(meta = (BindWidget))
		class UUI_PlayCard* CardImage;
	
};
