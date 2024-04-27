// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_MobPopup.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_MobPopup : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetMessage(FString MobName);

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Name;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Message;
	
};
