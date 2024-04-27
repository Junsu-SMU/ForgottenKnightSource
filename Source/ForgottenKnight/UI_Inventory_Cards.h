// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Inventory_Cards.generated.h"

/**
 *
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Inventory_Cards : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnButtonBackClicked();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardWidget")
		TSubclassOf<UUserWidget> CardWidget;

public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonBack;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* WrapBox_Card;

};
