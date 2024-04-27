// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ClickDeck.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_ClickDeck : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnButtonBackClicked();
	UFUNCTION()
		void OnButtonYesClicked();
	UFUNCTION()
		void OnButtonNoClicked();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardWidget")
		TSubclassOf<UUserWidget> CardWidget;

public:

	virtual void NativeOnInitialized() override;
	
	void SetMessage(int UseNum, int StoreOrder);
	void ShowPopup(int CardNum, int CardOrder);
	void HidePopup();
	void FixDeck();
	void ChangeGold();

	int MyCardNum = 0;
	int MyUseNum = 0;
	int MyStoreOrder = 0;
	int MyPrice = 0;
	int MyCardOrder = 0;
	int EventOrder = 0;

	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonBack;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_No;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Yes;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* WrapBox_Card;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Message;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Gold;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_PopupMessage;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_PopupGold;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Notify;
	UPROPERTY(meta = (BindWidget))
		class UImage* Background_Popup;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Arrow;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Notify;
	UPROPERTY(meta = (BindWidget))
		class UUI_PlayCard* CurrentCard;
	UPROPERTY(meta = (BindWidget))
		class UUI_PlayCard* ChangeCard;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Message;
	
};
