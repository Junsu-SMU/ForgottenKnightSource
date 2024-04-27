// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Equipment.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Equipment : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
		void OnButtonBackClicked();

public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonBack;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Sword;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Sheild;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Passive;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Ring;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Necklace;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Bracelet;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_SwordLv;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_SheildLv;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_PassiveLv;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Message;

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Necklace1;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Necklace2;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Necklace3;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Necklace4;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Necklace5;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Ring1;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Ring2;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Ring3;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Ring4;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Ring5;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Bracelet1;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Bracelet2;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Bracelet3;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Bracelet4;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Bracelet5;
	
};
