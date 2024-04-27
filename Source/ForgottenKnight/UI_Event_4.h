// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Event_4.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Event_4 : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();
	UFUNCTION()
		void OnUpgradeClicked();

public:

	virtual void NativeOnInitialized() override;

	void UpdateStat();

	int EventOrder = 0;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Upgrade;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Stage;

};
