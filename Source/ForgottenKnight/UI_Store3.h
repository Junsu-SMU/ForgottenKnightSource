// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Store3.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Store3 : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();
	UFUNCTION()
		void OnBuy1Clicked();
	UFUNCTION()
		void OnBuy2Clicked();
	UFUNCTION()
		void OnBuy3Clicked();
	UFUNCTION()
		void OnBuy4Clicked();
	UFUNCTION()
		void OnBuy5Clicked();
	UFUNCTION()
		void OnFrontClicked();
	UFUNCTION()
		void OnBehindClicked();
	UFUNCTION()
		void OnYesClicked();
	UFUNCTION()
		void OnNoClicked();

private:

	void ChangeGold();
	void BuyItem(int itemtype, int itemindex, int price);
	void ShowPopup(bool Swit);
	void ChangePage(); // 화면 전환

	int page = 1; // 화면 페이지
	int myitemtype = 0; // 구매할 아이템 타입
	int itemnum = 0; // 구매할 아이템 인덱스

public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_0;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_1;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_2;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_3;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_4;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_5;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemName_0;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemName_1;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemName_2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemName_3;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemName_4;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemName_5;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextMain_0;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextMain_1;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextMain_2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextMain_3;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextMain_4;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextMain_5;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextGold_1;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextGold_2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextGold_3;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextGold_4;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextGold_5;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Buy1;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Buy2;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Buy3;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Buy4;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Buy5;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Gold;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Front;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Behind;

	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Popup;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_Popup;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_PopupYes;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_PopupNo;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Message;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Notify;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Notify;

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
