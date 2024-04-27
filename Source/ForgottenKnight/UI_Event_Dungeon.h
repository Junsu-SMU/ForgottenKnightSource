// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Event_Dungeon.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Event_Dungeon : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();
	UFUNCTION()
		void OnEnterClicked();

private:

public:

	virtual void NativeOnInitialized() override;

	void ChangeStat(int Stage);

	int MyStage = 0;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Enter;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Gold;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_EXP;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Clear;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Heal;
	
};
