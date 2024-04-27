// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EnterStage.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "UI_MobPopup.h"

void UUI_EnterStage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Back != nullptr)
	{
		Button_Back->OnClicked.AddDynamic(this, &UUI_EnterStage::OnBackClicked);
	}
	if (Button_Enter != nullptr)
	{
		Button_Enter->OnClicked.AddDynamic(this, &UUI_EnterStage::OnEnterClicked);
	}

	MyChapter = 0;
	MyStage = 0;
}

void UUI_EnterStage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Image_Mob1->IsHovered())
	{

		PopupUI->SetVisibility(ESlateVisibility::Visible);
		PopupUI->SetMessage(Mob1);
	}
	else if (Image_Mob2->IsHovered())
	{
		PopupUI->SetVisibility(ESlateVisibility::Visible);
		PopupUI->SetMessage(Mob2);
	}
	else if (Image_Mob3->IsHovered())
	{
		PopupUI->SetVisibility(ESlateVisibility::Visible);
		PopupUI->SetMessage(Mob3);
	}
	else
	{
		PopupUI->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void UUI_EnterStage::ChangeStat(int Chapter, int Stage)
{
	MyChapter = Chapter;
	MyStage = Stage;

	if (Chapter == 1)
	{
		if (Stage == 1)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgSlime);
			Image_Mob2->SetBrush(ImgSlime);
			Image_Mob3->SetBrush(ImgSlime);
			Mob1 = ("Slime");
			Mob2 = ("Slime");
			Mob3 = ("Slime");

			if (AController_MainCharacter::MyProgress[1] == false)
			{
				NewWString = FString::FromInt(15);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(6);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(3);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 2)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgGoblin);
			Image_Mob2->SetBrush(ImgSlime);
			Image_Mob3->SetBrush(ImgGoblin);
			Mob1 = ("Goblin");
			Mob2 = ("Slime");
			Mob3 = ("Goblin");

			if (AController_MainCharacter::MyProgress[2] == false)
			{
				NewWString = FString::FromInt(19);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(10);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(4);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 3)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgSlime);
			Image_Mob2->SetBrush(ImgGoblin);
			Image_Mob3->SetBrush(ImgTroll);
			Mob1 = ("Slime");
			Mob2 = ("Goblin");
			Mob3 = ("Troll");

			if (AController_MainCharacter::MyProgress[3] == false)
			{
				NewWString = FString::FromInt(24);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(12);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(5);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 4)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgSpider);
			Image_Mob2->SetBrush(ImgGoblin);
			Image_Mob3->SetBrush(ImgSpider);
			Mob1 = ("Spider");
			Mob2 = ("Goblin");
			Mob3 = ("Spider");

			if (AController_MainCharacter::MyProgress[4] == false)
			{
				NewWString = FString::FromInt(27);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(16);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(5);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 5)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Collapsed);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Collapsed);
			Image_Mob2->SetBrush(ImgAsura);
			Mob2 = ("Asura");

			if (AController_MainCharacter::MyProgress[5] == false)
			{
				NewWString = FString::FromInt(50);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(30);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);

				//Button_Enter->SetIsEnabled(false);
			}
		}
		else if (Stage == 6)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgGolem);
			Image_Mob2->SetBrush(ImgTroll);
			Image_Mob3->SetBrush(ImgGolem);
			Mob1 = ("Golem");
			Mob2 = ("Troll");
			Mob3 = ("Golem");

			if (AController_MainCharacter::MyProgress[6] == false)
			{
				NewWString = FString::FromInt(32);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(36);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(6);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 7)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgShadowSpear);
			Image_Mob2->SetBrush(ImgShadowSheild);
			Image_Mob3->SetBrush(ImgShadowAxe);
			Mob1 = ("ShadowSpear");
			Mob2 = ("ShadowSheild");
			Mob3 = ("ShadowAxe");

			if (AController_MainCharacter::MyProgress[7] == false)
			{
				NewWString = FString::FromInt(45);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(30);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(9);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 8)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgGriffon);
			Image_Mob2->SetBrush(ImgShadowAxe);
			Image_Mob3->SetBrush(ImgGolem);
			Mob1 = ("Griffon");
			Mob2 = ("ShadowAxe");
			Mob3 = ("Golem");

			if (AController_MainCharacter::MyProgress[8] == false)
			{
				NewWString = FString::FromInt(55);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(40);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(11);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 9)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgGriffon);
			Image_Mob2->SetBrush(ImgRafflesia);
			Image_Mob3->SetBrush(ImgGriffon);
			Mob1 = ("Griffon");
			Mob2 = ("Rafflesia");
			Mob3 = ("Griffon");

			if (AController_MainCharacter::MyProgress[9] == false)
			{
				NewWString = FString::FromInt(85);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(42);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(17);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 10)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Collapsed);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Collapsed);
			Image_Mob2->SetBrush(ImgWarlock);
			Mob2 = ("Warlock");

			if (AController_MainCharacter::MyProgress[10] == false)
			{
				NewWString = FString::FromInt(150);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(50);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);

				//Button_Enter->SetIsEnabled(false);
			}
		}
	}
	if (Chapter == 2)
	{
		if (Stage == 1)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgSkeletonArcher);
			Image_Mob2->SetBrush(ImgSkeletonSoldier);
			Image_Mob3->SetBrush(ImgSkeletonArcher);
			Mob1 = ("SkeletonArcher");
			Mob2 = ("SkeletonSoldier");
			Mob3 = ("SkeletonArcher");

			if (AController_MainCharacter::MyProgress[11] == false)
			{
				NewWString = FString::FromInt(120);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(75);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(24);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 2)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgSkeletonSoldier);
			Image_Mob2->SetBrush(ImgSkeletonArcher);
			Image_Mob3->SetBrush(ImgSkeletonGuardDog);
			Mob1 = ("SkeletonSoldier");
			Mob2 = ("SkeletonArcher");
			Mob3 = ("SkeletonGuardDog");

			if (AController_MainCharacter::MyProgress[12] == false)
			{
				NewWString = FString::FromInt(120);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(75);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(24);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 3)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgSkeletonGuardDog);
			Image_Mob2->SetBrush(ImgSkeletonGuardDog);
			Image_Mob3->SetBrush(ImgSkeletonGuardDog);
			Mob1 = ("SkeletonGuardDog");
			Mob2 = ("SkeletonGuardDog");
			Mob3 = ("SkeletonGuardDog");

			if (AController_MainCharacter::MyProgress[13] == false)
			{
				NewWString = FString::FromInt(120);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(75);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(24);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 4)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgSkeletonSoldier);
			Image_Mob2->SetBrush(ImgLich);
			Image_Mob3->SetBrush(ImgSkeletonSoldier);
			Mob1 = ("SkeletonSoldier");
			Mob2 = ("Lich");
			Mob3 = ("SkeletonSoldier");

			if (AController_MainCharacter::MyProgress[14] == false)
			{
				NewWString = FString::FromInt(180);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(100);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(36);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 5)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgSkeletonArcher);
			Image_Mob2->SetBrush(ImgLich);
			Image_Mob3->SetBrush(ImgSkeletonGuardDog);
			Mob1 = ("SkeletonArcher");
			Mob2 = ("Lich");
			Mob3 = ("SkeletonGuardDog");

			if (AController_MainCharacter::MyProgress[15] == false)
			{
				NewWString = FString::FromInt(280);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(150);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(56);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);

				Button_Enter->SetIsEnabled(false);
			}
		}
		else if (Stage == 6)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgFallenFairy);
			Image_Mob2->SetBrush(ImgFallenFairy);
			Image_Mob3->SetBrush(ImgFallenFairy);
			Mob1 = ("FallenFairy");
			Mob2 = ("FallenFairy");
			Mob3 = ("FallenFairy");

			if (AController_MainCharacter::MyProgress[16] == false)
			{
				NewWString = FString::FromInt(450);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(150);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(90);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 7)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgBeholder);
			Image_Mob2->SetBrush(ImgFallenFairy);
			Image_Mob3->SetBrush(ImgBeholder);
			Mob1 = ("Beholder");
			Mob2 = ("FallenFairy");
			Mob3 = ("Beholder");

			if (AController_MainCharacter::MyProgress[17] == false)
			{
				NewWString = FString::FromInt(410);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(190);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(82);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 8)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgBeholder);
			Image_Mob2->SetBrush(ImgBaphomet);
			Image_Mob3->SetBrush(ImgFallenFairy);
			Mob1 = ("Beholder");
			Mob2 = ("Baphomet");
			Mob3 = ("FallenFairy");

			if (AController_MainCharacter::MyProgress[18] == false)
			{
				NewWString = FString::FromInt(530);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(200);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(106);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 9)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Visible);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Visible);
			Image_Mob1->SetBrush(ImgBaphomet);
			Image_Mob2->SetBrush(ImgDurahan);
			Image_Mob3->SetBrush(ImgBaphomet);
			Mob1 = ("Baphomet");
			Mob2 = ("Durahan");
			Mob3 = ("Baphomet");

			if (AController_MainCharacter::MyProgress[19] == false)
			{
				NewWString = FString::FromInt(800);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(260);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(160);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
		else if (Stage == 10)
		{
			FString NewWString = FString::FromInt(Chapter);
			FString NewWString2 = FString::FromInt(Stage);
			FText InWText = FText::FromString("Stage " + NewWString + " - " + NewWString2);
			Text_Stage->SetText(InWText);
			Image_Mob1->SetVisibility(ESlateVisibility::Collapsed);
			Image_Mob2->SetVisibility(ESlateVisibility::Visible);
			Image_Mob3->SetVisibility(ESlateVisibility::Collapsed);
			Image_Mob2->SetBrush(ImgKing);
			Mob2 = ("King");

			if (AController_MainCharacter::MyProgress[20] == false)
			{
				NewWString = FString::FromInt(1000);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(500);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("X");
				Text_Clear->SetText(InWText);
			}
			else
			{
				NewWString = FString::FromInt(200);
				InWText = FText::FromString(NewWString + " G");
				Text_Gold->SetText(InWText);

				NewWString = FString::FromInt(0);
				InWText = FText::FromString(NewWString);
				Text_EXP->SetText(InWText);

				InWText = FText::FromString("O");
				Text_Clear->SetText(InWText);
			}
		}
	}
}

