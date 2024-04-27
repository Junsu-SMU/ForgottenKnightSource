// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Store4_3.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Store4_3 : public UUserWidget
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
		class UUI_StoreCard* BuyCard21;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard22;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard23;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard24;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard25;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard26;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard27;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard28;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard29;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard30;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard31;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard32;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard33;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard34;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard35;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard36;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard37;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard38;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard39;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard40;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard41;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard42;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard43;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard44;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard45;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard46;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard47;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard48;
	UPROPERTY(meta = (BindWidget))
		class UUI_StoreCard* BuyCard49;

	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Notify;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Notify;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Message;
	
};
