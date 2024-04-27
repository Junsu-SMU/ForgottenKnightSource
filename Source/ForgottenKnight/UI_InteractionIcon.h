// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_InteractionIcon.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_InteractionIcon : public UUserWidget
{
	GENERATED_BODY()

protected:
	
public:
	void PlayOn();
	void PlayOff();

	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Icon;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_On;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Off;

};
