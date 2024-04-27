// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_StageDie.generated.h"

/**
 *
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_StageDie : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();

	UFUNCTION()
		void OnReplayClicked();

private:

public:

	virtual void NativeOnInitialized() override;

	int NewHP = 0;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Replay;

};
