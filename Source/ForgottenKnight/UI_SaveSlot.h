// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_SaveSlot.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_SaveSlot : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();
	UFUNCTION()
		void OnSlot1Clicked();
	UFUNCTION()
		void OnSlot2Clicked();
	UFUNCTION()
		void OnSlot3Clicked();

public:

	virtual void NativeOnInitialized() override;
	void CalcSlotData(int SlotNum);

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Slot1;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Slot2;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Slot3;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Save1;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Save2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Save3;
	
};
