// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Store1_3.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Store1_3 : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();
	UFUNCTION()
		void OnFrontClicked();
	UFUNCTION()
		void OnBuyUpgradeClicked();
	UFUNCTION()
		void OnBuyDeleteClicked();

private:

public:

	virtual void NativeOnInitialized() override;

	void ChangeGold();
	void PlayMessage(int Order);

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Front;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Upgrade;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Delete;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_UpgradePrice;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_DeletePrice;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Gold;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard1;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard2;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard3;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard4;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard5;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard6;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard7;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard8;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard9;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard10;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard11;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard12;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard13;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard14;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard15;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard16;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard17;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard18;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard19;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard20;

	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Notify;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Notify;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Message;

};
