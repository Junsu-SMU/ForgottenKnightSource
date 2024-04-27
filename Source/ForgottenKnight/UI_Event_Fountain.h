// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Event_Fountain.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Event_Fountain : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();
	UFUNCTION()
		void OnTakeClicked();
	UFUNCTION()
		void OnPopBackClicked();

public:

	virtual void NativeOnInitialized() override;

	void Popup();
	void PopupObjectControl();
	void SetTextTake();

	int EventOrder = 0;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Take;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_PopBack;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Popup;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Value;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Popup;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Message;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Notify;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Notify;
	
};
