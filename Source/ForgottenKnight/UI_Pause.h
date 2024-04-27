// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Pause.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Pause : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnButtonResumeClicked();

	UFUNCTION()
		void OnButtonSaveClicked();
	
	UFUNCTION()
		void OnButtonHelpClicked();

	UFUNCTION()
		void OnButtonQuitClicked();

	UFUNCTION()
		void OnButtonMainClicked();

	UFUNCTION()
		void OnButtonPopupClicked();

	UFUNCTION()
		void OnButtonTutorialClicked();

	UFUNCTION()
		void OnButtonCheatClicked(); // 치트 나중에 삭제

public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Resume;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Save;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Help;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Main;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Quit;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Popup;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Tutorial;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Popup;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Popup;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Cheat; // 치트 나중에 삭제
};