void UUI_EnterStage::OnEnterClicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyChapter == 1)
	{
		AForgottenKnightCharacter::MyStartTransform = MyPlayer->GetTransform();
	}
	else if (MyChapter == 2)
	{
		AForgottenKnightCharacter::MyStartTransform2 = MyPlayer->GetTransform();
	}

	if (MyChapter == 1)
	{
		if (MyStage == 1)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage1_1"));
		}
		else if (MyStage == 2)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage1_2"));
		}
		else if (MyStage == 3)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage1_3"));
		}
		else if (MyStage == 4)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage1_4"));
		}
		else if (MyStage == 5)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage1_5"));
		}
		else if (MyStage == 6)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage1_6"));
		}
		else if (MyStage == 7)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage1_7"));
		}
		else if (MyStage == 8)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage1_8"));
		}
		else if (MyStage == 9)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage1_9"));
		}
		else if (MyStage == 10)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage1_10"));
		}
	}
	else if (MyChapter == 2)
	{
		if (MyStage == 1)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage2_1"));
		}
		else if (MyStage == 2)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage2_2"));
		}
		else if (MyStage == 3)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage2_3"));
		}
		else if (MyStage == 4)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage2_4"));
		}
		else if (MyStage == 5)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage2_5"));
		}
		else if (MyStage == 6)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage2_6"));
		}
		else if (MyStage == 7)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage2_7"));
		}
		else if (MyStage == 8)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage2_8"));
		}
		else if (MyStage == 9)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage2_9"));
		}
		else if (MyStage == 10)
		{
			UGameplayStatics::OpenLevel(this, FName("Stage2_10"));
		}
	}
}

void UUI_EnterStage::OnBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideEnterStageWidget();
	}
}
