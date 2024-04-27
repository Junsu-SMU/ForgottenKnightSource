// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_DungeonClear.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_DungeonClear : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();
	UFUNCTION()
		void OnGoClicked();

private:

public:

	virtual void NativeOnInitialized() override;

	void ChangeStat(int EXP, int Gold, int clear);
	void ShowLvUp();

	int ClearNum = 0;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Go;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_EXP;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Gold;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_HP;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Message;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_LvUp;
};
