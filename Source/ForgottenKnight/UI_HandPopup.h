// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_HandPopup.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_HandPopup : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardWidget")
		TSubclassOf<UUserWidget> CardWidget;

public:

	void SetDeck(TArray<int32> Deck);

	UPROPERTY(meta = (BindWidget))
		class UWrapBox* WrapBox_Card;
	
};
