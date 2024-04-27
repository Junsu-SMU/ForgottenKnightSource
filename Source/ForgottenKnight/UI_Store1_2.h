// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Store1_2.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Store1_2 : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();
	UFUNCTION()
		void OnBehindClicked();
	UFUNCTION()
		void OnFrontClicked();
	UFUNCTION()
		void OnBuySwordClicked();
	UFUNCTION()
		void OnBuySheildClicked();

public:

	virtual void NativeOnInitialized() override;

	void ChangeGold();
	void ChangeEquipStat();

	int SwordPrice = 0;
	int SwordLv = 0;
	int SheildPrice = 0;
	int SheildLv = 0;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Behind;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Front;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuySword;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BuySheild;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Gold;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_SwordPrice;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_SheildPrice;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_SwordLvFront;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_SwordLvBehind;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_SheildLvFront;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_SheildLvBehind;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_SwordNeedLv;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_SheildNeedLv;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_SwordStat;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_SheildStat;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Message;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Notify;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Notify;

};
