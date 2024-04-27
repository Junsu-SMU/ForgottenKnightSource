// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_StageClear.generated.h"

/**
 *
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_StageClear : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FString OutsideMap;

public:

	virtual void NativeOnInitialized() override;

	void ChangeStat(int EXP, int Gold);
	void ShowLvUp();

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_EXP;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Gold;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_LvUp;

};
