// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_EnterStage.generated.h"

/**
 *
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_EnterStage : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnBackClicked();
	UFUNCTION()
		void OnEnterClicked();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:

public:

	virtual void NativeOnInitialized() override;

	void ChangeStat(int Chapter, int Stage);

	int MyChapter = 0;
	int MyStage = 0;
	FString Mob1 = { "" };
	FString Mob2 = { "" };
	FString Mob3 = { "" };

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Back;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Enter;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Gold;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_EXP;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Clear;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Stage;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Mob1;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Mob2;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Mob3;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSlime; // 슬라임
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgGoblin;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgTroll;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSpider;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgMantis;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgGolem;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgAsura;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgShadowSpear;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgShadowSheild;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgShadowAxe;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgGriffon;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgRafflesia;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgWarlock;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSkeletonSoldier;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSkeletonArcher;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSkeletonGuardDog;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgLich;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBeholder;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgFallenFairy;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBaphomet;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgDurahan;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgKing;

	UPROPERTY(meta = (BindWidget))
		class UUI_MobPopup* PopupUI;

};
