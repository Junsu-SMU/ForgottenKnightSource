// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Store4.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Store4 : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();
	UFUNCTION()
		void OnBehindClicked();
	UFUNCTION()
		void OnBuyHealClicked();
	UFUNCTION()
		void OnBuyHeal2Clicked();
	UFUNCTION()
		void OnBuyHeal3Clicked();
	UFUNCTION()
		void OnBuyMaxHealClicked();
	UFUNCTION()
		void OnBuyMaxHeal2Clicked();
	UFUNCTION()
		void OnBuyMaxHeal3Clicked();
	UFUNCTION()
		void OnBuySpecialClicked();
	UFUNCTION()
		void OnBuySpecial2Clicked();
	UFUNCTION()
		void OnBuySpecial3Clicked();
	UFUNCTION()
		void OnBuySpecial4Clicked();
	UFUNCTION()
		void OnBuySpecial5Clicked();
	UFUNCTION()
		void OnBuySpecial6Clicked();

private:

	void ChangeGold();
	void BuyHeal(int price, int count, int value);
	void BuyMaxHeal(int price, int count, int value);

public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Behind;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuyHeal;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuyHeal2;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuyHeal3;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuyMaxHeal;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuyMaxHeal2;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuyMaxHeal3;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuySpecial;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuySpecial2;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuySpecial3;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuySpecial4;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuySpecial5;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuySpecial6;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Gold;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Count0;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Count1;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Count2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Count3;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Count4;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Count5;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Count6;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Count7;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Count8;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Count9;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Count10;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Count11;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Message;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Notify;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Notify;
	
};
