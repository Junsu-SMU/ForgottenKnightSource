// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Tutorial.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Tutorial : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnButtonBackClicked();
	UFUNCTION()
		void OnButtonFrontClicked();
	UFUNCTION()
		void OnButtonNextClicked();

public:

	virtual void NativeOnInitialized() override;

	void ShowPage(int page);

	int thispage = 1;

	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonBack;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Next;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Front;
	UPROPERTY(meta = (BindWidget))
		class UCheckBox* CheckBox_0;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Page;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Dialogue;
	
};
