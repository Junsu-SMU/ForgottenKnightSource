// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Event_3.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Event_3 : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();
	UFUNCTION()
		void OnUpgradeClicked();
	UFUNCTION()
		void OnDeleteClicked();

public:

	virtual void NativeOnInitialized() override;

	void UpdateStat();

	int EventOrder = 0;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Upgrade;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Delete;
	
};
