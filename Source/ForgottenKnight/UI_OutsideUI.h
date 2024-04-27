// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_OutsideUI.generated.h"

/**
 *
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_OutsideUI : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnButtonInventoryClicked();

	UFUNCTION()
		void OnButtonPauseClicked();

public:

	virtual void NativeOnInitialized() override;

	void ChangeStat();

	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonInventory;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Pause;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Name;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Level;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Health;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_EXP;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Gold;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Progressbar_EXP;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Progressbar_Health;

};
