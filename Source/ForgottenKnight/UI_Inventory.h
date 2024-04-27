// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Inventory.generated.h"

/**
 *
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Inventory : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnButtonCardsClicked();

	UFUNCTION()
		void OnButtonEquipmentClicked();

	UFUNCTION()
		void OnButtonBackClicked();

public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonCards;

	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonEquipment;

	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonBack;


};
