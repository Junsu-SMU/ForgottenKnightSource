// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayWindow.h"
#include "UI_PlayCard.h"
#include "UI_StageClear.h"
#include "UI_Inventory_Cards.h"
#include "DragWidget.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "EnemyCharacter.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"
#include "Components/ProgressBar.h"
#include "Components/PanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SlateWrapperTypes.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "iostream"
#include "random"
#include "MyHealthBar.h"
#include "EnemyHealthBar.h"

std::random_device rng1;
std::uniform_int_distribution<int> dist1(1, 100); // [1, 100] 범위
std::uniform_int_distribution<int> dist2(0, 2); // [0, 2] 범위
std::uniform_int_distribution<int> dist3(0, 1); // [0, 1] 범위
std::uniform_int_distribution<int> distDelirium(0, 3); // [1, 100] 범위

void UUI_PlayWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_End != nullptr)
	{
		Button_End->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonEndClicked);
	}
	if (Button_First != nullptr)
	{
		Button_First->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonFirstClicked);
	}
	if (Button_Second != nullptr)
	{
		Button_Second->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonSecondClicked);
	}
	if (Button_Third != nullptr)
	{
		Button_Third->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonThirdClicked);
	}
	if (Button_Breath != nullptr)
	{
		Button_Breath->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonBreathClicked);
	}
	if (Button_Deck != nullptr)
	{
		Button_Deck->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonDeckClicked);
	}
	if (Button_Pause != nullptr)
	{
		Button_Pause->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonPauseClicked);
	}
	if (Button_DrawDeck != nullptr)
	{
		Button_DrawDeck->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonDrawDeckClicked);
	}
	if (Button_DumpDeck != nullptr)
	{
		Button_DumpDeck->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonDumpDeckClicked);
	}
	if (Button_PopupYes != nullptr)
	{
		Button_PopupYes->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonPopupYesClicked);
	}
	if (Button_PopupNo != nullptr)
	{
		Button_PopupNo->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonPopupNoClicked);
	}
	if (Button_FrontHand != nullptr)
	{
		Button_FrontHand->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonFrontHandClicked);
	}
	if (Button_BehindHand != nullptr)
	{
		Button_BehindHand->OnClicked.AddDynamic(this, &UUI_PlayWindow::OnButtonBehindHandClicked);
	}

	TurnStartDelegate.BindDynamic(this, &UUI_PlayWindow::TurnStartEnded);
	TurnEndDelegate.BindDynamic(this, &UUI_PlayWindow::TurnEndEnded);
	BloodSetDelegate.BindDynamic(this, &UUI_PlayWindow::BloodSetEnded);

	BindToAnimationFinished(Anim_TurnStart, TurnStartDelegate);
	BindToAnimationFinished(Anim_TurnEnd, TurnEndDelegate);
	BindToAnimationFinished(Anim_TurnEnd2, TurnEndDelegate);
	BindToAnimationFinished(Anim_BloodSet, BloodSetDelegate);

	StartCurrentHealth = AForgottenKnightCharacter::MyCurrentHealth;

	if (AForgottenKnightCharacter::MyAccessoryData[2] == 4) // 내가 주는 부식 강화
	{
		mycor = 1.5f;
	}
	else
	{
		mycor = 1.25f;
	}

	if (AForgottenKnightCharacter::MyAccessoryData[2] == 5) // 내가 주는 약화 강화
	{
		myweak = 0.5f;
	}
	else
	{
		myweak = 0.75f;
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AController_MainCharacter* Controller = Cast<AController_MainCharacter>(GetOwningPlayer());

	if (MyPlayer != nullptr)
	{
		if (MyPlayer->ActorHasTag("Background_1"))
		{
			Image_Background->SetBrush(Background_Stage1);
		}
		else if (MyPlayer->ActorHasTag("Background_2"))
		{
			Image_Background->SetBrush(Background_Stage2);
		}
		else if (MyPlayer->ActorHasTag("Background_Dungeon"))
		{
			Image_Background->SetBrush(Background_Dungeon);
		}
	}

	EnterHP = AForgottenKnightCharacter::MyCurrentHealth;

	Attack[1] = false;
	Attack[2] = false;
	Attack[3] = false;
	MobDone[1] = 0;
	MobDone[2] = 0;
	MobDone[3] = 0;
	MobTargetCard[1] = 0;
	MobTargetCard[2] = 0;
	MobTargetCard[3] = 0;
	MobTargetCost[1] = 0;
	MobTargetCost[2] = 0;
	MobTargetCost[3] = 0;
	CurrentCost = 3;
	MaxCost = 3;
	EnemyCount = 3;
	EnemyLife[1] = true;
	EnemyLife[2] = true;
	EnemyLife[3] = true;
	Breathe[1] = false;
	Breathe[2] = false;
	Breathe[3] = false;
	Breathe[0] = false;
	TurnCount = 0;
	HandCard.Empty();

	MixDeck = AController_MainCharacter::MyDeck;
	DrawDeck = MixDeck;
	DrawCardIndex = 0;
	CardCircle = MixDeck.Num();

	for (int i = 0; i < 1000; i++)
	{
		int x = dist1(rng1) % CardCircle;
		int y = dist1(rng1) % CardCircle;

		int z = MixDeck[x];
		MixDeck[x] = MixDeck[y];
		MixDeck[y] = z;
	}

	// 아수라 기믹
	int asura = 0;
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (Enemy->ActorHasTag("Asura") || Enemy->ActorHasTag("Asura_Up"))
			{
				asura += 1;
			}
		}
	}
	if (asura > 0) // 필드에 아수라 있으면
	{
		FieldinAsura = true;
		EnemyCount += 1; // 부활 패턴을 위해 적 카운트 +1
	}

	// 흑마 기믹
	int warlock = 0;
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (Enemy->ActorHasTag("Warlock"))
			{
				warlock += 1;
			}
		}
	}
	if (warlock > 0) // 필드에 흑마 있으면
	{
		FieldinWarlock = true;
		EnemyCount += 10; // 10마리 몹 추가 상대
		WarlockMobCount = 0; // 몹 잡은 카운트
	}

	//백인대장 기믹
	if (MyPlayer->ActorHasTag("Buff_SkeletonCenturion") || MyPlayer->ActorHasTag("Buff_SkeletonCenturion_D"))
	{
		SkullReviveCooltime = 1;
		Image_Buffmob->SetVisibility(ESlateVisibility::Visible);
		Image_Buffmob->SetBrush(ImgSkeletonCenturion);
	}
	else
	{
		SkullReviveCooltime = 2;
	}

	SkeletonFlag();

	// 왕 등장
	if (MyPlayer->ActorHasTag("Stage2_10"))
	{
		LastStage = true;
		SpecialBoomOn = true;
		Image_Buffmob->SetVisibility(ESlateVisibility::Visible);
		Image_Buffmob->SetBrush(ImgKing);
	}



	DurahanGiveHead();

	TurnStart();
	UpdateCost();
	UpdateMaxCost();
	ChangeTurnText();
}

void UUI_PlayWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (WaitBlooding)
	{
		AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (MyPlayer->AllBloodEnd == 0)
		{
			TurnClear();
			WaitBlooding = false;
		}
	}

	if (CardBox2->GetChildrenCount() == 0 && !DeckFront)
	{
		OnButtonFrontHandClicked();
	}

	if (EnemyHealthbar1->IamLich && !FieldOnlyLich())
	{
		EnemyHealthbar1->Lichimmune = true;
	}
	else if (EnemyHealthbar1->IamLich && FieldOnlyLich())
	{
		EnemyHealthbar1->Lichimmune = false;
	}

	if (EnemyHealthbar2->IamLich && !FieldOnlyLich())
	{
		EnemyHealthbar2->Lichimmune = true;
	}
	else if (EnemyHealthbar2->IamLich && FieldOnlyLich())
	{
		EnemyHealthbar2->Lichimmune = false;
	}

	if (EnemyHealthbar3->IamLich && !FieldOnlyLich())
	{
		EnemyHealthbar3->Lichimmune = true;
	}
	else if (EnemyHealthbar3->IamLich && FieldOnlyLich())
	{
		EnemyHealthbar3->Lichimmune = false;
	}

	if (CardBox2->GetChildrenCount() > 0) // 카드 박스2에 카드가 있으면
	{
		Button_FrontHand->SetVisibility(ESlateVisibility::Visible);
		Button_BehindHand->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Button_FrontHand->SetVisibility(ESlateVisibility::Collapsed);
		Button_BehindHand->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void UUI_PlayWindow::ChangeTurnText()
{
	if (!Attack[1]) // 선제공격 턴이면
	{
		for (int i = 0; i < CardBox->GetChildrenCount(); i++)
		{
			UUI_PlayCard* MyCard = Cast<UUI_PlayCard>(CardBox->GetChildAt(i));
			if (MyCard != nullptr)
			{
				MyCard->ChangeTurnText(1);
			}
		}

		if (CardBox2->GetChildrenCount() > 0)
		{
			for (int i = 0; i < CardBox2->GetChildrenCount(); i++)
			{
				UUI_PlayCard* MyCard = Cast<UUI_PlayCard>(CardBox2->GetChildAt(i));
				if (MyCard != nullptr)
				{
					MyCard->ChangeTurnText(1);
				}
			}
		}
	}
	else if (!Attack[2]) // 패링 턴이면
	{
		for (int i = 0; i < CardBox->GetChildrenCount(); i++)
		{
			UUI_PlayCard* MyCard = Cast<UUI_PlayCard>(CardBox->GetChildAt(i));
			if (MyCard != nullptr)
			{
				MyCard->ChangeTurnText(2);
			}
		}

		if (CardBox2->GetChildrenCount() > 0)
		{
			for (int i = 0; i < CardBox2->GetChildrenCount(); i++)
			{
				UUI_PlayCard* MyCard = Cast<UUI_PlayCard>(CardBox2->GetChildAt(i));
				if (MyCard != nullptr)
				{
					MyCard->ChangeTurnText(2);
				}
			}
		}
	}
	else if (!Attack[3]) // 카운터 턴이면
	{
		for (int i = 0; i < CardBox->GetChildrenCount(); i++)
		{
			UUI_PlayCard* MyCard = Cast<UUI_PlayCard>(CardBox->GetChildAt(i));
			if (MyCard != nullptr)
			{
				MyCard->ChangeTurnText(3);
			}
		}

		if (CardBox2->GetChildrenCount() > 0)
		{
			for (int i = 0; i < CardBox2->GetChildrenCount(); i++)
			{
				UUI_PlayCard* MyCard = Cast<UUI_PlayCard>(CardBox2->GetChildAt(i));
				if (MyCard != nullptr)
				{
					MyCard->ChangeTurnText(3);
				}
			}
		}
	}
}

void UUI_PlayWindow::OnButtonPauseClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowPauseWidget();
	}
}

void UUI_PlayWindow::Mix() // 미완
{
	MixDeck = DumpDeck; // 버린 카드 중에서 섞기
	DrawDeck = MixDeck;
	DumpDeck.Empty();

	CardCircle = MixDeck.Num();

	for (int i = 0; i < 1000; i++)
	{
		int x = dist1(rng1) % CardCircle;
		int y = dist1(rng1) % CardCircle;

		int z = MixDeck[x];
		MixDeck[x] = MixDeck[y];
		MixDeck[y] = z;
	}
}

void UUI_PlayWindow::ChangeAttackImage(int AttackNum, int CardNum, int EnemyNum)
{
	if (AttackNum == 1)
	{
		if (EnemyNum == 0)
		{
			FString str = (" ");
			Text_1st->SetText(FText::FromString(str));
		}
		else if (EnemyNum == 1)
		{
			FString str = ("L");
			Text_1st->SetText(FText::FromString(str));
		}
		else if (EnemyNum == 2)
		{
			FString str = ("C");
			Text_1st->SetText(FText::FromString(str));
		}
		else if (EnemyNum == 3)
		{
			FString str = ("R");
			Text_1st->SetText(FText::FromString(str));
		}

		// 사용한 카드
		if (CardNum == -1)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Collapsed);
		}
		else if (CardNum == 0)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(ImgBreathe);
		}
		else if (CardNum == 1 || CardNum == 101)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card1);
		}
		else if (CardNum == 2 || CardNum == 102)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card2);
		}
		else if (CardNum == 3 || CardNum == 103)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card3);
		}
		else if (CardNum == 4 || CardNum == 104)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card4);
		}
		else if (CardNum == 5 || CardNum == 105)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card5);
		}
		else if (CardNum == 6 || CardNum == 106)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card6);
		}
		else if (CardNum == 7 || CardNum == 107)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card7);
		}
		else if (CardNum == 8 || CardNum == 108)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card8);
		}
		else if (CardNum == 9 || CardNum == 109)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card9);
		}
		else if (CardNum == 10 || CardNum == 110)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card10);
		}
		else if (CardNum == 11 || CardNum == 111)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card11);
		}
		else if (CardNum == 12 || CardNum == 112)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card12);
		}
		else if (CardNum == 13 || CardNum == 113)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card13);
		}
		else if (CardNum == 14 || CardNum == 114)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card14);
		}
		else if (CardNum == 15 || CardNum == 115)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card15);
		}
		else if (CardNum == 16 || CardNum == 116)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card16);
		}
		else if (CardNum == 17 || CardNum == 117)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card17);
		}
		else if (CardNum == 18 || CardNum == 118)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card18);
		}
		else if (CardNum == 19 || CardNum == 119)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card19);
		}
		else if (CardNum == 20 || CardNum == 120)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card20);
		}
		else if (CardNum == 21 || CardNum == 121)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card21);
		}
		else if (CardNum == 22 || CardNum == 122)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card22);
		}
		else if (CardNum == 23 || CardNum == 123)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card23);
		}
		else if (CardNum == 24 || CardNum == 124)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card24);
		}
		else if (CardNum == 25 || CardNum == 125)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card25);
		}
		else if (CardNum == 26 || CardNum == 126)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card26);
		}
		else if (CardNum == 27 || CardNum == 127)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card27);
		}
		else if (CardNum == 31 || CardNum == 131)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card31);
		}
		else if (CardNum == 32 || CardNum == 132)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card32);
		}
		else if (CardNum == 33 || CardNum == 133)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card33);
		}
		else if (CardNum == 34 || CardNum == 134)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card34);
		}
		else if (CardNum == 35 || CardNum == 135)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card35);
		}
		else if (CardNum == 36 || CardNum == 136)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card36);
		}
		else if (CardNum == 37 || CardNum == 137)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card37);
		}
		else if (CardNum == 41 || CardNum == 141)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card41);
		}
		else if (CardNum == 42 || CardNum == 142)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card42);
		}
		else if (CardNum == 43 || CardNum == 143)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card43);
		}
		else if (CardNum == 44 || CardNum == 144)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card44);
		}
		else if (CardNum == 45 || CardNum == 145)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card45);
		}
		else if (CardNum == 46 || CardNum == 146)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card46);
		}
		else if (CardNum == 47 || CardNum == 147)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card47);
		}
		else if (CardNum == 48 || CardNum == 148)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card48);
		}
		else if (CardNum == 49 || CardNum == 149)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card49);
		}
		else if (CardNum == 50 || CardNum == 150)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card50);
		}
		else if (CardNum == 51 || CardNum == 151)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card51);
		}
		else if (CardNum == 52 || CardNum == 152)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card52);
		}
		else if (CardNum == 53 || CardNum == 153)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card53);
		}
		else if (CardNum == 54 || CardNum == 154)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card54);
		}


		// 특수 카드
		if (CardNum == 81)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card81);
		}
		else if (CardNum == 82)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card82);
		}
		else if (CardNum == 83)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card83);
		}
		else if (CardNum == 84)
		{
			Image_AttackCard1->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard1->SetBrush(Card84);
		}

		// 상대 적
		if (EnemyNum == 0)
		{
			Image_AttackMob1->SetVisibility(ESlateVisibility::Collapsed);
		}

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;
			if (Enemy != nullptr)
			{
				if ((Enemy->ActorHasTag("Enemy1") && EnemyNum == 1) || (Enemy->ActorHasTag("Enemy2") && EnemyNum == 2) || (Enemy->ActorHasTag("Enemy3") && EnemyNum == 3))
				{
					if (Enemy->ActorHasTag("Slime"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgSlime);
					}
					else if (Enemy->ActorHasTag("Goblin"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgGoblin);
					}
					else if (Enemy->ActorHasTag("Troll"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgTroll);
					}
					else if (Enemy->ActorHasTag("Spider"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgSpider);
					}
					else if (Enemy->ActorHasTag("Mantis"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgMantis);
					}
					else if (Enemy->ActorHasTag("Golem"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgGolem);
					}
					else if (Enemy->ActorHasTag("Asura"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgAsura);
					}
					else if (Enemy->ActorHasTag("ShadowSpear"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgShadowSpear);
					}
					else if (Enemy->ActorHasTag("ShadowSheild"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgShadowSheild);
					}
					else if (Enemy->ActorHasTag("ShadowAxe"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgShadowAxe);
					}
					else if (Enemy->ActorHasTag("Griffon"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgGriffon);
					}
					else if (Enemy->ActorHasTag("Rafflesia"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgRafflesia);
					}
					else if (Enemy->ActorHasTag("Warlock2Phase"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgWarlock2Phase);
					}
					else if (Enemy->ActorHasTag("Warlock"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgWarlock);
					}
					else if (Enemy->ActorHasTag("SkullHead"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgSkullHead);
					}
					else if (Enemy->ActorHasTag("SkeletonSoldier"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgSkeletonSoldier);
					}
					else if (Enemy->ActorHasTag("SkeletonArcher"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgSkeletonArcher);
					}
					else if (Enemy->ActorHasTag("SkeletonGuardDog"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgSkeletonGuardDog);
					}
					else if (Enemy->ActorHasTag("Lich"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgLich);
					}
					else if (Enemy->ActorHasTag("Beholder"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgBeholder);
					}
					else if (Enemy->ActorHasTag("FallenFairy"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgFallenFairy);
					}
					else if (Enemy->ActorHasTag("Baphomet"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgBaphomet);
					}
					else if (Enemy->ActorHasTag("Durahan"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgDurahan);
					}
					else if (Enemy->ActorHasTag("MagicBall"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgBall);
					}
					else if (Enemy->ActorHasTag("DemonLordHead"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgDemonLordHead);
					}
					else if (Enemy->ActorHasTag("DemonLordLeft"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgDemonLordLeft);
					}
					else if (Enemy->ActorHasTag("DemonLordRight"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgDemonLordRight);
					}
					else if (Enemy->ActorHasTag("SkeletonCenturion"))
					{
						Image_AttackMob1->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob1->SetBrush(ImgSkeletonCenturion);
					}
				}
			}
		}
	}

	if (AttackNum == 2)
	{
		if (EnemyNum == 0)
		{
			FString str = (" ");
			Text_2nd->SetText(FText::FromString(str));
		}
		else if (EnemyNum == 1)
		{
			FString str = ("L");
			Text_2nd->SetText(FText::FromString(str));
		}
		else if (EnemyNum == 2)
		{
			FString str = ("C");
			Text_2nd->SetText(FText::FromString(str));
		}
		else if (EnemyNum == 3)
		{
			FString str = ("R");
			Text_2nd->SetText(FText::FromString(str));
		}

		// 사용한 카드
		if (CardNum == -1)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Collapsed);
		}
		else if (CardNum == 0)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(ImgBreathe);
		}
		else if (CardNum == 1 || CardNum == 101)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card1);
		}
		else if (CardNum == 2 || CardNum == 102)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card2);
		}
		else if (CardNum == 3 || CardNum == 103)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card3);
		}
		else if (CardNum == 4 || CardNum == 104)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card4);
		}
		else if (CardNum == 5 || CardNum == 105)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card5);
		}
		else if (CardNum == 6 || CardNum == 106)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card6);
		}
		else if (CardNum == 7 || CardNum == 107)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card7);
		}
		else if (CardNum == 8 || CardNum == 108)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card8);
		}
		else if (CardNum == 9 || CardNum == 109)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card9);
		}
		else if (CardNum == 10 || CardNum == 110)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card10);
		}
		else if (CardNum == 11 || CardNum == 111)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card11);
		}
		else if (CardNum == 12 || CardNum == 112)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card12);
		}
		else if (CardNum == 13 || CardNum == 113)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card13);
		}
		else if (CardNum == 14 || CardNum == 114)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card14);
		}
		else if (CardNum == 15 || CardNum == 115)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card15);
		}
		else if (CardNum == 16 || CardNum == 116)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card16);
		}
		else if (CardNum == 17 || CardNum == 117)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card17);
		}
		else if (CardNum == 18 || CardNum == 118)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card18);
		}
		else if (CardNum == 19 || CardNum == 119)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card19);
		}
		else if (CardNum == 20 || CardNum == 120)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card20);
		}
		else if (CardNum == 21 || CardNum == 121)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card21);
		}
		else if (CardNum == 22 || CardNum == 122)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card22);
		}
		else if (CardNum == 23 || CardNum == 123)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card23);
		}
		else if (CardNum == 24 || CardNum == 124)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card24);
		}
		else if (CardNum == 25 || CardNum == 125)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card25);
		}
		else if (CardNum == 26 || CardNum == 126)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card26);
		}
		else if (CardNum == 27 || CardNum == 127)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card27);
		}
		else if (CardNum == 31 || CardNum == 131)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card31);
		}
		else if (CardNum == 32 || CardNum == 132)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card32);
		}
		else if (CardNum == 33 || CardNum == 133)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card33);
		}
		else if (CardNum == 34 || CardNum == 134)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card34);
		}
		else if (CardNum == 35 || CardNum == 135)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card35);
		}
		else if (CardNum == 36 || CardNum == 136)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card36);
		}
		else if (CardNum == 37 || CardNum == 137)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card37);
		}
		else if (CardNum == 41 || CardNum == 141)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card41);
		}
		else if (CardNum == 42 || CardNum == 142)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card42);
		}
		else if (CardNum == 43 || CardNum == 143)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card43);
		}
		else if (CardNum == 44 || CardNum == 144)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card44);
		}
		else if (CardNum == 45 || CardNum == 145)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card45);
		}
		else if (CardNum == 46 || CardNum == 146)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card46);
		}
		else if (CardNum == 47 || CardNum == 147)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card47);
		}
		else if (CardNum == 48 || CardNum == 148)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card48);
		}
		else if (CardNum == 49 || CardNum == 149)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card49);
		}
		else if (CardNum == 50 || CardNum == 150)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card50);
		}
		else if (CardNum == 51 || CardNum == 151)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card51);
		}
		else if (CardNum == 52 || CardNum == 152)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card52);
		}
		else if (CardNum == 53 || CardNum == 153)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card53);
		}
		else if (CardNum == 54 || CardNum == 154)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card54);
		}


		// 특수 카드
		if (CardNum == 81)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card81);
		}
		else if (CardNum == 82)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card82);
		}
		else if (CardNum == 83)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card83);
		}
		else if (CardNum == 84)
		{
			Image_AttackCard2->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard2->SetBrush(Card84);
		}

		// 상대 적
		if (EnemyNum == 0)
		{
			Image_AttackMob2->SetVisibility(ESlateVisibility::Collapsed);
		}

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;
			if (Enemy != nullptr)
			{
				if ((Enemy->ActorHasTag("Enemy1") && EnemyNum == 1) || (Enemy->ActorHasTag("Enemy2") && EnemyNum == 2) || (Enemy->ActorHasTag("Enemy3") && EnemyNum == 3))
				{
					if (Enemy->ActorHasTag("Slime"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgSlime);
					}
					else if (Enemy->ActorHasTag("Goblin"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgGoblin);
					}
					else if (Enemy->ActorHasTag("Troll"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgTroll);
					}
					else if (Enemy->ActorHasTag("Spider"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgSpider);
					}
					else if (Enemy->ActorHasTag("Mantis"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgMantis);
					}
					else if (Enemy->ActorHasTag("Golem"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgGolem);
					}
					else if (Enemy->ActorHasTag("Asura"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgAsura);
					}
					else if (Enemy->ActorHasTag("ShadowSpear"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgShadowSpear);
					}
					else if (Enemy->ActorHasTag("ShadowSheild"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgShadowSheild);
					}
					else if (Enemy->ActorHasTag("ShadowAxe"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgShadowAxe);
					}
					else if (Enemy->ActorHasTag("Griffon"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgGriffon);
					}
					else if (Enemy->ActorHasTag("Rafflesia"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgRafflesia);
					}
					else if (Enemy->ActorHasTag("Warlock2Phase"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgWarlock2Phase);
					}
					else if (Enemy->ActorHasTag("Warlock"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgWarlock);
					}
					else if (Enemy->ActorHasTag("SkullHead"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgSkullHead);
					}
					else if (Enemy->ActorHasTag("SkeletonSoldier"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgSkeletonSoldier);
					}
					else if (Enemy->ActorHasTag("SkeletonArcher"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgSkeletonArcher);
					}
					else if (Enemy->ActorHasTag("SkeletonGuardDog"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgSkeletonGuardDog);
					}
					else if (Enemy->ActorHasTag("Lich"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgLich);
					}
					else if (Enemy->ActorHasTag("Beholder"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgBeholder);
					}
					else if (Enemy->ActorHasTag("FallenFairy"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgFallenFairy);
					}
					else if (Enemy->ActorHasTag("Baphomet"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgBaphomet);
					}
					else if (Enemy->ActorHasTag("Durahan"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgDurahan);
					}
					else if (Enemy->ActorHasTag("MagicBall"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgBall);
					}
					else if (Enemy->ActorHasTag("DemonLordHead"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgDemonLordHead);
					}
					else if (Enemy->ActorHasTag("DemonLordLeft"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgDemonLordLeft);
					}
					else if (Enemy->ActorHasTag("DemonLordRight"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgDemonLordRight);
					}
					else if (Enemy->ActorHasTag("SkeletonCenturion"))
					{
						Image_AttackMob2->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob2->SetBrush(ImgSkeletonCenturion);
					}
				}
			}
		}
	}

	if (AttackNum == 3)
	{
		if (EnemyNum == 0)
		{
			FString str = (" ");
			Text_3rd->SetText(FText::FromString(str));
		}
		else if (EnemyNum == 1)
		{
			FString str = ("L");
			Text_3rd->SetText(FText::FromString(str));
		}
		else if (EnemyNum == 2)
		{
			FString str = ("C");
			Text_3rd->SetText(FText::FromString(str));
		}
		else if (EnemyNum == 3)
		{
			FString str = ("R");
			Text_3rd->SetText(FText::FromString(str));
		}

		// 사용한 카드
		if (CardNum == -1)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Collapsed);
		}
		else if (CardNum == 0)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(ImgBreathe);
		}
		else if (CardNum == 1 || CardNum == 101)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card1);
		}
		else if (CardNum == 2 || CardNum == 102)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card2);
		}
		else if (CardNum == 3 || CardNum == 103)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card3);
		}
		else if (CardNum == 4 || CardNum == 104)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card4);
		}
		else if (CardNum == 5 || CardNum == 105)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card5);
		}
		else if (CardNum == 6 || CardNum == 106)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card6);
		}
		else if (CardNum == 7 || CardNum == 107)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card7);
		}
		else if (CardNum == 8 || CardNum == 108)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card8);
		}
		else if (CardNum == 9 || CardNum == 109)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card9);
		}
		else if (CardNum == 10 || CardNum == 110)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card10);
		}
		else if (CardNum == 11 || CardNum == 111)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card11);
		}
		else if (CardNum == 12 || CardNum == 112)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card12);
		}
		else if (CardNum == 13 || CardNum == 113)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card13);
		}
		else if (CardNum == 14 || CardNum == 114)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card14);
		}
		else if (CardNum == 15 || CardNum == 115)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card15);
		}
		else if (CardNum == 16 || CardNum == 116)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card16);
		}
		else if (CardNum == 17 || CardNum == 117)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card17);
		}
		else if (CardNum == 18 || CardNum == 118)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card18);
		}
		else if (CardNum == 19 || CardNum == 119)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card19);
		}
		else if (CardNum == 20 || CardNum == 120)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card20);
		}
		else if (CardNum == 21 || CardNum == 121)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card21);
		}
		else if (CardNum == 22 || CardNum == 122)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card22);
		}
		else if (CardNum == 23 || CardNum == 123)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card23);
		}
		else if (CardNum == 24 || CardNum == 124)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card24);
		}
		else if (CardNum == 25 || CardNum == 125)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card25);
		}
		else if (CardNum == 26 || CardNum == 126)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card26);
		}
		else if (CardNum == 27 || CardNum == 127)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card27);
		}
		else if (CardNum == 31 || CardNum == 131)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card31);
		}
		else if (CardNum == 32 || CardNum == 132)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card32);
		}
		else if (CardNum == 33 || CardNum == 133)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card33);
		}
		else if (CardNum == 34 || CardNum == 134)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card34);
		}
		else if (CardNum == 35 || CardNum == 135)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card35);
		}
		else if (CardNum == 36 || CardNum == 136)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card36);
		}
		else if (CardNum == 37 || CardNum == 137)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card37);
		}
		else if (CardNum == 41 || CardNum == 141)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card41);
		}
		else if (CardNum == 42 || CardNum == 142)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card42);
		}
		else if (CardNum == 43 || CardNum == 143)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card43);
		}
		else if (CardNum == 44 || CardNum == 144)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card44);
		}
		else if (CardNum == 45 || CardNum == 145)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card45);
		}
		else if (CardNum == 46 || CardNum == 146)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card46);
		}
		else if (CardNum == 47 || CardNum == 147)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card47);
		}
		else if (CardNum == 48 || CardNum == 148)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card48);
		}
		else if (CardNum == 49 || CardNum == 149)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card49);
		}
		else if (CardNum == 50 || CardNum == 150)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card50);
		}
		else if (CardNum == 51 || CardNum == 151)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card51);
		}
		else if (CardNum == 52 || CardNum == 152)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card52);
		}
		else if (CardNum == 53 || CardNum == 153)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card53);
		}
		else if (CardNum == 54 || CardNum == 154)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card54);
		}


		// 특수 카드
		if (CardNum == 81)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card81);
		}
		else if (CardNum == 82)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card82);
		}
		else if (CardNum == 83)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card83);
		}
		else if (CardNum == 84)
		{
			Image_AttackCard3->SetVisibility(ESlateVisibility::Visible);
			Image_AttackCard3->SetBrush(Card84);
		}

		// 상대 적
		if (EnemyNum == 0)
		{
			Image_AttackMob3->SetVisibility(ESlateVisibility::Collapsed);
		}

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;
			if (Enemy != nullptr)
			{
				if ((Enemy->ActorHasTag("Enemy1") && EnemyNum == 1) || (Enemy->ActorHasTag("Enemy2") && EnemyNum == 2) || (Enemy->ActorHasTag("Enemy3") && EnemyNum == 3))
				{
					if (Enemy->ActorHasTag("Slime"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgSlime);
					}
					else if (Enemy->ActorHasTag("Goblin"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgGoblin);
					}
					else if (Enemy->ActorHasTag("Troll"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgTroll);
					}
					else if (Enemy->ActorHasTag("Spider"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgSpider);
					}
					else if (Enemy->ActorHasTag("Mantis"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgMantis);
					}
					else if (Enemy->ActorHasTag("Golem"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgGolem);
					}
					else if (Enemy->ActorHasTag("Asura"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgAsura);
					}
					else if (Enemy->ActorHasTag("ShadowSpear"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgShadowSpear);
					}
					else if (Enemy->ActorHasTag("ShadowSheild"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgShadowSheild);
					}
					else if (Enemy->ActorHasTag("ShadowAxe"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgShadowAxe);
					}
					else if (Enemy->ActorHasTag("Griffon"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgGriffon);
					}
					else if (Enemy->ActorHasTag("Rafflesia"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgRafflesia);
					}
					else if (Enemy->ActorHasTag("Warlock2Phase"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgWarlock2Phase);
					}
					else if (Enemy->ActorHasTag("Warlock"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgWarlock);
					}
					else if (Enemy->ActorHasTag("SkullHead"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgSkullHead);
					}
					else if (Enemy->ActorHasTag("SkeletonSoldier"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgSkeletonSoldier);
					}
					else if (Enemy->ActorHasTag("SkeletonArcher"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgSkeletonArcher);
					}
					else if (Enemy->ActorHasTag("SkeletonGuardDog"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgSkeletonGuardDog);
					}
					else if (Enemy->ActorHasTag("Lich"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgLich);
					}
					else if (Enemy->ActorHasTag("Beholder"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgBeholder);
					}
					else if (Enemy->ActorHasTag("FallenFairy"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgFallenFairy);
					}
					else if (Enemy->ActorHasTag("Baphomet"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgBaphomet);
					}
					else if (Enemy->ActorHasTag("Durahan"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgDurahan);
					}
					else if (Enemy->ActorHasTag("MagicBall"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgBall);
					}
					else if (Enemy->ActorHasTag("DemonLordHead"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgDemonLordHead);
					}
					else if (Enemy->ActorHasTag("DemonLordLeft"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgDemonLordLeft);
					}
					else if (Enemy->ActorHasTag("DemonLordRight"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgDemonLordRight);
					}
					else if (Enemy->ActorHasTag("SkeletonCenturion"))
					{
						Image_AttackMob3->SetVisibility(ESlateVisibility::Visible);
						Image_AttackMob3->SetBrush(ImgSkeletonCenturion);
					}
				}
			}
		}
	}
}

bool UUI_PlayWindow::CardDragControl(int CardNum)
{
	if ((CardNum == 4 || CardNum == 104) && ((Attack[1] == false) || (Attack[2] == false)))
	{
		return false;
	}
	else if ((CardNum == 8 || CardNum == 108) && (Attack[1] == false || (Attack[2] == true && Attack[3] == false)))
	{
		return false;
	}
	else if ((CardNum == 15 || CardNum == 115) && ((Attack[1] == false) || (Attack[2] == false)))
	{
		return false;
	}
	else if ((CardNum == 35 || CardNum == 135) && ((Attack[1] == true) || (Attack[2] == true)))
	{
		return false;
	}

	return true;
}

void UUI_PlayWindow::TurnStart() // 미완
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// 던전 2 기믹
	if (MyPlayer->ActorHasTag("Dungeon2_1") || MyPlayer->ActorHasTag("Dungeon2_2") || MyPlayer->ActorHasTag("Dungeon2_3")) // 던전 2에서는 턴 시작 시 모두에게 출혈 1스택 부여
	{
		MyPlayer->GetStackBlooding += 1;
		MyHealthbar0->UpdateNagativeEffect(MyPlayer->GetStackCorrosion, MyPlayer->GetStackWeakening, MyPlayer->GetStackBlooding);

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;
			if (Enemy != nullptr && EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
			{
				Enemy->GetStackBlooding += 1;
				EnemyHealthbar1->UpdateNagativeEffect(Enemy->GetStackCorrosion, Enemy->GetStackWeakening, Enemy->GetStackBlooding);
			}
			else if (Enemy != nullptr && EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
			{
				Enemy->GetStackBlooding += 1;
				EnemyHealthbar2->UpdateNagativeEffect(Enemy->GetStackCorrosion, Enemy->GetStackWeakening, Enemy->GetStackBlooding);
			}
			else if (Enemy != nullptr && EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
			{
				Enemy->GetStackBlooding += 1;
				EnemyHealthbar3->UpdateNagativeEffect(Enemy->GetStackCorrosion, Enemy->GetStackWeakening, Enemy->GetStackBlooding);
			}
		}
	}

	if (CardWidget != nullptr)
	{
		TurnCount += 1;

		AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
		MyController->FreezeStart();
		FString sStr = FString::Printf(TEXT("턴 시작")); // 한글 되게 재인코딩하기
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_TurnStart);

		if (MyPlayer->BonusDrawDumpDeck > 0) // 이번 턴에 버린 덱에서 카드 가져온다면
		{
			int k = MyPlayer->BonusDrawDumpDeck;
			if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() + k <= 10) // 손패 제한에 문제가 없다면
			{
				for (int i = 0; i < k; i++)
				{
					MyController->ShowDumpDeckWidget(DumpDeck);
				}
			}
			else // 손패 제한에 문제가 있으면
			{
				int xk = (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() + k) - 10;
				k -= xk;
				for (int i = 0; i < k; i++)
				{
					MyController->ShowDumpDeckWidget(DumpDeck);
				}
			}

			MyPlayer->BonusDrawDumpDeck = 0;
		}

		if (BeChronicity) // 이번 턴에 대기만성 효과를 받는다면
		{
			BeChronicity = false;

			if (TakeChronicityNum == 1) // 삼중연쇄 카드 가져오기 13~15
			{
				while (DumpDeck.Contains(13))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(13);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(14))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(14);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(15))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(15);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(113))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(113);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(114))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(114);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(115))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(115);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(13))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(13);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(14))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(14);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(15))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(15);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(113))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(113);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(114))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(114);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(115))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(115);
					}
					else
					{
						break;
					}
				}
			}
			else if (TakeChronicityNum == 2) // 잽잽훅 카드 가져오기 16~18
			{
				while (DumpDeck.Contains(16))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(16);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(17))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(17);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(18))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(18);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(116))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(116);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(117))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(117);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(118))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(118);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(16))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(16);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(17))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(17);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(18))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(18);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(116))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(116);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(117))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(117);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(118))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(118);
					}
					else
					{
						break;
					}
				}
			}
			else if (TakeChronicityNum == 3) // 삼위일체 카드 가져오기 25-27
			{
				while (DumpDeck.Contains(25))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(25);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(26))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(26);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(27))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(27);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(125))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(125);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(126))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(126);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(127))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(127);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(25))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(25);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(26))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(26);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(27))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(27);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(125))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(125);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(126))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(126);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(127))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(127);
					}
					else
					{
						break;
					}
				}
			}
			else if (TakeChronicityNum == 4) // 오의 : 절멸 카드 가져오기 31 35 50 51
			{
				while (DumpDeck.Contains(31))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(31);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(35))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(35);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(50))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(50);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(51))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(51);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(131))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(131);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(135))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(135);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(150))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(150);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(151))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(151);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(31))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(31);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(35))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(35);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(50))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(50);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(51))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(51);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(131))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(131);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(135))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(135);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(150))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(150);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(151))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(151);
					}
					else
					{
						break;
					}
				}
			}
			else if (TakeChronicityNum == 5) // 광란 카드 가져오기 41-43
			{
				while (DumpDeck.Contains(41))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(41);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(42))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(42);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(43))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(43);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(141))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(141);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(142))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(142);
					}
					else
					{
						break;
					}
				}
				while (DumpDeck.Contains(143))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDumpCard(143);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(41))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(41);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(42))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(42);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(43))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(43);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(141))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(141);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(142))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(142);
					}
					else
					{
						break;
					}
				}
				while (DrawDeck.Contains(143))
				{
					if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10)
					{
						TakeDrawCard(143);
					}
					else
					{
						break;
					}
				}
			}

			TakeChronicityNum = 0;
		}

		DrawCard(5 + MyPlayer->BonusDrawNum);
		MyPlayer->BonusDrawNum = 0;
	}
}

void UUI_PlayWindow::DrawCard(int DrawNum) // DrawNum만큼 드로우
{

	int BonusDrawing = 0; // 추가 드로우 있으면 필요한 변수

	for (int a = 0; a < DrawNum; a++)
	{
		if (CardBox->GetChildrenCount() + CardBox2->GetChildrenCount() < 10) // 손패 제한에 문제가 없다면
		{
			//남은 카드가 0장이면, 버린 카드에서 다시 Mix한다.
			if (DrawCardIndex >= MixDeck.Num())
			{
				Mix();
				DrawCardIndex = 0;
			}

			UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
			UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);

			if (BeDelirium) // 착란 상태이면
			{
				int fixco = distDelirium(rng1);
				NewCard->FixCost = fixco;
			}

			if (CardBox->GetChildrenCount() < 5)
			{
				UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
				NewCard->InsertImage(MixDeck[DrawCardIndex]);

				if (BeCombination && !NewCard->ComboCard && CombinationBonusDraw > 0) // 연격의 대가 상태이고, 드로우된 카드가 콤보 카드가 아니고, 추가 드로우 상한에 초과 안되었으면 추가 드로우
				{
					CombinationBonusDraw -= 1;
					BonusDrawing += 1;
				}

				DrawDeck.RemoveSingle(MixDeck[DrawCardIndex]); // 해당 카드 한장 덱에서 삭제
				HandCard.Add(MixDeck[DrawCardIndex]);
				UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
				BoxSlot->SetPadding(10);
				FSlateChildSize Size(ESlateSizeRule::Type::Fill);
				BoxSlot->SetSize(Size);
				BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

				DrawCardIndex += 1;
			}
			else
			{
				UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
				NewCard->InsertImage(MixDeck[DrawCardIndex]);

				if (BeCombination && !NewCard->ComboCard && CombinationBonusDraw > 0) // 연격의 대가 상태이고, 드로우된 카드가 콤보 카드가 아니고, 추가 드로우 상한에 초과 안되었으면 추가 드로우
				{
					CombinationBonusDraw -= 1;
					BonusDrawing += 1;
				}

				DrawDeck.RemoveSingle(MixDeck[DrawCardIndex]); // 해당 카드 한장 덱에서 삭제
				HandCard.Add(MixDeck[DrawCardIndex]);
				UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
				BoxSlot->SetPadding(10);
				FSlateChildSize Size(ESlateSizeRule::Type::Fill);
				BoxSlot->SetSize(Size);
				BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

				DrawCardIndex += 1;
			}

			UE_LOG(LogTemp, Log, TEXT("Add :: %d"), DrawCardIndex);
		}
		else
		{
			FString sStr = FString::Printf(TEXT("손패에 카드가 넘치고 있습니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
		}
	}

	if (BonusDrawing > 0)
	{
		int ktkd = BonusDrawing;
		BonusDrawing = 0;
		DrawCard(ktkd);
	}

	ChangeHandCost();
	ChangeTurnText();
}

void UUI_PlayWindow::UpdateCost() // 미완
{
	FString NewString = FString::FromInt(CurrentCost);
	FText InText = FText::FromString(NewString);
	Text_CurrentCost->SetText(InText);
}

void UUI_PlayWindow::UpdateMaxCost() // 미완
{
	FString NewString = FString::FromInt(MaxCost);
	FText InText = FText::FromString(NewString);
	Text_MaxCost->SetText(InText);
}

bool UUI_PlayWindow::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UDragWidget* DragWidgetResult = Cast<UDragWidget>(InOperation);
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (!IsValid(DragWidgetResult))
	{
		UE_LOG(LogTemp, Log, TEXT("Cast returned null"));
		return false;
	}

	FVector2D DragWindowOffset = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	FVector2D DragWindowOffsetResult = DragWindowOffset - DragWidgetResult->DragOffset;

	TArray<bool> ScentMob = { false, false, false, false }; // 향기 상태를 가진 몹 식별

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;
		if (Enemy != nullptr)
		{
			if (Enemy->ActorHasTag("Enemy1") && EnemyLife[1])
			{
				if (Enemy->IamScent)
				{
					ScentMob[0] = true; // 향기 상태인 몹이 존재한다
					ScentMob[1] = true;
				}
				UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(UGameplayStatics::GetPlayerController(this, 0), Enemy->GetActorLocation(), PreMob1, true);
			}
			else if (Enemy->ActorHasTag("Enemy2") && EnemyLife[2])
			{
				if (Enemy->IamScent)
				{
					ScentMob[0] = true; // 향기 상태인 몹이 존재한다
					ScentMob[2] = true;
				}
				UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(UGameplayStatics::GetPlayerController(this, 0), Enemy->GetActorLocation(), PreMob2, true);
			}
			else if (Enemy->ActorHasTag("Enemy3") && EnemyLife[3])
			{
				if (Enemy->IamScent)
				{
					ScentMob[0] = true; // 향기 상태인 몹이 존재한다
					ScentMob[3] = true;
				}
				UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(UGameplayStatics::GetPlayerController(this, 0), Enemy->GetActorLocation(), PreMob3, true);
			}
		}

	}
	Mob1 = PreMob1 - ErrorGap;
	Mob2 = PreMob2 - ErrorGap;
	Mob3 = PreMob3 - ErrorGap;

	if (200 >= UKismetMathLibrary::Distance2D(Mob1, DragWindowOffsetResult) && MobDone[1] == 0 && EnemyLife[1] == true)
	{
		FString Thistag = DragWidgetResult->EnemyTag;
		MobTargetCard[1] = FCString::Atof(*Thistag);
		FString CostConfirm = DragWidgetResult->CostTag;
		MobTargetCost[1] = FCString::Atof(*CostConfirm);
		FString FixCostConfirm = DragWidgetResult->FixCostTag;
		MobTargetFixCost[1] = FCString::Atof(*FixCostConfirm);
		FString KeyWordinConfirm = DragWidgetResult->KeyWordinTag;
		int KeyWordinCard = FCString::Atof(*FixCostConfirm); // 키워드 들어가있으면 1, 아니면 0
		bool IsComboCardConfirm = DragWidgetResult->IsComboCard;


		bool CanIt = CardDragControl(MobTargetCard[1]); // 몹 타겟 카드가 특정 경우에만 사용 가능한 카드일 경우 식별하는 함수
		if (!CanIt)
		{
			FString sStr = FString::Printf(TEXT("호환되지 않는 카드입니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
			RecoverDragingCard();
			return false;
		}

		if (MyPlayer->Ironmode) // 철괴 모드이면
		{
			FString sStr = FString::Printf(TEXT("철괴를 사용하였습니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
			RecoverDragingCard();
			return false;
		}

		//코스트 증가 효과 적용(연격의 대가)
		if (!IsComboCardConfirm && BeCombination) // 콤보 카드가 아니면서 연격의 대가 발동 중이면
		{
			MobTargetCost[1] += 1;
		}

		//위기 모면 효과 적용
		if (MobTargetCard[1] == 49 || MobTargetCard[1] == 149)
		{
			if (EnemyLife[1])
			{
				MobTargetCost[1] -= 1;
				if (MobTargetCost[1] < 0)
				{
					MobTargetCost[1] = 0;
				}
			}
			if (EnemyLife[2])
			{
				MobTargetCost[1] -= 1;
				if (MobTargetCost[1] < 0)
				{
					MobTargetCost[1] = 0;
				}
			}
			if (EnemyLife[3])
			{
				MobTargetCost[1] -= 1;
				if (MobTargetCost[1] < 0)
				{
					MobTargetCost[1] = 0;
				}
			}
		}

		//코스트 감소 효과 적용 (오의 : 절멸)
		if ((MobTargetCard[1] == 31 || MobTargetCard[1] == 32 || MobTargetCard[1] == 35 || MobTargetCard[1] == 50 || MobTargetCard[1] == 51 || MobTargetCard[1] == 131 || MobTargetCard[1] == 132 || MobTargetCard[1] == 135 || MobTargetCard[1] == 150 || MobTargetCard[1] == 151) && OnCombo_Extinction_CostDown1)
		{
			MobTargetCost[1] -= 1;
			if (MobTargetCost[1] < 0)
			{
				MobTargetCost[1] = 0;
			}
		}

		//코스트 감소 효과 적용 (삼위일체)
		if ((MobTargetCard[1] == 25 || MobTargetCard[1] == 26 || MobTargetCard[1] == 27 || MobTargetCard[1] == 32 || MobTargetCard[1] == 125 || MobTargetCard[1] == 126 || MobTargetCard[1] == 127 || MobTargetCard[1] == 132) && OnCombo_Triangle_CostDown1)
		{
			MobTargetCost[1] -= 3;
			if (MobTargetCost[1] < 0)
			{
				MobTargetCost[1] = 0;
			}
		}

		if (KeyWordinCard == 0 && BeGrinding) // 키워드 x, 기본기 연마 시
		{
			MobTargetCost[1] -= 1;
			if (MobTargetCost[1] < 0)
			{
				MobTargetCost[1] = 0;
			}
		}

		//코스트 고정 효과 적용
		if (MobTargetFixCost[1] >= 0)
		{
			MobTargetCost[1] = MobTargetFixCost[1];
			if (KeyWordinCard == 0 && BeGrinding) // 키워드 x, 기본기 연마 시
			{
				MobTargetCost[1] -= 1;
				if (MobTargetCost[1] < 0)
				{
					MobTargetCost[1] = 0;
				}
			}
		}

		if (CurrentCost - MobTargetCost[1] < 0) // 코스트 관련 처리
		{
			FString sStr = FString::Printf(TEXT("코스트가 부족합니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
			MobTargetCost[1] = 0;
			RecoverDragingCard();
			return false;
		}

		//신속
		if (MobTargetCard[1] == 23) // 카드가 대기만성이면
		{
			CurrentCost -= MobTargetCost[1];

			bool gsd = ChronicitySelect();
			if (gsd)
			{
				BeChronicity = true;
				AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
				MyController->ShowDumpDeckWidget(ChronicityDeck);
			}
			else
			{
				FString sStr = FString::Printf(TEXT("연쇄 카드가 존재하지 않습니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
			}
			UpdateCost();
			return true;
		}
		else if (MobTargetCard[1] == 123) // 카드가 대기만성+면
		{
			CurrentCost -= MobTargetCost[1];
			CurrentCost += 1;

			bool gsd = ChronicitySelect();
			if (gsd)
			{
				BeChronicity = true;
				AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
				MyController->ShowDumpDeckWidget(ChronicityDeck);
			}
			else
			{
				FString sStr = FString::Printf(TEXT("연쇄 카드가 존재하지 않습니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
			}
			UpdateCost();
			return true;
		}

		if (MobTargetCard[1] == 33) // 카드가 명상이면
		{
			CurrentCost -= MobTargetCost[1];
			MyPlayer->GetStackCorrosion = 0;
			MyPlayer->GetStackWeakening = 0;
			MyPlayer->GetStackBlooding = 0;
			MyPlayer->GetCharmed = { false, false, false, false };
			MyHealthbar0->UpdateNagativeEffect(MyPlayer->GetStackCorrosion, MyPlayer->GetStackWeakening, MyPlayer->GetStackBlooding);
			UpdateCost();
			return true;
		}
		else if (MobTargetCard[1] == 133) // 카드가 명상+면
		{
			CurrentCost -= MobTargetCost[1];
			MyPlayer->GetStackCorrosion = 0;
			MyPlayer->GetStackWeakening = 0;
			MyPlayer->GetStackBlooding = 0;
			MyPlayer->GetCharmed = { false, false, false, false };
			MyHealthbar0->UpdateNagativeEffect(MyPlayer->GetStackCorrosion, MyPlayer->GetStackWeakening, MyPlayer->GetStackBlooding);
			UpdateCost();
			DrawCard(1);
			return true;
		}

		if (MobTargetCard[1] == 37 || MobTargetCard[1] == 137) // 카드가 무소유면 - 손패 버리고 버린 만큼 뽑기
		{
			CurrentCost -= MobTargetCost[1];
			int rts1 = CardBox->GetChildrenCount(); // 손패 카드 수
			int rts2 = CardBox2->GetChildrenCount();
			for (int x = 0; x < rts1; x++)
			{
				CardBox->RemoveChildAt(0); // 손패 카드들 삭제
			}
			for (int x = 0; x < rts2; x++)
			{
				CardBox2->RemoveChildAt(0); // 손패 카드들 삭제
			}
			DrawCard(rts1 + rts2);

			UpdateCost();
			return true;
		}

		if (MobTargetCard[1] == 44) // 카드가 날 세우기면
		{
			CurrentCost -= MobTargetCost[1];
			MyPlayer->AttackUp += 2;
			UpdateCost();
			return true;
		}
		else if (MobTargetCard[1] == 144) // 카드가 날 세우기+면
		{
			CurrentCost -= MobTargetCost[1];
			MyPlayer->AttackUp += 3;
			UpdateCost();
			return true;
		}

		if ((MobTargetCard[1] == 31 || MobTargetCard[1] == 131)) // 피해6, 8 오의:절멸1
		{
			CurrentCost -= MobTargetCost[1];
			OnCombo_Extinction_CostDown1 = true;

			if (MyPlayer->ExtinctionWholeAttack) // 광역 공격을 실시하면
			{
				MyPlayer->ExtinctionWholeAttack = false;
				MyPlayer->ExtinctionWholeAttackDone = true;
			}
			if (MobTargetCard[1] == 31)
			{
				FastAttacking(1, 6);
			}
			else
			{
				FastAttacking(1, 8);
			}

			UpdateCost();
			return true;
		}

		if ((MobTargetCard[1] == 50 || MobTargetCard[1] == 150)) // 피해2, 피해4 오의:절멸2 카드 선택
		{
			CurrentCost -= MobTargetCost[1];
			OnCombo_Extinction_CostDown1 = true;

			if (MyPlayer->ExtinctionWholeAttack) // 광역 공격을 실시하면
			{
				MyPlayer->ExtinctionWholeAttack = false;
				MyPlayer->ExtinctionWholeAttackDone = true;
			}
			if (MobTargetCard[1] == 50)
			{
				FastAttacking(1, 2);
			}
			else
			{
				FastAttacking(1, 4);
			}

			TArray<int32> ThisHand = HandCard;

			ThisHand.RemoveSingle(MobTargetCard[1]);

			if (ThisHand.Num() > 0)
			{
				AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
				if (PlayerController != nullptr)
				{
					PlayerController->ShowHandDeckWidget(ThisHand);
				}
			}

			UpdateCost();
			return true;
		}

		if (MobTargetCard[1] == 51 || MobTargetCard[1] == 151) // 카드가 오의:절멸3이면
		{
			CurrentCost -= MobTargetCost[1];
			OnCombo_Extinction_CostDown1 = true;

			MyPlayer->ExtinctionWholeAttack = true;
			OnCombo_Extinction_CostDown1 = true;
			UpdateCost();
			return true;
		}

		if ((MobTargetCard[1] == 25 || MobTargetCard[1] == 125)) // 삼위일체1 - 피해3, 모든 적 출혈3
		{
			CurrentCost -= MobTargetCost[1];
			OnCombo_Triangle_CostDown1 = true;
			if (MobTargetCard[1] == 25)
			{
				FastAttacking(1, 3);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackBlooding += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackBlooding += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackBlooding += 3;
							Enemy->UpdateNagativeEffect();
						}
					}
				}

				UpdateCost();
				return true;
			}
			else
			{
				FastAttacking(1, 4);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackBlooding += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackBlooding += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackBlooding += 4;
							Enemy->UpdateNagativeEffect();
						}
					}
				}

				UpdateCost();
				return true;
			}
		}

		if ((MobTargetCard[1] == 26 || MobTargetCard[1] == 126)) // 삼위일체2 - 피해3, 모든 적 부식3
		{
			CurrentCost -= MobTargetCost[1];
			OnCombo_Triangle_CostDown1 = true;
			if (MobTargetCard[1] == 26)
			{
				FastAttacking(1, 3);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackCorrosion += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackCorrosion += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackCorrosion += 3;
							Enemy->UpdateNagativeEffect();
						}
					}
				}

				UpdateCost();
				return true;
			}
			else
			{
				FastAttacking(1, 4);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackCorrosion += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackCorrosion += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackCorrosion += 4;
							Enemy->UpdateNagativeEffect();
						}
					}
				}

				UpdateCost();
				return true;
			}
		}

		if ((MobTargetCard[1] == 27 || MobTargetCard[1] == 127)) // 삼위일체3 - 피해3, 모든 적 약화3
		{
			CurrentCost -= MobTargetCost[1];
			OnCombo_Triangle_CostDown1 = true;
			if (MobTargetCard[1] == 27)
			{
				FastAttacking(1, 3);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackWeakening += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackWeakening += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackWeakening += 3;
							Enemy->UpdateNagativeEffect();
						}
					}
				}

				UpdateCost();
				return true;
			}
			else
			{
				FastAttacking(1, 4);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackWeakening += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackWeakening += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackWeakening += 4;
							Enemy->UpdateNagativeEffect();
						}
					}
				}

				UpdateCost();
				return true;
			}
		}

		if (MobTargetCard[1] == 52) // 카드가 전투휴식이면
		{
			CurrentCost -= MobTargetCost[1];
			CurrentCost += 1;

			UpdateCost();
			DrawCard(1);
			return true;
		}
		else if (MobTargetCard[1] == 152) // 카드가 전투휴식+면
		{
			CurrentCost -= MobTargetCost[1];
			CurrentCost += 2;

			UpdateCost();
			DrawCard(1);
			return true;
		}

		if (MobTargetCard[1] == 54 || MobTargetCard[1] == 154) // 카드가 각성이면
		{
			CurrentCost -= MobTargetCost[1];
			ChangeHandUpgrade();
			UpdateCost();
			return true;
		}




		if (MobTargetCard[1] == 81) // 카드가 착란이면
		{
			CurrentCost -= MobTargetCost[1];
			BeDelirium = true;
			UpdateCost();
			HandCard.RemoveSingle(81);
			return true;
		}

		if (MobTargetCard[1] == 82) // 카드가 기본기 연마이면
		{
			CurrentCost -= MobTargetCost[1];
			BeGrinding = true;
			UpdateCost();
			HandCard.RemoveSingle(82);
			return true;
		}

		if (MobTargetCard[1] == 83) // 카드가 광기발현이면
		{
			CurrentCost -= MobTargetCost[1];
			BeMadness = true;
			MyPlayer->BeMadness = true;
			UpdateCost();
			HandCard.RemoveSingle(83);
			return true;
		}

		if (MobTargetCard[1] == 84) // 카드가 연격의 대가이면
		{
			CurrentCost -= MobTargetCost[1];
			BeCombination = true;
			UpdateCost();
			HandCard.RemoveSingle(84);
			return true;
		}

		//철괴
		if (MobTargetCard[1] == 11 || MobTargetCard[1] == 111) // 카드가 철괴면
		{
			if (Attack[1] == false) // 공격 시도 안한 상태면
			{
				MyPlayer->Ironmode = true;

				if (MobTargetCard[1] == 111)
				{
					IronHeal = true;
				}
			}
			else
			{
				FString sStr = FString::Printf(TEXT("이미 공격을 시도했습니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
				RecoverDragingCard();
				return false;
			}
		}

		if (ScentMob[0] && !Attack[1]) // 향기 상태인 몹이 있고, 선제공격 턴이면
		{
			if (!ScentMob[1]) // 몹1이 향기상태가 아니면
			{
				FString sStr = FString::Printf(TEXT("향기 상태인 적이 존재합니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
				RecoverDragingCard();
				return false;
			}
		}

		if (MyPlayer->GetCharmed[0] && !Attack[1]) // 매혹 상태인 몹이 있고, 선제공격 턴이면
		{
			if (!MyPlayer->GetCharmed[1]) // 몹1이 매혹상태가 아니면
			{
				FString sStr = FString::Printf(TEXT("매혹 상태인 적이 존재합니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
				RecoverDragingCard();
				return false;
			}
		}

		if (Attack[1] && !Attack[2]) // 패링 턴이면
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr) // 다른 몹 중에 듀라한 힘 겨루기 모드 중이면
				{
					if (Enemy->ActorHasTag("Enemy2") && Enemy->ActorHasTag("Durahan") && Enemy->DurahanParryingMode)
					{
						FString sStr = FString::Printf(TEXT("듀라한과 힘겨루기 중 입니다"));
						Text_Notify->SetText(FText::FromString(sStr));
						PlayAnimation(Anim_Message);
						RecoverDragingCard();
						return false;
					}
					else if (Enemy->ActorHasTag("Enemy3") && Enemy->ActorHasTag("Durahan") && Enemy->DurahanParryingMode)
					{
						FString sStr = FString::Printf(TEXT("듀라한과 힘겨루기 중 입니다"));
						Text_Notify->SetText(FText::FromString(sStr));
						PlayAnimation(Anim_Message);
						RecoverDragingCard();
						return false;
					}
				}
			}
		}

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1"))
				{
					if (Attack[1] == false)
					{
						Enemy->ChangeWidgetImage(MobTargetCard[1], 1);
						Attack[1] = true;
						MobDone[1] = 1;
						CurrentCost -= MobTargetCost[1];

						if (Breathe[0] == true)
						{
							Enemy->ChangeBreatheEffect(true);
						}
						ChangeAttackImage(1, MobTargetCard[1], 1);
						CardNumByAttackOrder[1] = MobTargetCard[1];
						ComboOnByAttackOrder_Extinction[1] = OnCombo_Extinction_CostDown1;
						ComboOnByAttackOrder_Triangle[1] = OnCombo_Triangle_CostDown1;
					}
					else if (Attack[2] == false)
					{
						Enemy->ChangeWidgetImage(MobTargetCard[1], 2);
						Attack[2] = true;
						MobDone[1] = 2;
						CurrentCost -= MobTargetCost[1];

						if (Breathe[1] == true)
						{
							Enemy->ChangeBreatheEffect(true);
						}
						ChangeAttackImage(2, MobTargetCard[1], 1);
						CardNumByAttackOrder[2] = MobTargetCard[1];
						ComboOnByAttackOrder_Extinction[2] = OnCombo_Extinction_CostDown1;
						ComboOnByAttackOrder_Triangle[2] = OnCombo_Triangle_CostDown1;
					}
					else if (Attack[3] == false)
					{
						Enemy->ChangeWidgetImage(MobTargetCard[1], 3);
						Attack[3] = true;
						MobDone[1] = 3;
						CurrentCost -= MobTargetCost[1];

						if (Breathe[2] == true)
						{
							Enemy->ChangeBreatheEffect(true);
						}
						ChangeAttackImage(3, MobTargetCard[1], 1);
						CardNumByAttackOrder[3] = MobTargetCard[1];
						ComboOnByAttackOrder_Extinction[3] = OnCombo_Extinction_CostDown1;
						ComboOnByAttackOrder_Triangle[3] = OnCombo_Triangle_CostDown1;
					}
					else
					{
						RecoverDragingCard();
						return false;
					}
				}
			}
		}

		if ((MobTargetCard[1] == 32 || MobTargetCard[1] == 132 || MobTargetCard[1] == 35 || MobTargetCard[1] == 135) && OnCombo_Extinction_CostDown1)
		{
			OnCombo_Extinction_CostDown1 = false;
		}

		if (MobTargetCard[1] == 31 || MobTargetCard[1] == 50 || MobTargetCard[1] == 51 || MobTargetCard[1] == 131 || MobTargetCard[1] == 150 || MobTargetCard[1] == 151)
		{
			OnCombo_Extinction_CostDown1 = true;
		}

		UpdateCost();
		ChangeHandCost();
		ChangeTurnText();
		DragingCardNum = 0;
		return true;
	}

	if (200 >= UKismetMathLibrary::Distance2D(Mob2, DragWindowOffsetResult) && MobDone[2] == 0 && EnemyLife[2] == true && !(EnemyHealthbar2->IamWarlock))
	{
		FString Thistag = DragWidgetResult->EnemyTag;
		MobTargetCard[2] = FCString::Atof(*Thistag);
		FString CostConfirm = DragWidgetResult->CostTag;
		MobTargetCost[2] = FCString::Atof(*CostConfirm);
		FString FixCostConfirm = DragWidgetResult->FixCostTag;
		MobTargetFixCost[2] = FCString::Atof(*FixCostConfirm);
		FString KeyWordinConfirm = DragWidgetResult->KeyWordinTag;
		int KeyWordinCard = FCString::Atof(*FixCostConfirm); // 키워드 들어가있으면 1, 아니면 0
		bool IsComboCardConfirm = DragWidgetResult->IsComboCard;

		bool CanIt = CardDragControl(MobTargetCard[2]);
		if (!CanIt)
		{
			FString sStr = FString::Printf(TEXT("호환되지 않는 카드입니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
			RecoverDragingCard();
			return false;
		}

		if (MyPlayer->Ironmode) // 철괴 모드이면
		{
			FString sStr = FString::Printf(TEXT("철괴를 사용하였습니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
			RecoverDragingCard();
			return false;
		}

		//코스트 증가 효과 적용(연격의 대가)
		if (!IsComboCardConfirm && BeCombination) // 콤보 카드가 아니면서 연격의 대가 발동 중이면
		{
			MobTargetCost[2] += 1;
		}

		//위기 모면 효과 적용
		if (MobTargetCard[2] == 49 || MobTargetCard[2] == 149)
		{
			if (EnemyLife[1])
			{
				MobTargetCost[2] -= 1;
				if (MobTargetCost[2] < 0)
				{
					MobTargetCost[2] = 0;
				}
			}
			if (EnemyLife[2])
			{
				MobTargetCost[2] -= 1;
				if (MobTargetCost[2] < 0)
				{
					MobTargetCost[2] = 0;
				}
			}
			if (EnemyLife[3])
			{
				MobTargetCost[2] -= 1;
				if (MobTargetCost[2] < 0)
				{
					MobTargetCost[2] = 0;
				}
			}
		}

		//코스트 감소 효과 적용
		if ((MobTargetCard[2] == 31 || MobTargetCard[2] == 32 || MobTargetCard[2] == 35 || MobTargetCard[2] == 50 || MobTargetCard[2] == 51 || MobTargetCard[2] == 131 || MobTargetCard[2] == 132 || MobTargetCard[2] == 135 || MobTargetCard[2] == 150 || MobTargetCard[2] == 151) && OnCombo_Extinction_CostDown1)
		{
			MobTargetCost[2] -= 1;
			if (MobTargetCost[2] < 0)
			{
				MobTargetCost[2] = 0;
			}
		}

		//코스트 감소 효과 적용 (삼위일체)
		if ((MobTargetCard[2] == 25 || MobTargetCard[2] == 26 || MobTargetCard[2] == 27 || MobTargetCard[2] == 32 || MobTargetCard[2] == 125 || MobTargetCard[2] == 126 || MobTargetCard[2] == 127 || MobTargetCard[2] == 132) && OnCombo_Triangle_CostDown1)
		{
			MobTargetCost[2] -= 3;
			if (MobTargetCost[2] < 0)
			{
				MobTargetCost[2] = 0;
			}
		}

		if (KeyWordinCard == 0 && BeGrinding) // 키워드 x, 기본기 연마 시
		{
			MobTargetCost[2] -= 1;
			if (MobTargetCost[2] < 0)
			{
				MobTargetCost[2] = 0;
			}
		}

		//코스트 고정 효과 적용
		if (MobTargetFixCost[2] >= 0)
		{
			MobTargetCost[2] = MobTargetFixCost[2];
			if (KeyWordinCard == 0 && BeGrinding) // 키워드 x, 기본기 연마 시
			{
				MobTargetCost[2] -= 1;
				if (MobTargetCost[2] < 0)
				{
					MobTargetCost[2] = 0;
				}
			}
		}

		if (CurrentCost - MobTargetCost[2] < 0)
		{
			FString sStr = FString::Printf(TEXT("코스트가 부족합니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
			MobTargetCost[2] = 0;
			RecoverDragingCard();
			return false;
		}

		if (MobTargetCard[2] == 23) // 카드가 대기만성이면
		{
			CurrentCost -= MobTargetCost[2];
			bool gsd = ChronicitySelect();
			if (gsd)
			{
				BeChronicity = true;
				AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
				MyController->ShowDumpDeckWidget(ChronicityDeck);
			}
			else
			{
				FString sStr = FString::Printf(TEXT("연쇄 카드가 존재하지 않습니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
			}
			UpdateCost();
			return true;
		}
		else if (MobTargetCard[2] == 123) // 카드가 대기만성+면
		{
			CurrentCost -= MobTargetCost[2];
			CurrentCost += 1;

			bool gsd = ChronicitySelect();
			if (gsd)
			{
				BeChronicity = true;
				AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
				MyController->ShowDumpDeckWidget(ChronicityDeck);
			}
			else
			{
				FString sStr = FString::Printf(TEXT("연쇄 카드가 존재하지 않습니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
			}
			UpdateCost();
			return true;
		}

		if (MobTargetCard[2] == 33) // 카드가 명상이면
		{
			CurrentCost -= MobTargetCost[2];
			MyPlayer->GetStackCorrosion = 0;
			MyPlayer->GetStackWeakening = 0;
			MyPlayer->GetStackBlooding = 0;
			MyPlayer->GetCharmed = { false, false, false, false };
			MyHealthbar0->UpdateNagativeEffect(MyPlayer->GetStackCorrosion, MyPlayer->GetStackWeakening, MyPlayer->GetStackBlooding);
			UpdateCost();
			return true;
		}
		else if (MobTargetCard[2] == 133) // 카드가 명상+면
		{
			CurrentCost -= MobTargetCost[2];
			MyPlayer->GetStackCorrosion = 0;
			MyPlayer->GetStackWeakening = 0;
			MyPlayer->GetStackBlooding = 0;
			MyPlayer->GetCharmed = { false, false, false, false };
			MyHealthbar0->UpdateNagativeEffect(MyPlayer->GetStackCorrosion, MyPlayer->GetStackWeakening, MyPlayer->GetStackBlooding);
			UpdateCost();
			DrawCard(1);
			return true;
		}

		if (MobTargetCard[2] == 37 || MobTargetCard[2] == 137) // 카드가 무소유면 - 손패 버리고 버린 만큼 뽑기
		{
			CurrentCost -= MobTargetCost[2];
			int rts1 = CardBox->GetChildrenCount(); // 손패 카드 수
			int rts2 = CardBox2->GetChildrenCount();
			for (int x = 0; x < rts1; x++)
			{
				CardBox->RemoveChildAt(0); // 손패 카드들 삭제
			}
			for (int x = 0; x < rts2; x++)
			{
				CardBox2->RemoveChildAt(0); // 손패 카드들 삭제
			}
			DrawCard(rts1 + rts2);
			UpdateCost();
			return true;
		}

		if (MobTargetCard[2] == 44) // 카드가 날 세우기면
		{
			CurrentCost -= MobTargetCost[2];
			MyPlayer->AttackUp += 2;
			UpdateCost();
			return true;
		}
		else if (MobTargetCard[2] == 144) // 카드가 날 세우기+면
		{
			CurrentCost -= MobTargetCost[2];
			MyPlayer->AttackUp += 3;
			UpdateCost();
			return true;
		}

		if ((MobTargetCard[2] == 31 || MobTargetCard[2] == 131)) // 피해6, 8 오의:절멸1
		{
			CurrentCost -= MobTargetCost[2];
			OnCombo_Extinction_CostDown1 = true;
			if (MyPlayer->ExtinctionWholeAttack) // 광역 공격을 실시하면
			{
				MyPlayer->ExtinctionWholeAttack = false;
				MyPlayer->ExtinctionWholeAttackDone = true;
			}
			if (MobTargetCard[2] == 31)
			{
				FastAttacking(2, 6);
			}
			else
			{
				FastAttacking(2, 8);
			}
			UpdateCost();
			return true;
		}

		if ((MobTargetCard[2] == 50 || MobTargetCard[2] == 150)) // 피해2, 피해4 오의:절멸2 카드 선택
		{
			CurrentCost -= MobTargetCost[2];
			OnCombo_Extinction_CostDown1 = true;
			if (MyPlayer->ExtinctionWholeAttack) // 광역 공격을 실시하면
			{
				MyPlayer->ExtinctionWholeAttack = false;
				MyPlayer->ExtinctionWholeAttackDone = true;
			}
			if (MobTargetCard[2] == 50)
			{
				FastAttacking(2, 2);
			}
			else
			{
				FastAttacking(2, 4);
			}

			TArray<int32> ThisHand = HandCard;

			ThisHand.RemoveSingle(MobTargetCard[2]);

			if (ThisHand.Num() > 0)
			{
				AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
				if (PlayerController != nullptr)
				{
					PlayerController->ShowHandDeckWidget(ThisHand);
				}
			}
			UpdateCost();
			return true;
		}

		if (MobTargetCard[2] == 51 || MobTargetCard[2] == 151) // 카드가 오의:절멸3이면
		{
			CurrentCost -= MobTargetCost[2];
			OnCombo_Extinction_CostDown1 = true;

			MyPlayer->ExtinctionWholeAttack = true;
			OnCombo_Extinction_CostDown1 = true;
			UpdateCost();
			return true;
		}

		if ((MobTargetCard[2] == 25 || MobTargetCard[2] == 125)) // 삼위일체1 - 피해3, 모든 적 출혈3
		{
			CurrentCost -= MobTargetCost[2];
			OnCombo_Triangle_CostDown1 = true;
			if (MobTargetCard[2] == 25)
			{
				FastAttacking(2, 3);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackBlooding += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackBlooding += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackBlooding += 3;
							Enemy->UpdateNagativeEffect();
						}
					}
				}
				UpdateCost();
				return true;
			}
			else
			{
				FastAttacking(2, 4);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackBlooding += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackBlooding += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackBlooding += 4;
							Enemy->UpdateNagativeEffect();
						}
					}
				}
				UpdateCost();
				return true;
			}
		}

		if ((MobTargetCard[2] == 26 || MobTargetCard[2] == 126)) // 삼위일체2 - 피해3, 모든 적 부식3
		{
			CurrentCost -= MobTargetCost[2];
			OnCombo_Triangle_CostDown1 = true;
			if (MobTargetCard[2] == 26)
			{
				FastAttacking(2, 3);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackCorrosion += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackCorrosion += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackCorrosion += 3;
							Enemy->UpdateNagativeEffect();
						}
					}
				}
				UpdateCost();
				return true;
			}
			else
			{
				FastAttacking(2, 4);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackCorrosion += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackCorrosion += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackCorrosion += 4;
							Enemy->UpdateNagativeEffect();
						}
					}
				}
				UpdateCost();
				return true;
			}
		}

		if ((MobTargetCard[2] == 27 || MobTargetCard[2] == 127)) // 삼위일체3 - 피해3, 모든 적 약화3
		{
			CurrentCost -= MobTargetCost[2];
			OnCombo_Triangle_CostDown1 = true;
			if (MobTargetCard[2] == 27)
			{
				FastAttacking(2, 3);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackWeakening += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackWeakening += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackWeakening += 3;
							Enemy->UpdateNagativeEffect();
						}
					}
				}
				UpdateCost();
				return true;
			}
			else
			{
				FastAttacking(2, 4);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackWeakening += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackWeakening += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackWeakening += 4;
							Enemy->UpdateNagativeEffect();
						}
					}
				}
				UpdateCost();
				return true;
			}
		}

		if (MobTargetCard[2] == 52) // 카드가 전투휴식이면
		{
			CurrentCost -= MobTargetCost[2];
			CurrentCost += 1;

			UpdateCost();
			DrawCard(1);
			return true;
		}
		else if (MobTargetCard[2] == 152) // 카드가 전투휴식+면
		{
			CurrentCost -= MobTargetCost[2];
			CurrentCost += 2;

			UpdateCost();
			DrawCard(1);
			return true;
		}

		if (MobTargetCard[2] == 54 || MobTargetCard[2] == 154) // 카드가 각성이면
		{
			CurrentCost -= MobTargetCost[2];
			ChangeHandUpgrade();
			UpdateCost();
			return true;
		}



		if (MobTargetCard[2] == 81) // 카드가 착란이면
		{
			CurrentCost -= MobTargetCost[2];
			BeDelirium = true;

			UpdateCost();
			return true;
		}

		if (MobTargetCard[2] == 82) // 카드가 기본기 연마이면
		{
			CurrentCost -= MobTargetCost[2];
			BeGrinding = true;

			UpdateCost();
			return true;
		}

		if (MobTargetCard[2] == 83) // 카드가 광기발현이면
		{
			CurrentCost -= MobTargetCost[2];
			BeMadness = true;
			MyPlayer->BeMadness = true;

			UpdateCost();
			return true;
		}

		if (MobTargetCard[2] == 84) // 카드가 연격의 대가이면
		{
			CurrentCost -= MobTargetCost[2];
			BeCombination = true;

			UpdateCost();
			return true;
		}

		//철괴
		if (MobTargetCard[2] == 11 || MobTargetCard[2] == 111) // 카드가 철괴면
		{
			if (Attack[1] == false) // 공격 시도 안한 상태면
			{
				MyPlayer->Ironmode = true;

				if (MobTargetCard[2] == 111)
				{
					IronHeal = true;
				}
			}
			else
			{
				FString sStr = FString::Printf(TEXT("이미 공격을 시도했습니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
				RecoverDragingCard();
				return false;
			}
		}

		if (ScentMob[0] && !Attack[1]) // 향기 상태인 몹이 있고, 선제공격 턴이면
		{
			if (!ScentMob[2]) // 몹2가 향기상태가 아니면
			{
				FString sStr = FString::Printf(TEXT("향기 상태인 적이 존재합니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
				RecoverDragingCard();
				return false;
			}
		}

		if (MyPlayer->GetCharmed[0] && !Attack[1]) // 매혹 상태인 몹이 있고, 선제공격 턴이면
		{
			if (!MyPlayer->GetCharmed[2]) // 몹2이 매혹상태가 아니면
			{
				FString sStr = FString::Printf(TEXT("매혹 상태인 적이 존재합니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
				RecoverDragingCard();
				return false;
			}
		}

		if (Attack[1] && !Attack[2]) // 패링 턴이면
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr) // 다른 몹 중에 듀라한 힘 겨루기 모드 중이면
				{
					if (Enemy->ActorHasTag("Enemy1") && Enemy->ActorHasTag("Durahan") && Enemy->DurahanParryingMode)
					{
						FString sStr = FString::Printf(TEXT("듀라한과 힘겨루기 중 입니다"));
						Text_Notify->SetText(FText::FromString(sStr));
						PlayAnimation(Anim_Message);
						RecoverDragingCard();
						return false;
					}
					else if (Enemy->ActorHasTag("Enemy3") && Enemy->ActorHasTag("Durahan") && Enemy->DurahanParryingMode)
					{
						FString sStr = FString::Printf(TEXT("듀라한과 힘겨루기 중 입니다"));
						Text_Notify->SetText(FText::FromString(sStr));
						PlayAnimation(Anim_Message);
						RecoverDragingCard();
						return false;
					}
				}
			}
		}

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy2"))
				{
					if (Attack[1] == false)
					{
						Enemy->ChangeWidgetImage(MobTargetCard[2], 1);
						Attack[1] = true;
						MobDone[2] = 1;
						CurrentCost -= MobTargetCost[2];

						if (Breathe[0] == true)
						{
							Enemy->ChangeBreatheEffect(true);
						}
						ChangeAttackImage(1, MobTargetCard[2], 2);
						CardNumByAttackOrder[1] = MobTargetCard[2];
						ComboOnByAttackOrder_Extinction[1] = OnCombo_Extinction_CostDown1;
						ComboOnByAttackOrder_Triangle[1] = OnCombo_Triangle_CostDown1;
					}
					else if (Attack[2] == false)
					{
						Enemy->ChangeWidgetImage(MobTargetCard[2], 2);
						Attack[2] = true;
						MobDone[2] = 2;
						CurrentCost -= MobTargetCost[2];

						if (Breathe[1] == true)
						{
							Enemy->ChangeBreatheEffect(true);
						}
						ChangeAttackImage(2, MobTargetCard[2], 2);
						CardNumByAttackOrder[2] = MobTargetCard[2];
						ComboOnByAttackOrder_Extinction[2] = OnCombo_Extinction_CostDown1;
						ComboOnByAttackOrder_Triangle[2] = OnCombo_Triangle_CostDown1;
					}
					else if (Attack[3] == false)
					{
						Enemy->ChangeWidgetImage(MobTargetCard[2], 3);
						Attack[3] = true;
						MobDone[2] = 3;
						CurrentCost -= MobTargetCost[2];

						if (Breathe[2] == true)
						{
							Enemy->ChangeBreatheEffect(true);
						}
						ChangeAttackImage(3, MobTargetCard[2], 2);
						CardNumByAttackOrder[3] = MobTargetCard[2];
						ComboOnByAttackOrder_Extinction[3] = OnCombo_Extinction_CostDown1;
						ComboOnByAttackOrder_Triangle[3] = OnCombo_Triangle_CostDown1;
					}
					else
					{
						RecoverDragingCard();
						return false;
					}
				}
			}
		}

		if ((MobTargetCard[2] == 32 || MobTargetCard[2] == 132 || MobTargetCard[2] == 35 || MobTargetCard[2] == 135) && OnCombo_Extinction_CostDown1)
		{
			OnCombo_Extinction_CostDown1 = false;
		}

		if (MobTargetCard[2] == 31 || MobTargetCard[2] == 50 || MobTargetCard[2] == 51 || MobTargetCard[2] == 131 || MobTargetCard[2] == 150 || MobTargetCard[2] == 151)
		{
			OnCombo_Extinction_CostDown1 = true;
		}

		UpdateCost();
		ChangeHandCost();
		ChangeTurnText();
		DragingCardNum = 0;
		return true;
	}

	if (200 >= UKismetMathLibrary::Distance2D(Mob3, DragWindowOffsetResult) && MobDone[3] == 0 && EnemyLife[3] == true)
	{
		FString Thistag = DragWidgetResult->EnemyTag;
		MobTargetCard[3] = FCString::Atof(*Thistag);
		FString CostConfirm = DragWidgetResult->CostTag;
		MobTargetCost[3] = FCString::Atof(*CostConfirm);
		FString FixCostConfirm = DragWidgetResult->FixCostTag;
		MobTargetFixCost[3] = FCString::Atof(*FixCostConfirm);
		FString KeyWordinConfirm = DragWidgetResult->KeyWordinTag;
		int KeyWordinCard = FCString::Atof(*FixCostConfirm); // 키워드 들어가있으면 1, 아니면 0
		bool IsComboCardConfirm = DragWidgetResult->IsComboCard;

		bool CanIt = CardDragControl(MobTargetCard[3]);
		if (!CanIt)
		{
			FString sStr = FString::Printf(TEXT("호환되지 않는 카드입니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
			RecoverDragingCard();
			return false;
		}

		if (MyPlayer->Ironmode) // 철괴 모드이면
		{
			FString sStr = FString::Printf(TEXT("철괴를 사용하였습니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
			RecoverDragingCard();
			return false;
		}

		//코스트 증가 효과 적용(연격의 대가)
		if (!IsComboCardConfirm && BeCombination) // 콤보 카드가 아니면서 연격의 대가 발동 중이면
		{
			MobTargetCost[3] += 1;
		}

		//위기 모면 효과 적용
		if (MobTargetCard[3] == 49 || MobTargetCard[3] == 149)
		{
			if (EnemyLife[1])
			{
				MobTargetCost[3] -= 1;
				if (MobTargetCost[3] < 0)
				{
					MobTargetCost[3] = 0;
				}
			}
			if (EnemyLife[2])
			{
				MobTargetCost[3] -= 1;
				if (MobTargetCost[3] < 0)
				{
					MobTargetCost[3] = 0;
				}
			}
			if (EnemyLife[3])
			{
				MobTargetCost[3] -= 1;
				if (MobTargetCost[3] < 0)
				{
					MobTargetCost[3] = 0;
				}
			}
		}

		//코스트 감소 효과 적용
		if ((MobTargetCard[3] == 31 || MobTargetCard[3] == 32 || MobTargetCard[3] == 35 || MobTargetCard[3] == 50 || MobTargetCard[3] == 51 || MobTargetCard[3] == 131 || MobTargetCard[3] == 132 || MobTargetCard[3] == 135 || MobTargetCard[3] == 150 || MobTargetCard[3] == 151) && OnCombo_Extinction_CostDown1)
		{
			MobTargetCost[3] -= 1;
			if (MobTargetCost[3] < 0)
			{
				MobTargetCost[3] = 0;
			}
		}

		//코스트 감소 효과 적용 (삼위일체)
		if ((MobTargetCard[3] == 25 || MobTargetCard[3] == 26 || MobTargetCard[3] == 27 || MobTargetCard[3] == 32 || MobTargetCard[3] == 125 || MobTargetCard[3] == 126 || MobTargetCard[3] == 127 || MobTargetCard[3] == 132) && OnCombo_Triangle_CostDown1)
		{
			MobTargetCost[3] -= 3;
			if (MobTargetCost[3] < 0)
			{
				MobTargetCost[3] = 0;
			}
		}

		if (KeyWordinCard == 0 && BeGrinding) // 키워드 x, 기본기 연마 시
		{
			MobTargetCost[3] -= 1;
			if (MobTargetCost[3] < 0)
			{
				MobTargetCost[3] = 0;
			}
		}

		//코스트 고정 효과 적용
		if (MobTargetFixCost[3] >= 0)
		{
			MobTargetCost[3] = MobTargetFixCost[3];
			if (KeyWordinCard == 0 && BeGrinding) // 키워드 x, 기본기 연마 시
			{
				MobTargetCost[3] -= 1;
				if (MobTargetCost[3] < 0)
				{
					MobTargetCost[3] = 0;
				}
			}
		}

		if (CurrentCost - MobTargetCost[3] < 0)
		{
			FString sStr = FString::Printf(TEXT("코스트가 부족합니다"));
			Text_Notify->SetText(FText::FromString(sStr));
			PlayAnimation(Anim_Message);
			MobTargetCost[3] = 0;
			RecoverDragingCard();
			return false;
		}

		//신속
		if (MobTargetCard[3] == 23) // 카드가 대기만성이면
		{
			CurrentCost -= MobTargetCost[3];
			bool gsd = ChronicitySelect();
			if (gsd)
			{
				BeChronicity = true;
				AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
				MyController->ShowDumpDeckWidget(ChronicityDeck);
			}
			else
			{
				FString sStr = FString::Printf(TEXT("연쇄 카드가 존재하지 않습니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
			}
			UpdateCost();
			return true;
		}
		else if (MobTargetCard[3] == 123) // 카드가 대기만성+면
		{
			CurrentCost -= MobTargetCost[3];
			CurrentCost += 1;

			bool gsd = ChronicitySelect();
			if (gsd)
			{
				BeChronicity = true;
				AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
				MyController->ShowDumpDeckWidget(ChronicityDeck);
			}
			else
			{
				FString sStr = FString::Printf(TEXT("연쇄 카드가 존재하지 않습니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
			}
			UpdateCost();
			return true;
		}

		if (MobTargetCard[3] == 33) // 카드가 명상이면
		{
			CurrentCost -= MobTargetCost[3];
			MyPlayer->GetStackCorrosion = 0;
			MyPlayer->GetStackWeakening = 0;
			MyPlayer->GetStackBlooding = 0;
			MyPlayer->GetCharmed = { false, false, false, false };
			MyHealthbar0->UpdateNagativeEffect(MyPlayer->GetStackCorrosion, MyPlayer->GetStackWeakening, MyPlayer->GetStackBlooding);
			UpdateCost();
			return true;
		}
		else if (MobTargetCard[3] == 133) // 카드가 명상+면
		{
			CurrentCost -= MobTargetCost[3];
			MyPlayer->GetStackCorrosion = 0;
			MyPlayer->GetStackWeakening = 0;
			MyPlayer->GetStackBlooding = 0;
			MyPlayer->GetCharmed = { false, false, false, false };
			MyHealthbar0->UpdateNagativeEffect(MyPlayer->GetStackCorrosion, MyPlayer->GetStackWeakening, MyPlayer->GetStackBlooding);
			UpdateCost();
			DrawCard(1);
			return true;
		}

		if (MobTargetCard[3] == 37 || MobTargetCard[3] == 137) // 카드가 무소유면 - 손패 버리고 버린 만큼 뽑기
		{
			CurrentCost -= MobTargetCost[3];
			int rts1 = CardBox->GetChildrenCount(); // 손패 카드 수
			int rts2 = CardBox2->GetChildrenCount();
			for (int x = 0; x < rts1; x++)
			{
				CardBox->RemoveChildAt(0); // 손패 카드들 삭제
			}
			for (int x = 0; x < rts2; x++)
			{
				CardBox2->RemoveChildAt(0); // 손패 카드들 삭제
			}
			DrawCard(rts1 + rts2);
			UpdateCost();
			return true;
		}

		if (MobTargetCard[3] == 44) // 카드가 날 세우기면
		{
			CurrentCost -= MobTargetCost[3];
			MyPlayer->AttackUp += 2;
			UpdateCost();
			return true;
		}
		else if (MobTargetCard[3] == 144) // 카드가 날 세우기+면
		{
			CurrentCost -= MobTargetCost[3];
			MyPlayer->AttackUp += 3;
			UpdateCost();
			return true;
		}

		if ((MobTargetCard[3] == 31 || MobTargetCard[3] == 131)) // 피해6, 8 오의:절멸1
		{
			CurrentCost -= MobTargetCost[3];
			OnCombo_Extinction_CostDown1 = true;

			if (MyPlayer->ExtinctionWholeAttack) // 광역 공격을 실시하면
			{
				MyPlayer->ExtinctionWholeAttack = false;
				MyPlayer->ExtinctionWholeAttackDone = true;
			}
			if (MobTargetCard[3] == 31)
			{
				FastAttacking(3, 6);
			}
			else
			{
				FastAttacking(3, 8);
			}
			UpdateCost();
			return true;
		}

		if ((MobTargetCard[3] == 50 || MobTargetCard[3] == 150)) // 피해2, 피해4 오의:절멸2 카드 선택
		{
			CurrentCost -= MobTargetCost[3];
			OnCombo_Extinction_CostDown1 = true;

			if (MyPlayer->ExtinctionWholeAttack) // 광역 공격을 실시하면
			{
				MyPlayer->ExtinctionWholeAttack = false;
				MyPlayer->ExtinctionWholeAttackDone = true;
			}
			if (MobTargetCard[3] == 50)
			{
				FastAttacking(3, 2);
			}
			else
			{
				FastAttacking(3, 4);
			}

			TArray<int32> ThisHand = HandCard;

			ThisHand.RemoveSingle(MobTargetCard[3]);

			if (ThisHand.Num() > 0)
			{
				AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
				if (PlayerController != nullptr)
				{
					PlayerController->ShowHandDeckWidget(ThisHand);
				}
			}
			UpdateCost();
			return true;
		}

		if (MobTargetCard[3] == 51 || MobTargetCard[3] == 151) // 카드가 오의:절멸3이면
		{
			CurrentCost -= MobTargetCost[3];
			OnCombo_Extinction_CostDown1 = true;

			MyPlayer->ExtinctionWholeAttack = true;
			OnCombo_Extinction_CostDown1 = true;
			UpdateCost();
			return true;
		}

		if ((MobTargetCard[3] == 25 || MobTargetCard[3] == 125)) // 삼위일체1 - 피해3, 모든 적 출혈3
		{
			CurrentCost -= MobTargetCost[3];
			OnCombo_Triangle_CostDown1 = true;
			if (MobTargetCard[3] == 25)
			{
				FastAttacking(3, 3);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackBlooding += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackBlooding += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackBlooding += 3;
							Enemy->UpdateNagativeEffect();
						}
					}
				}
				UpdateCost();
				return true;
			}
			else
			{
				FastAttacking(3, 4);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackBlooding += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackBlooding += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackBlooding += 4;
							Enemy->UpdateNagativeEffect();
						}
					}
				}
				UpdateCost();
				return true;
			}
		}

		if ((MobTargetCard[3] == 26 || MobTargetCard[3] == 126)) // 삼위일체2 - 피해3, 모든 적 부식3
		{
			CurrentCost -= MobTargetCost[3];
			OnCombo_Triangle_CostDown1 = true;
			if (MobTargetCard[3] == 26)
			{
				FastAttacking(3, 3);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackCorrosion += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackCorrosion += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackCorrosion += 3;
							Enemy->UpdateNagativeEffect();
						}
					}
				}
				UpdateCost();
				return true;
			}
			else
			{
				FastAttacking(3, 4);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackCorrosion += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackCorrosion += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackCorrosion += 4;
							Enemy->UpdateNagativeEffect();
						}
					}
				}
				UpdateCost();
				return true;
			}
		}

		if ((MobTargetCard[3] == 27 || MobTargetCard[3] == 127)) // 삼위일체3 - 피해3, 모든 적 약화3
		{
			CurrentCost -= MobTargetCost[3];
			OnCombo_Triangle_CostDown1 = true;
			if (MobTargetCard[3] == 27)
			{
				FastAttacking(3, 3);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackWeakening += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackWeakening += 3;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackWeakening += 3;
							Enemy->UpdateNagativeEffect();
						}
					}
				}
				UpdateCost();
				return true;
			}
			else
			{
				FastAttacking(3, 4);

				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;
					if (Enemy != nullptr)
					{
						if (EnemyLife[1] && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->GetStackWeakening += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[2] && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->GetStackWeakening += 4;
							Enemy->UpdateNagativeEffect();
						}
						else if (EnemyLife[3] && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->GetStackWeakening += 4;
							Enemy->UpdateNagativeEffect();
						}
					}
				}
				UpdateCost();
				return true;
			}
		}

		if (MobTargetCard[3] == 52) // 카드가 전투휴식이면
		{
			CurrentCost -= MobTargetCost[3];
			CurrentCost += 1;

			UpdateCost();
			DrawCard(1);
			return true;
		}
		else if (MobTargetCard[3] == 152) // 카드가 전투휴식+면
		{
			CurrentCost -= MobTargetCost[3];
			CurrentCost += 2;

			UpdateCost();
			DrawCard(1);
			return true;
		}

		if (MobTargetCard[3] == 54 || MobTargetCard[3] == 154) // 카드가 각성이면
		{
			CurrentCost -= MobTargetCost[3];
			ChangeHandUpgrade();
			UpdateCost();
			return true;
		}



		if (MobTargetCard[3] == 81) // 카드가 착란이면
		{
			CurrentCost -= MobTargetCost[3];
			BeDelirium = true;

			UpdateCost();
			return true;
		}

		if (MobTargetCard[3] == 82) // 카드가 기본기 연마이면
		{
			CurrentCost -= MobTargetCost[3];
			BeGrinding = true;

			UpdateCost();
			return true;
		}

		if (MobTargetCard[3] == 83) // 카드가 광기발현이면
		{
			CurrentCost -= MobTargetCost[3];
			BeMadness = true;
			MyPlayer->BeMadness = true;

			UpdateCost();
			return true;
		}

		if (MobTargetCard[3] == 84) // 카드가 연격의 대가이면
		{
			CurrentCost -= MobTargetCost[3];
			BeCombination = true;

			UpdateCost();
			return true;
		}

		//철괴
		if (MobTargetCard[3] == 11 || MobTargetCard[3] == 111) // 카드가 철괴면
		{
			if (Attack[1] == false) // 공격 시도 안한 상태면
			{
				MyPlayer->Ironmode = true;

				if (MobTargetCard[3] == 111)
				{
					IronHeal = true;
				}
			}
			else
			{
				FString sStr = FString::Printf(TEXT("이미 공격을 시도했습니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
				RecoverDragingCard();
				return false;
			}
		}

		if (ScentMob[0] && !Attack[1]) // 향기 상태인 몹이 있고, 선제공격 턴이면
		{
			if (!ScentMob[3]) // 몹3이 향기상태가 아니면
			{
				FString sStr = FString::Printf(TEXT("향기 상태인 적이 존재합니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
				RecoverDragingCard();
				return false;
			}
		}

		if (MyPlayer->GetCharmed[0] && !Attack[1]) // 매혹 상태인 몹이 있고, 선제공격 턴이면
		{
			if (!MyPlayer->GetCharmed[3]) // 몹3이 매혹상태가 아니면
			{
				FString sStr = FString::Printf(TEXT("매혹 상태인 적이 존재합니다"));
				Text_Notify->SetText(FText::FromString(sStr));
				PlayAnimation(Anim_Message);
				RecoverDragingCard();
				return false;
			}
		}

		if (Attack[1] && !Attack[2]) // 패링 턴이면
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr) // 다른 몹 중에 듀라한 힘 겨루기 모드 중이면
				{
					if (Enemy->ActorHasTag("Enemy1") && Enemy->ActorHasTag("Durahan") && Enemy->DurahanParryingMode)
					{
						FString sStr = FString::Printf(TEXT("듀라한과 힘겨루기 중 입니다"));
						Text_Notify->SetText(FText::FromString(sStr));
						PlayAnimation(Anim_Message);
						RecoverDragingCard();
						return false;
					}
					else if (Enemy->ActorHasTag("Enemy2") && Enemy->ActorHasTag("Durahan") && Enemy->DurahanParryingMode)
					{
						FString sStr = FString::Printf(TEXT("듀라한과 힘겨루기 중 입니다"));
						Text_Notify->SetText(FText::FromString(sStr));
						PlayAnimation(Anim_Message);
						RecoverDragingCard();
						return false;
					}
				}
			}
		}

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy3"))
				{
					if (Attack[1] == false)
					{
						Enemy->ChangeWidgetImage(MobTargetCard[3], 1);
						Attack[1] = true;
						MobDone[3] = 1;
						CurrentCost -= MobTargetCost[3];

						if (Breathe[0] == true)
						{
							Enemy->ChangeBreatheEffect(true);
						}
						ChangeAttackImage(1, MobTargetCard[3], 3);
						CardNumByAttackOrder[1] = MobTargetCard[3];
						ComboOnByAttackOrder_Extinction[1] = OnCombo_Extinction_CostDown1;
						ComboOnByAttackOrder_Triangle[1] = OnCombo_Triangle_CostDown1;
					}
					else if (Attack[2] == false)
					{
						Enemy->ChangeWidgetImage(MobTargetCard[3], 2);
						Attack[2] = true;
						MobDone[3] = 2;
						CurrentCost -= MobTargetCost[3];

						if (Breathe[1] == true)
						{
							Enemy->ChangeBreatheEffect(true);
						}
						ChangeAttackImage(2, MobTargetCard[3], 3);
						CardNumByAttackOrder[2] = MobTargetCard[3];
						ComboOnByAttackOrder_Extinction[2] = OnCombo_Extinction_CostDown1;
						ComboOnByAttackOrder_Triangle[2] = OnCombo_Triangle_CostDown1;
					}
					else if (Attack[3] == false)
					{
						Enemy->ChangeWidgetImage(MobTargetCard[3], 3);
						Attack[3] = true;
						MobDone[3] = 3;
						CurrentCost -= MobTargetCost[3];

						if (Breathe[2] == true)
						{
							Enemy->ChangeBreatheEffect(true);
						}
						ChangeAttackImage(3, MobTargetCard[3], 3);
						CardNumByAttackOrder[3] = MobTargetCard[3];
						ComboOnByAttackOrder_Extinction[3] = OnCombo_Extinction_CostDown1;
						ComboOnByAttackOrder_Triangle[3] = OnCombo_Triangle_CostDown1;
					}
					else
					{
						RecoverDragingCard();
						return false;
					}
				}
			}
		}

		if ((MobTargetCard[3] == 32 || MobTargetCard[3] == 132 || MobTargetCard[3] == 35 || MobTargetCard[3] == 135) && OnCombo_Extinction_CostDown1)
		{
			OnCombo_Extinction_CostDown1 = false;
		}

		if (MobTargetCard[3] == 31 || MobTargetCard[3] == 50 || MobTargetCard[3] == 51 || MobTargetCard[3] == 131 || MobTargetCard[3] == 150 || MobTargetCard[3] == 151)
		{
			OnCombo_Extinction_CostDown1 = true;
		}

		UpdateCost();
		ChangeHandCost();
		ChangeTurnText();
		DragingCardNum = 0;
		return true;
	}

	RecoverDragingCard();
	return false;
}

void UUI_PlayWindow::RecoverDragingCard()
{
	if (CardBox->GetChildrenCount() < 5)
	{
		UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
		UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
		NewCard->FixCost = DragingCardFixCost;
		UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
		NewCard->InsertImage(DragingCardNum);
		UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
		BoxSlot->SetPadding(10);
		FSlateChildSize Size(ESlateSizeRule::Type::Fill);
		BoxSlot->SetSize(Size);
		BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	}
	else
	{
		UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
		UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
		NewCard->FixCost = DragingCardFixCost;
		UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
		NewCard->InsertImage(DragingCardNum);
		UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
		BoxSlot->SetPadding(10);
		FSlateChildSize Size(ESlateSizeRule::Type::Fill);
		BoxSlot->SetSize(Size);
		BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	}

	ChangeHandCost();
	ChangeTurnText();
	DragingCardNum = 0;
}

void UUI_PlayWindow::ChangeHandCost()
{
	for (int i = 0; i < CardBox->GetChildrenCount() + CardBox2->GetChildrenCount(); i++)
	{
		if (i < 5)
		{
			UUI_PlayCard* MyCard = Cast<UUI_PlayCard>(CardBox->GetChildAt(i));
			if (MyCard != nullptr)
			{
				if (MyCard->ImageNum == 49)
				{
					int ks = 0;
					if (EnemyLife[1])
					{
						ks += 1;
					}
					if (EnemyLife[2])
					{
						ks += 1;
					}
					if (EnemyLife[3])
					{
						ks += 1;
					}

					if (BeCombination)
					{
						MyCard->MarkChangeCost(-ks + 1, BeGrinding);
					}
					else
					{
						MyCard->MarkChangeCost(-ks, BeGrinding);
					}
				}
				else if ((MyCard->ImageNum == 31 || MyCard->ImageNum == 35 || MyCard->ImageNum == 50 || MyCard->ImageNum == 51) && OnCombo_Extinction_CostDown1)
				{
					MyCard->MarkChangeCost(-1, BeGrinding);
				}
				else if ((MyCard->ImageNum == 25 || MyCard->ImageNum == 26 || MyCard->ImageNum == 27) && OnCombo_Triangle_CostDown1)
				{
					MyCard->MarkChangeCost(-3, BeGrinding);
				}
				else if (MyCard->ImageNum == 32)
				{
					if (OnCombo_Triangle_CostDown1 && OnCombo_Extinction_CostDown1)
					{
						MyCard->MarkChangeCost(-4, BeGrinding);
					}
					else if (!OnCombo_Triangle_CostDown1 && OnCombo_Extinction_CostDown1)
					{
						MyCard->MarkChangeCost(-1, BeGrinding);
					}
					else if (OnCombo_Triangle_CostDown1 && !OnCombo_Extinction_CostDown1)
					{
						MyCard->MarkChangeCost(-3, BeGrinding);
					}
				}
				else if (BeCombination && !MyCard->ComboCard) // 연격의 대가 발동 시 콤보 카드가 아닌 카드 비용 1 증가(기본기 연마랑 상쇄될 수 있음)
				{
					MyCard->MarkChangeCost(1, BeGrinding);
				}
				else if (BeGrinding)
				{
					MyCard->MarkChangeCost(0, BeGrinding);
				}

				if (MyCard->FixCost >= 0)
				{
					MyCard->MarkChangeCost(0, BeGrinding);
				}
			}
		}
		else
		{
			UUI_PlayCard* MyCard = Cast<UUI_PlayCard>(CardBox2->GetChildAt(i - 5));
			if (MyCard != nullptr)
			{
				if (MyCard->ImageNum == 49)
				{
					int ks = 0;
					if (EnemyLife[1])
					{
						ks += 1;
					}
					if (EnemyLife[2])
					{
						ks += 1;
					}
					if (EnemyLife[3])
					{
						ks += 1;
					}

					if (BeCombination)
					{
						MyCard->MarkChangeCost(-ks + 1, BeGrinding);
					}
					else
					{
						MyCard->MarkChangeCost(-ks, BeGrinding);
					}
				}
				else if ((MyCard->ImageNum == 31 || MyCard->ImageNum == 35 || MyCard->ImageNum == 50 || MyCard->ImageNum == 51) && OnCombo_Extinction_CostDown1)
				{
					MyCard->MarkChangeCost(-1, BeGrinding);
				}
				else if ((MyCard->ImageNum == 25 || MyCard->ImageNum == 26 || MyCard->ImageNum == 27) && OnCombo_Triangle_CostDown1)
				{
					MyCard->MarkChangeCost(-3, BeGrinding);
				}
				else if (MyCard->ImageNum == 32)
				{
					if (OnCombo_Triangle_CostDown1 && OnCombo_Extinction_CostDown1)
					{
						MyCard->MarkChangeCost(-4, BeGrinding);
					}
					else if (!OnCombo_Triangle_CostDown1 && OnCombo_Extinction_CostDown1)
					{
						MyCard->MarkChangeCost(-1, BeGrinding);
					}
					else if (OnCombo_Triangle_CostDown1 && !OnCombo_Extinction_CostDown1)
					{
						MyCard->MarkChangeCost(-3, BeGrinding);
					}
				}
				else if (BeCombination && !MyCard->ComboCard) // 연격의 대가 발동 시 콤보 카드가 아닌 카드 비용 1 증가(기본기 연마랑 상쇄될 수 있음)
				{
					MyCard->MarkChangeCost(1, BeGrinding);
				}
				else if (BeGrinding)
				{
					MyCard->MarkChangeCost(0, BeGrinding);
				}

				if (MyCard->FixCost >= 0)
				{
					MyCard->MarkChangeCost(0, BeGrinding);
				}
			}
		}
	}
}

void UUI_PlayWindow::ChangeHandUpgrade() // 이번 전투 지속
{
	for (int i = 0; i < CardBox->GetChildrenCount() + CardBox2->GetChildrenCount(); i++)
	{
		if (i < 5)
		{
			UUI_PlayCard* MyCard = Cast<UUI_PlayCard>(CardBox->GetChildAt(i));
			if (MyCard != nullptr && MyCard->ImageNum < 80)
			{
				MyCard->InsertImage(MyCard->ImageNum + 100);
				HandCard.Add(MyCard->ImageNum + 100);
				HandCard.RemoveSingle(MyCard->ImageNum);
			}
		}
		else
		{
			UUI_PlayCard* MyCard = Cast<UUI_PlayCard>(CardBox2->GetChildAt(i - 5));
			if (MyCard != nullptr && MyCard->ImageNum < 80)
			{
				MyCard->InsertImage(MyCard->ImageNum + 100);
				HandCard.Add(MyCard->ImageNum + 100);
				HandCard.RemoveSingle(MyCard->ImageNum);
			}
		}
	}
}

void UUI_PlayWindow::FixHandCost(int CardNum) // 이번 턴 지속
{
	for (int i = 0; i < CardBox->GetChildrenCount() + CardBox2->GetChildrenCount(); i++)
	{
		if (i < 5)
		{
			UUI_PlayCard* MyCard = Cast<UUI_PlayCard>(CardBox->GetChildAt(i));
			if (MyCard != nullptr && MyCard->MyCardNum == CardNum)
			{
				MyCard->FixCost = 1;
			}
		}
		else
		{
			UUI_PlayCard* MyCard = Cast<UUI_PlayCard>(CardBox2->GetChildAt(i - 5));
			if (MyCard != nullptr && MyCard->MyCardNum == CardNum)
			{
				MyCard->FixCost = 1;
			}
		}
	}
	ChangeHandCost();
}

void UUI_PlayWindow::FastAttacking(int MobNum, int AttackValue)
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	FName EnemyTag = FName(*("Enemy" + FString::FromInt(MobNum)));

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && MyPlayer != nullptr)
		{
			if (EnemyLife[MobNum] && Enemy->ActorHasTag(EnemyTag))
			{
				int findamage = AttackValue;

				if (BeMadness) // 광기 발현 상태이면
				{
					findamage = 1;
				}

				findamage += AForgottenKnightCharacter::MySwordLv - 1;

				if (AForgottenKnightCharacter::MyAccessoryData[1] == 1) // 철 팔찌 = 대미지+2
				{
					findamage += 2;
				}
				else if (AForgottenKnightCharacter::MyAccessoryData[1] == 2) // 루비 팔찌 = 대미지-1
				{
					findamage -= 1;
				}

				if (AForgottenKnightCharacter::MyAccessoryData[2] == 1) // 철 반지 = 대미지+1
				{
					findamage += 1;
				}
				else if (AForgottenKnightCharacter::MyAccessoryData[2] == 5) // 철 반지+ = 대미지+2
				{
					findamage += 2;
				}

				findamage += MyPlayer->AttackUp;

				findamage += MyPlayer->ShowBonusAttack();

				if (Enemy->GetStackCorrosion > 0 && MyPlayer->GetStackWeakening <= 0) // 내가 부식되고, 상대가 약화되지 않았다면
				{
					findamage = round(float(findamage) * mycor);
				}
				else if (Enemy->GetStackCorrosion <= 0 && MyPlayer->GetStackWeakening > 0) // 내가 부식되지 않고, 상대가 약화되었다면
				{
					findamage = round(float(findamage) * 0.75f); // 대미지 25퍼 감소
				}
				else if (Enemy->GetStackCorrosion > 0 && MyPlayer->GetStackWeakening > 0) // 내가 부식되고, 상대가 약화되었다면
				{
					findamage = round(float(findamage) * mycor * 0.75f); // 대미지 25퍼 감소
				}
				else if (Enemy->GetStackCorrosion <= 0 && MyPlayer->GetStackWeakening <= 0) // 내가 부식되지 않고, 상대가 약화되지 않았다면
				{
					findamage = round(float(findamage)); // 대미지 25퍼 감소
				}

				if (MobNum == 1)
				{
					EnemyHealthbar1->GetDamaged(findamage);
					EnemyHealthbar1->DamageEffect(-1, 1, true, Enemy->GetStackCorrosion, Enemy->GetStackWeakening, Enemy->GetStackBlooding);
				}
				else if (MobNum == 2)
				{
					EnemyHealthbar2->GetDamaged(findamage);
					EnemyHealthbar2->DamageEffect(-1, 1, true, Enemy->GetStackCorrosion, Enemy->GetStackWeakening, Enemy->GetStackBlooding);
				}
				else if (MobNum == 3)
				{
					EnemyHealthbar3->GetDamaged(findamage);
					EnemyHealthbar3->DamageEffect(-1, 1, true, Enemy->GetStackCorrosion, Enemy->GetStackWeakening, Enemy->GetStackBlooding);
				}

			}
		}
	}

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			Enemy->UpdateNagativeEffect(); // 확정 디버프 갱신
		}
	}

	CheckLife();

	ChangeHandCost();
}

void UUI_PlayWindow::CheckLife()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (Enemy->ActorHasTag("Enemy1") && EnemyLife[1]) // 적1
			{
				if (Enemy->LifeCheck() == false) // 적1이 죽었으면
				{
					if (Enemy->ActorHasTag("SkullHead"))
					{
						EnemyLife[1] = false;
						Enemy->Tags[2] = FName(" ");
					}
					else
					{
						if (!Enemy->ActorHasTag("MagicBall"))
						{
							EnemyCount -= 1;
						}
						EnemyLife[1] = false;

						// 흑마 1페이즈 기믹(아군 죽으면 피 10 깎이고 몹 소환
						if (EnemyHealthbar2->IamWarlock)
						{
							EnemyHealthbar2->UpdateHealthBar(-10);
							if (EnemyHealthbar2->CurrentHealth <= 0)
							{
								EnemyHealthbar2->CurrentHealth = 1;
							}
						}

						// 흑마 2페이즈 기믹(머리 한쪽 죽으면 살아있는쪽 힐,정화
						if (EnemyHealthbar2->IamWarlock2Phase && EnemyLife[2])
						{
							EnemyHealthbar2->UpdateHealthBar(10);
							Cleanze(2);
						}
						if (EnemyHealthbar3->IamWarlock2Phase && EnemyLife[3])
						{
							EnemyHealthbar3->UpdateHealthBar(10);
							Cleanze(3);
						}

						// 해골 사망 기믹(죽으면 해골머리로 변해서 부활)
						if (EnemyHealthbar1->IamSkeleton > 0 && !EnemyHealthbar1->Skullmode)
						{
							SoonSkullHead[1] = EnemyHealthbar1->IamSkeleton;
						}

						if (Enemy->ActorHasTag("FallenFairy") && Enemy->FairyTurn < 4)
						{
							AForgottenKnightCharacter::MyCurrentHealth += 15;
							if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
							{
								AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
							}
							MyPlayer->DebuffCleanze();
							MyPlayer->UpdateNagativeEffect();
						}

						Enemy->IamScent = false;
					}
				}
			}
			else if (Enemy->ActorHasTag("Enemy2") && EnemyLife[2]) // 적2
			{
				if (Enemy->LifeCheck() == false)
				{
					if (Enemy->ActorHasTag("SkullHead"))
					{
						EnemyLife[2] = false;
						Enemy->Tags[2] = FName(" ");
					}
					else
					{
						if (!Enemy->ActorHasTag("MagicBall"))
						{
							EnemyCount -= 1;
						}
						EnemyLife[2] = false;

						// 흑마 2페이즈 기믹(머리 한쪽 죽으면 살아있는쪽 힐,정화
						if (EnemyHealthbar1->IamWarlock2Phase && EnemyLife[1])
						{
							EnemyHealthbar1->UpdateHealthBar(10);
							Cleanze(1);
						}
						if (EnemyHealthbar3->IamWarlock2Phase && EnemyLife[3])
						{
							EnemyHealthbar3->UpdateHealthBar(10);
							Cleanze(3);
						}

						// 해골 사망 기믹(죽으면 해골머리로 변해서 부활)
						if (EnemyHealthbar2->IamSkeleton > 0 && !EnemyHealthbar2->Skullmode)
						{
							SoonSkullHead[2] = EnemyHealthbar2->IamSkeleton;
						}

						if (Enemy->ActorHasTag("FallenFairy") && Enemy->FairyTurn < 4)
						{
							AForgottenKnightCharacter::MyCurrentHealth += 15;
							if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
							{
								AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
							}
							MyPlayer->DebuffCleanze();
							MyPlayer->UpdateNagativeEffect();
						}

						Enemy->IamScent = false;
					}
				}
			}
			else if (Enemy->ActorHasTag("Enemy3") && EnemyLife[3]) // 적3
			{
				if (Enemy->LifeCheck() == false)
				{
					if (Enemy->ActorHasTag("SkullHead"))
					{
						EnemyLife[3] = false;
						Enemy->Tags[2] = FName(" ");
					}
					else
					{
						if (!Enemy->ActorHasTag("MagicBall"))
						{
							EnemyCount -= 1;
						}
						EnemyLife[3] = false;

						// 흑마 1페이즈 기믹(아군 죽으면 피 10 깎이고 몹 소환
						if (EnemyHealthbar2->IamWarlock)
						{
							EnemyHealthbar2->UpdateHealthBar(-10);
							if (EnemyHealthbar2->CurrentHealth <= 0)
							{
								EnemyHealthbar2->CurrentHealth = 1;
							}
						}

						// 흑마 2페이즈 기믹(머리 한쪽 죽으면 살아있는쪽 힐,정화
						if (EnemyHealthbar2->IamWarlock2Phase && EnemyLife[2])
						{
							EnemyHealthbar2->UpdateHealthBar(10);
							Cleanze(2);
						}
						if (EnemyHealthbar1->IamWarlock2Phase && EnemyLife[1])
						{
							EnemyHealthbar1->UpdateHealthBar(10);
							Cleanze(1);
						}

						// 해골 사망 기믹(죽으면 해골머리로 변해서 부활)
						if (EnemyHealthbar3->IamSkeleton > 0 && !EnemyHealthbar3->Skullmode)
						{
							SoonSkullHead[3] = EnemyHealthbar3->IamSkeleton;
						}

						if (Enemy->ActorHasTag("FallenFairy") && Enemy->FairyTurn < 4)
						{
							AForgottenKnightCharacter::MyCurrentHealth += 15;
							if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
							{
								AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
							}
							MyPlayer->DebuffCleanze();
							MyPlayer->UpdateNagativeEffect();
						}

						Enemy->IamScent = false;
					}
				}
			}
		}
	}

	if (EnemyHealthbar2->IamWarlock2Phase)
	{
		if (EnemyLife[1])
		{
			if (EnemyLife[2])
			{
				if (EnemyLife[3])
				{
					Image_Cerberus->SetBrush(ImgCerABC);
				}
				else
				{
					Image_Cerberus->SetBrush(ImgCerAB);
				}
			}
			else
			{
				if (EnemyLife[3])
				{
					Image_Cerberus->SetBrush(ImgCerAC);
				}
				else
				{
					Image_Cerberus->SetBrush(ImgCerA);
				}
			}
		}
		else
		{
			if (EnemyLife[2])
			{
				if (EnemyLife[3])
				{
					Image_Cerberus->SetBrush(ImgCerBC);
				}
				else
				{
					Image_Cerberus->SetBrush(ImgCerB);
				}
			}
			else
			{
				if (EnemyLife[3])
				{
					Image_Cerberus->SetBrush(ImgCerC);
				}
			}
		}
	}

	if (LastOrder == 4)
	{
		if (!EnemyLife[1])
		{
			Image_demonright->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			Image_demonright->SetVisibility(ESlateVisibility::Visible);
		}
		
		if (!EnemyLife[2])
		{
			Image_demonhead->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			Image_demonhead->SetVisibility(ESlateVisibility::Visible);
		}

		if (!EnemyLife[3])
		{
			Image_demonleft->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			Image_demonleft->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if (LastOrder == 3) // 왕 2페이즈일 경우
	{
		if (KingHP <= 0)
		{
			KingHP = 0;
			EnemyCount = 0;
		}
	}

	if (AForgottenKnightCharacter::MyCurrentHealth <= 0)
	{
		if (SpecialBoomOn)
		{
			SpecialBoomOn = false;
			AController_MainCharacter* Controller = Cast<AController_MainCharacter>(GetOwningPlayer());
			if (Controller)
			{
				Controller->ShowDialogueWidget(7);
			}
		}
		else
		{
			AForgottenKnightCharacter::MyCurrentHealth = StartCurrentHealth;

			StageDie();
		}
	}
}

void UUI_PlayWindow::WarlockPhase1Revive()
{
	// 흑마 1페이즈 특수 패턴
	if (WarlockMobCount == 0 && EnemyCount < 13 && FieldinWarlock) // 트롤 소환
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && !EnemyLife[1] && WarlockMobCount == 0) // 몹1이 죽은 상태면
			{
				EnemyLife[1] = true; // 다시 살리고
				Enemy->Tags[2] = FName("Troll");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar1->Revive();
				WarlockMobCount += 1;
			}
			else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && !EnemyLife[3] && WarlockMobCount == 0) // 몹3이 죽은 상태면
			{
				EnemyLife[3] = true; // 다시 살리고
				Enemy->Tags[2] = FName("Troll");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar3->Revive();
				WarlockMobCount += 1;
			}
		}
	}
	if (WarlockMobCount == 1 && EnemyCount < 12 && FieldinWarlock) // 거미무리 소환
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && !EnemyLife[1] && WarlockMobCount == 1) // 몹1이 죽은 상태면
			{
				EnemyLife[1] = true; // 다시 살리고
				Enemy->Tags[2] = FName("Spider");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar1->Revive();
				WarlockMobCount += 1;
			}
			else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && !EnemyLife[3] && WarlockMobCount == 1) // 몹3이 죽은 상태면
			{
				EnemyLife[3] = true; // 다시 살리고
				Enemy->Tags[2] = FName("Spider");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar3->Revive();
				WarlockMobCount += 1;
			}
		}
	}
	if (WarlockMobCount == 2 && EnemyCount < 11 && FieldinWarlock) // 골렘 소환
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && !EnemyLife[1] && WarlockMobCount == 2) // 몹1이 죽은 상태면
			{
				EnemyLife[1] = true; // 다시 살리고
				Enemy->Tags[2] = FName("Golem");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar1->Revive();
				WarlockMobCount += 1;
			}
			else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && !EnemyLife[3] && WarlockMobCount == 2) // 몹3이 죽은 상태면
			{
				EnemyLife[3] = true; // 다시 살리고
				Enemy->Tags[2] = FName("Golem");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar3->Revive();
				WarlockMobCount += 1;
			}
		}
	}
	if (WarlockMobCount == 3 && EnemyCount < 10 && FieldinWarlock) // 그림자 창병 소환
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && !EnemyLife[1] && WarlockMobCount == 3) // 몹1이 죽은 상태면
			{
				EnemyLife[1] = true; // 다시 살리고
				Enemy->Tags[2] = FName("ShadowSpear");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar1->Revive();
				WarlockMobCount += 1;
			}
			else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && !EnemyLife[3] && WarlockMobCount == 3) // 몹3이 죽은 상태면
			{
				EnemyLife[3] = true; // 다시 살리고
				Enemy->Tags[2] = FName("ShadowSpear");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar3->Revive();
				WarlockMobCount += 1;
			}
		}
	}
	if (WarlockMobCount == 4 && EnemyCount < 9 && FieldinWarlock) // 그림자 방패병 소환
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && !EnemyLife[1] && WarlockMobCount == 4) // 몹1이 죽은 상태면
			{
				EnemyLife[1] = true; // 다시 살리고
				Enemy->Tags[2] = FName("ShadowSheild");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar1->Revive();
				WarlockMobCount += 1;
			}
			else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && !EnemyLife[3] && WarlockMobCount == 4) // 몹3이 죽은 상태면
			{
				EnemyLife[3] = true; // 다시 살리고
				Enemy->Tags[2] = FName("ShadowSheild");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar3->Revive();
				WarlockMobCount += 1;
			}
		}
	}
	if (WarlockMobCount == 5 && EnemyCount < 8 && FieldinWarlock) // 그림자 도끼병 소환
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && !EnemyLife[1] && WarlockMobCount == 5) // 몹1이 죽은 상태면
			{
				EnemyLife[1] = true; // 다시 살리고
				Enemy->Tags[2] = FName("ShadowAxe");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar1->Revive();
				WarlockMobCount += 1;
			}
			else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && !EnemyLife[3] && WarlockMobCount == 5) // 몹3이 죽은 상태면
			{
				EnemyLife[3] = true; // 다시 살리고
				Enemy->Tags[2] = FName("ShadowAxe");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar3->Revive();
				WarlockMobCount += 1;
			}
		}
	}
	if (WarlockMobCount == 6 && EnemyCount < 7 && FieldinWarlock) // 그리폰 소환
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && !EnemyLife[1] && WarlockMobCount == 6) // 몹1이 죽은 상태면
			{
				EnemyLife[1] = true; // 다시 살리고
				Enemy->Tags[2] = FName("Griffon");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar1->Revive();
				WarlockMobCount += 1;
			}
			else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && !EnemyLife[3] && WarlockMobCount == 6) // 몹3이 죽은 상태면
			{
				EnemyLife[3] = true; // 다시 살리고
				Enemy->Tags[2] = FName("Griffon");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar3->Revive();
				WarlockMobCount += 1;
			}
		}
	}
	if (WarlockMobCount == 7 && EnemyCount < 6 && FieldinWarlock) // 라플레시아 소환
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && !EnemyLife[1] && WarlockMobCount == 7) // 몹1이 죽은 상태면
			{
				EnemyLife[1] = true; // 다시 살리고
				Enemy->Tags[2] = FName("Rafflesia");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar1->Revive();
				WarlockMobCount += 1;
			}
			else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && !EnemyLife[3] && WarlockMobCount == 7) // 몹3이 죽은 상태면
			{
				EnemyLife[3] = true; // 다시 살리고
				Enemy->Tags[2] = FName("Rafflesia");
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar3->Revive();
				WarlockMobCount += 1;
			}
		}
	}
	if (WarlockMobCount == 8 && EnemyCount < 4 && FieldinWarlock) // 2페이즈 돌입
	{
		AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		AController_MainCharacter* Controller = Cast<AController_MainCharacter>(GetOwningPlayer());
		if (Controller)
		{
			Controller->ShowDialogueWidget(1);
		}

		PlayAnimation(Anim_Cerberus);
		Image_Cerberus->SetBrush(ImgCerABC);

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1")) // 2페이즈 돌입
			{
				EnemyLife[1] = true;
				Enemy->Tags[2] = FName("Warlock2Phase");
				Enemy->WarlockAttackmode = true;
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar1->Revive();
				WarlockMobCount += 1;
			}
			else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy2"))
			{
				EnemyLife[2] = true;
				Enemy->Tags[2] = FName("Warlock2Phase");
				Enemy->WarlockAttackmode = true;
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				WarlockMobCount += 1;
			}
			else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3"))
			{
				EnemyLife[3] = true;
				Enemy->Tags[2] = FName("Warlock2Phase");
				Enemy->WarlockAttackmode = true;
				Enemy->SetStat();
				Enemy->CleanzeDebuff();
				Enemy->UpdateNagativeEffect();
				EnemyHealthbar3->Revive();
				WarlockMobCount += 1;
			}
		}
	}



}

void UUI_PlayWindow::SkullRevive()
{
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (SoonSkullHead[1] > 0 && Enemy->ActorHasTag("Enemy1")) // 몹1이 해골머리로 부활
			{
				if (!EnemyLife[1]) // 몹1이 죽은 상태면
				{
					EnemyLife[1] = true; // 다시 살리고
					SoonSkullHead[1] = 0; // 변수 초기화
					EnemyHealthbar1->SkullReviveCool = 0; // 변수 초기화
					Enemy->Tags[2] = FName("SkullHead");
					Enemy->SetStat();
					EnemyHealthbar1->Revive();
				}
			}
			else if (SoonSkullHead[2] > 0 && Enemy->ActorHasTag("Enemy2")) // 몹2이 해골머리로 부활
			{
				if (!EnemyLife[2]) // 몹2이 죽은 상태면
				{
					EnemyLife[2] = true; // 다시 살리고
					SoonSkullHead[2] = 0; // 변수 초기화
					EnemyHealthbar2->SkullReviveCool = 0; // 변수 초기화
					Enemy->Tags[2] = FName("SkullHead");
					Enemy->SetStat();
					EnemyHealthbar2->Revive();
				}
			}
			else if (SoonSkullHead[3] > 0 && Enemy->ActorHasTag("Enemy3")) // 몹3이 해골머리로 부활
			{
				if (!EnemyLife[3]) // 몹3이 죽은 상태면
				{
					EnemyLife[3] = true; // 다시 살리고
					SoonSkullHead[3] = 0; // 변수 초기화
					EnemyHealthbar3->SkullReviveCool = 0; // 변수 초기화
					Enemy->Tags[2] = FName("SkullHead");
					Enemy->SetStat();
					EnemyHealthbar3->Revive();
				}
			}
			else if (EnemyHealthbar1->Skullmode && EnemyHealthbar1->SkullReviveCool >= SkullReviveCooltime && Enemy->ActorHasTag("Enemy1"))
			{ // 부활할 턴(2턴)이 왔다면
				EnemyHealthbar1->SkullReviveCool = 0; // 변수 초기화
				EnemyHealthbar1->Skullmode = false;
				SoonSkullHead[1] = 0;
				EnemyCount += 1;
				Enemy->Tags[2] = FName(" ");
				Enemy->SetStat();
				EnemyHealthbar1->Revive();
			}
			else if (EnemyHealthbar2->Skullmode && EnemyHealthbar2->SkullReviveCool >= SkullReviveCooltime && Enemy->ActorHasTag("Enemy2"))
			{ // 부활할 턴(2턴)이 왔다면
				EnemyHealthbar2->SkullReviveCool = 0; // 변수 초기화
				EnemyHealthbar2->Skullmode = false;
				SoonSkullHead[2] = 0;
				EnemyCount += 1;
				Enemy->Tags[2] = FName(" ");
				Enemy->SetStat();
				EnemyHealthbar2->Revive();
			}
			else if (EnemyHealthbar3->Skullmode && EnemyHealthbar3->SkullReviveCool >= SkullReviveCooltime && Enemy->ActorHasTag("Enemy3"))
			{ // 부활할 턴(2턴)이 왔다면
				EnemyHealthbar3->SkullReviveCool = 0; // 변수 초기화
				EnemyHealthbar3->Skullmode = false;
				SoonSkullHead[3] = 0;
				EnemyCount += 1;
				Enemy->Tags[2] = FName(" ");
				Enemy->SetStat();
				EnemyHealthbar3->Revive();
			}
		}
	}

}

void UUI_PlayWindow::LichReviveSkill(int mobnum)
{
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (Enemy->ActorHasTag("Enemy1") && !EnemyLife[1]) // 몹1이 죽은 상태면
			{
				EnemyLife[1] = true; // 다시 살리고
				EnemyCount += 1; // 몹 카운트 복구
				EnemyHealthbar1->IamSkeleton = 0;
				EnemyHealthbar1->SkullReviveCool = 0;
				EnemyHealthbar1->Skullmode = false;
				SoonSkullHead[1] = 0;

				if (mobnum == 1)
				{
					Enemy->Tags[1] = FName("SkeletonSoldier");
					Enemy->Tags[2] = FName(" ");
				}
				else if (mobnum == 2)
				{
					Enemy->Tags[1] = FName("SkeletonArcher");
					Enemy->Tags[2] = FName(" ");
				}
				else if (mobnum == 3)
				{
					Enemy->Tags[1] = FName("SkeletonGuardDog");
					Enemy->Tags[2] = FName(" ");
				}

				Enemy->SetStat();
				EnemyHealthbar1->Revive();

				return;
			}
			else if (Enemy->ActorHasTag("Enemy2") && !EnemyLife[2]) // 몹2이 죽은 상태면
			{
				EnemyLife[2] = true; // 다시 살리고
				EnemyCount += 1; // 몹 카운트 복구
				EnemyHealthbar2->IamSkeleton = 0;
				EnemyHealthbar2->SkullReviveCool = 0;
				EnemyHealthbar2->Skullmode = false;
				SoonSkullHead[2] = 0;

				if (mobnum == 1)
				{
					Enemy->Tags[1] = FName("SkeletonSoldier");
					Enemy->Tags[2] = FName(" ");
				}
				else if (mobnum == 2)
				{
					Enemy->Tags[1] = FName("SkeletonArcher");
					Enemy->Tags[2] = FName(" ");
				}
				else if (mobnum == 3)
				{
					Enemy->Tags[1] = FName("SkeletonGuardDog");
					Enemy->Tags[2] = FName(" ");
				}

				Enemy->SetStat();
				EnemyHealthbar2->Revive();

				return;
			}
			else if (Enemy->ActorHasTag("Enemy3") && !EnemyLife[3]) // 몹3이 죽은 상태면
			{
				EnemyLife[3] = true; // 다시 살리고
				EnemyCount += 1; // 몹 카운트 복구
				EnemyHealthbar3->IamSkeleton = 0;
				EnemyHealthbar3->SkullReviveCool = 0;
				EnemyHealthbar3->Skullmode = false;
				SoonSkullHead[3] = 0;

				if (mobnum == 1)
				{
					Enemy->Tags[1] = FName("SkeletonSoldier");
					Enemy->Tags[2] = FName(" ");
				}
				else if (mobnum == 2)
				{
					Enemy->Tags[1] = FName("SkeletonArcher");
					Enemy->Tags[2] = FName(" ");
				}
				else if (mobnum == 3)
				{
					Enemy->Tags[1] = FName("SkeletonGuardDog");
					Enemy->Tags[2] = FName(" ");
				}

				Enemy->SetStat();
				EnemyHealthbar3->Revive();

				return;
			}
		}
	}
}

bool UUI_PlayWindow::FieldOnlyLich()
{
	int qwer = 0;

	if (!EnemyHealthbar1->IamLich) // 적 1이 리치, 해골머리가 아니면
	{
		if (!EnemyLife[1] || EnemyHealthbar1->Skullmode)
		{
			qwer += 1;
		}
	}
	else // 리치이면
	{
		qwer += 1;
	}

	if (!EnemyHealthbar2->IamLich) // 적 2이 리치가 아니면
	{
		if (!EnemyLife[2] || EnemyHealthbar2->Skullmode)
		{
			qwer += 1;
		}
	}
	else // 리치이면
	{
		qwer += 1;
	}

	if (!EnemyHealthbar3->IamLich) // 적 3이 리치가 아니면
	{
		if (!EnemyLife[3] || EnemyHealthbar3->Skullmode)
		{
			qwer += 1;
		}
	}
	else // 리치이면
	{
		qwer += 1;
	}

	if (qwer == 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UUI_PlayWindow::SkeletonFlag()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// 전턴 깃발 초기화
	if (FlagOn == 1) // 전 턴 붉은 깃발
	{
		FlagOn = 0;

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1") && FlagEffect[1])
				{
					FlagEffect[1] = false;
					Enemy->BonusDamageValue -= 4;
					if (Enemy->BonusDamageValue < 0)
					{
						Enemy->BonusDamageValue = 0;
					}
				}
				else if (Enemy->ActorHasTag("Enemy2") && FlagEffect[2])
				{
					FlagEffect[2] = false;
					Enemy->BonusDamageValue -= 4;
					if (Enemy->BonusDamageValue < 0)
					{
						Enemy->BonusDamageValue = 0;
					}
				}
				else if (Enemy->ActorHasTag("Enemy3") && FlagEffect[3])
				{
					FlagEffect[3] = false;
					Enemy->BonusDamageValue -= 4;
					if (Enemy->BonusDamageValue < 0)
					{
						Enemy->BonusDamageValue = 0;
					}
				}
			}
		}

		if (MyPlayer->ActorHasTag("Buff_SkeletonCenturion_D") && FlagEffect[0])
		{
			FlagEffect[0] = false;
			MyHealthbar0->BonusDamageValue -= 4;
			if (MyHealthbar0->BonusDamageValue < 0)
			{
				MyHealthbar0->BonusDamageValue = 0;
			}
		}
	}
	else if (FlagOn == 2) // 전 턴 푸른 깃발
	{
		FlagOn = 0;

		if (FlagEffect[1])
		{
			FlagEffect[1] = false;
			EnemyHealthbar1->BonusDefenseValue -= 4;
			if (EnemyHealthbar1->BonusDefenseValue < 0)
			{
				EnemyHealthbar1->BonusDefenseValue = 0;
			}
		}
		if (FlagEffect[2])
		{
			FlagEffect[2] = false;
			EnemyHealthbar2->BonusDefenseValue -= 4;
			if (EnemyHealthbar2->BonusDefenseValue < 0)
			{
				EnemyHealthbar2->BonusDefenseValue = 0;
			}
		}
		if (FlagEffect[3])
		{
			FlagEffect[3] = false;
			EnemyHealthbar3->BonusDefenseValue -= 4;
			if (EnemyHealthbar3->BonusDefenseValue < 0)
			{
				EnemyHealthbar3->BonusDefenseValue = 0;
			}
		}

		if (MyPlayer->ActorHasTag("Buff_SkeletonCenturion_D") && FlagEffect[0])
		{
			FlagEffect[0] = false;
			MyHealthbar0->BonusDefenseValue -= 4;
			if (MyHealthbar0->BonusDefenseValue < 0)
			{
				MyHealthbar0->BonusDefenseValue = 0;
			}
		}
	}
	else if (FlagOn == 3) // 전 턴 초록 깃발
	{
		FlagOn = 0;
		FlagEffect[1] = false;
		FlagEffect[2] = false;
		FlagEffect[3] = false;
		FlagEffect[0] = false;
	}

	// 백인대장 기믹
	if (MyPlayer->ActorHasTag("Buff_SkeletonCenturion")) // 적에게만 버프 영향
	{
		int sc = dist2(rng1);

		if (sc == 0) // 붉은 깃발 : 대미지 4 증가
		{
			FlagOn = 1;

			Image_Buffmob->SetVisibility(ESlateVisibility::Visible);
			Image_Buffmob->SetBrush(ImgSkeletonCenturionR);

			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr)
				{
					if (Enemy->ActorHasTag("Enemy1") && EnemyLife[1])
					{
						Enemy->BonusDamageValue += 4;
						FlagEffect[1] = true;
					}
					else if (Enemy->ActorHasTag("Enemy2") && EnemyLife[2])
					{
						Enemy->BonusDamageValue += 4;
						FlagEffect[2] = true;
					}
					else if (Enemy->ActorHasTag("Enemy3") && EnemyLife[3])
					{
						Enemy->BonusDamageValue += 4;
						FlagEffect[3] = true;
					}
				}
			}
		}
		else if (sc == 1) // 푸른 깃발 받뎀 4 감소
		{
			FlagOn = 2;

			Image_Buffmob->SetVisibility(ESlateVisibility::Visible);
			Image_Buffmob->SetBrush(ImgSkeletonCenturionB);

			if (EnemyLife[1])
			{
				EnemyHealthbar1->BonusDefenseValue += 4;
				FlagEffect[1] = true;
			}
			if (EnemyLife[2])
			{
				EnemyHealthbar2->BonusDefenseValue += 4;
				FlagEffect[2] = true;
			}
			if (EnemyLife[3])
			{
				EnemyHealthbar3->BonusDefenseValue += 4;
				FlagEffect[3] = true;
			}
		}
		else if (sc == 2) // 초록 깃발 체력 5 회복
		{
			FlagOn = 3;

			Image_Buffmob->SetVisibility(ESlateVisibility::Visible);
			Image_Buffmob->SetBrush(ImgSkeletonCenturionG);

			if (EnemyLife[1])
			{
				EnemyHealthbar1->UpdateHealthBar(5);
				FlagEffect[1] = true;
			}
			if (EnemyLife[2])
			{
				EnemyHealthbar2->UpdateHealthBar(5);
				FlagEffect[2] = true;
			}
			if (EnemyLife[3])
			{
				EnemyHealthbar3->UpdateHealthBar(5);
				FlagEffect[3] = true;
			}
		}
	}
	else if (MyPlayer->ActorHasTag("Buff_SkeletonCenturion_D")) // 모두 영향, 대신 힐은 나한테 2배
	{
		int sc = dist2(rng1);

		if (sc == 0) // 붉은 깃발 : 대미지 4 증가
		{
			FlagOn = 1;

			Image_Buffmob->SetVisibility(ESlateVisibility::Visible);
			Image_Buffmob->SetBrush(ImgSkeletonCenturionR);

			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr)
				{
					if (Enemy->ActorHasTag("Enemy1") && EnemyLife[1])
					{
						Enemy->BonusDamageValue += 4;
						FlagEffect[1] = true;
					}
					else if (Enemy->ActorHasTag("Enemy2") && EnemyLife[2])
					{
						Enemy->BonusDamageValue += 4;
						FlagEffect[2] = true;
					}
					else if (Enemy->ActorHasTag("Enemy3") && EnemyLife[3])
					{
						Enemy->BonusDamageValue += 4;
						FlagEffect[3] = true;
					}
				}
			}

			MyHealthbar0->BonusDamageValue += 4;
			FlagEffect[0] = true;
		}
		else if (sc == 1) // 푸른 깃발 받뎀 4 감소
		{
			FlagOn = 2;

			Image_Buffmob->SetVisibility(ESlateVisibility::Visible);
			Image_Buffmob->SetBrush(ImgSkeletonCenturionB);

			if (EnemyLife[1])
			{
				EnemyHealthbar1->BonusDefenseValue += 4;
				FlagEffect[1] = true;
			}
			if (EnemyLife[2])
			{
				EnemyHealthbar2->BonusDefenseValue += 4;
				FlagEffect[2] = true;
			}
			if (EnemyLife[3])
			{
				EnemyHealthbar3->BonusDefenseValue += 4;
				FlagEffect[3] = true;
			}

			MyHealthbar0->BonusDefenseValue += 4;
			FlagEffect[0] = true;
		}
		else if (sc == 2) // 초록 깃발 체력 5 회복(플레이어는 10)
		{
			FlagOn = 3;

			Image_Buffmob->SetVisibility(ESlateVisibility::Visible);
			Image_Buffmob->SetBrush(ImgSkeletonCenturionG);

			if (EnemyLife[1])
			{
				EnemyHealthbar1->UpdateHealthBar(5);
				FlagEffect[1] = true;
			}
			if (EnemyLife[2])
			{
				EnemyHealthbar2->UpdateHealthBar(5);
				FlagEffect[2] = true;
			}
			if (EnemyLife[3])
			{
				EnemyHealthbar3->UpdateHealthBar(5);
				FlagEffect[3] = true;
			}

			MyHealthbar0->PlayHealEffect(10);
			FlagEffect[0] = true;
		}
	}
}

int UUI_PlayWindow::ShowBonusAttack()
{
	return MyHealthbar0->BonusDamageValue;
}

void UUI_PlayWindow::TakeDumpCard(int CardNum)
{
	UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
	UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);

	if (BeDelirium) // 착란 상태이면
	{
		int fixco = distDelirium(rng1);
		NewCard->FixCost = fixco;
	}

	if (CardBox->GetChildrenCount() < 5)
	{
		UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
		NewCard->InsertImage(CardNum);
		DumpDeck.RemoveSingle(CardNum); // 버린 덱에서 해당 카드 삭제
		HandCard.Add(CardNum);
		UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
		BoxSlot->SetPadding(10);
		FSlateChildSize Size(ESlateSizeRule::Type::Fill);
		BoxSlot->SetSize(Size);
		BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	}
	else
	{
		UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
		NewCard->InsertImage(CardNum);
		DumpDeck.RemoveSingle(CardNum); // 버린 덱에서 해당 카드 삭제
		HandCard.Add(CardNum);
		UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
		BoxSlot->SetPadding(10);
		FSlateChildSize Size(ESlateSizeRule::Type::Fill);
		BoxSlot->SetSize(Size);
		BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	}

	UE_LOG(LogTemp, Log, TEXT("TakeDumpCard"));

	ChangeHandCost();
	ChangeTurnText();
}

void UUI_PlayWindow::TakeDrawCard(int CardNum)
{
	UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
	UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);

	if (BeDelirium) // 착란 상태이면
	{
		int fixco = distDelirium(rng1);
		NewCard->FixCost = fixco;
	}

	if (CardBox->GetChildrenCount() < 5)
	{
		UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
		NewCard->InsertImage(CardNum);
		DrawDeck.RemoveSingle(CardNum); // 뽑을 덱에서 해당 카드 삭제
		DrawCardIndex += 1;
		HandCard.Add(CardNum);
		UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
		BoxSlot->SetPadding(10);
		FSlateChildSize Size(ESlateSizeRule::Type::Fill);
		BoxSlot->SetSize(Size);
		BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	}
	else
	{
		UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
		NewCard->InsertImage(CardNum);
		DrawDeck.RemoveSingle(CardNum); // 뽑을 덱에서 해당 카드 삭제
		DrawCardIndex += 1;
		HandCard.Add(CardNum);
		UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
		BoxSlot->SetPadding(10);
		FSlateChildSize Size(ESlateSizeRule::Type::Fill);
		BoxSlot->SetSize(Size);
		BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	}

	UE_LOG(LogTemp, Log, TEXT("TakeDrawCard"));

	ChangeHandCost();
	ChangeTurnText();
}

void UUI_PlayWindow::TakeChronicityCard(int CardNum)
{
	ChronicityDeck.Empty();

	if (CardNum == 13 || CardNum == 14 || CardNum == 15 || CardNum == 113 || CardNum == 114 || CardNum == 115)
	{
		TakeChronicityNum = 1; // 삼중연쇄
	}
	else if (CardNum == 16 || CardNum == 17 || CardNum == 18 || CardNum == 116 || CardNum == 117 || CardNum == 118)
	{
		TakeChronicityNum = 2; // 잽잽훅
	}
	else if (CardNum == 25 || CardNum == 26 || CardNum == 27 || CardNum == 125 || CardNum == 126 || CardNum == 127)
	{
		TakeChronicityNum = 3; // 삼위일체
	}
	else if (CardNum == 31 || CardNum == 35 || CardNum == 50 || CardNum == 51 || CardNum == 131 || CardNum == 135 || CardNum == 150 || CardNum == 151)
	{
		TakeChronicityNum = 4; // 오의 절멸
	}
	else if (CardNum == 41 || CardNum == 42 || CardNum == 43 || CardNum == 141 || CardNum == 142 || CardNum == 143)
	{
		TakeChronicityNum = 5; // 광란
	}
}

void UUI_PlayWindow::DurahanGiveHead()
{
	int x = 0;
	int UpDamage = 0;

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && Enemy->ActorHasTag("Durahan") && Enemy->ActorHasTag("Enemy1") && EnemyLife[1]) // 몹1이 듀라한이면
		{
			x = 1;
			UpDamage = Enemy->DurahanHead;
		}
		else if (Enemy != nullptr && Enemy->ActorHasTag("Durahan") && Enemy->ActorHasTag("Enemy2") && EnemyLife[2]) // 몹2이 듀라한이면
		{
			x = 2;
			UpDamage = Enemy->DurahanHead;
		}
		else if (Enemy != nullptr && Enemy->ActorHasTag("Durahan") && Enemy->ActorHasTag("Enemy3") && EnemyLife[3]) // 몹3이 듀라한이면
		{
			x = 3;
			UpDamage = Enemy->DurahanHead;
		}
	}

	if (x == 1) // 몹1이 듀라한이면
	{
		if (EnemyLife[2] && EnemyLife[3])
		{
			int k = dist3(rng1);

			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr && Enemy->ActorHasTag("Enemy2") && EnemyLife[2] && k == 0) //
				{
					Enemy->BonusDamageValue += UpDamage;
					Enemy->TakeDurahanHead = true;
				}
				else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && EnemyLife[3] && k == 1) // 
				{
					Enemy->BonusDamageValue += UpDamage;
					Enemy->TakeDurahanHead = true;
				}
			}
		}
		else if (EnemyLife[2] && !EnemyLife[3])
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr && Enemy->ActorHasTag("Enemy2") && EnemyLife[2]) //
				{
					Enemy->BonusDamageValue += UpDamage;
					Enemy->TakeDurahanHead = true;
				}
			}
		}
		else if (!EnemyLife[2] && EnemyLife[3])
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && EnemyLife[3]) //
				{
					Enemy->BonusDamageValue += UpDamage;
					Enemy->TakeDurahanHead = true;
				}
			}
		}
	}
	else if (x == 2) // 몹2이 듀라한이면
	{
		if (EnemyLife[1] && EnemyLife[3])
		{
			int k = dist3(rng1);

			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && EnemyLife[1] && k == 0) //
				{
					Enemy->BonusDamageValue += UpDamage;
					Enemy->TakeDurahanHead = true;
				}
				else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && EnemyLife[3] && k == 1) // 
				{
					Enemy->BonusDamageValue += UpDamage;
					Enemy->TakeDurahanHead = true;
				}
			}
		}
		else if (EnemyLife[1] && !EnemyLife[3])
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && EnemyLife[21]) //
				{
					Enemy->BonusDamageValue += UpDamage;
					Enemy->TakeDurahanHead = true;
				}
			}
		}
		else if (!EnemyLife[1] && EnemyLife[3])
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && EnemyLife[3]) //
				{
					Enemy->BonusDamageValue += UpDamage;
					Enemy->TakeDurahanHead = true;
				}
			}
		}
	}
	else if (x == 3) // 몹3이 듀라한이면
	{
		if (EnemyLife[1] && EnemyLife[2])
		{
			int k = dist3(rng1);

			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && EnemyLife[1] && k == 0) //
				{
					Enemy->BonusDamageValue += UpDamage;
					Enemy->TakeDurahanHead = true;
				}
				else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy2") && EnemyLife[2] && k == 1) // 
				{
					Enemy->BonusDamageValue += UpDamage;
					Enemy->TakeDurahanHead = true;
				}
			}
		}
		else if (EnemyLife[1] && !EnemyLife[2])
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && EnemyLife[1]) //
				{
					Enemy->BonusDamageValue += UpDamage;
					Enemy->TakeDurahanHead = true;
				}
			}
		}
		else if (!EnemyLife[1] && EnemyLife[2])
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr && Enemy->ActorHasTag("Enemy2") && EnemyLife[2]) //
				{
					Enemy->BonusDamageValue += UpDamage;
					Enemy->TakeDurahanHead = true;
				}
			}
		}
	}

}

bool UUI_PlayWindow::ChronicitySelect()
{
	//콤보 카드 포함 여부 검사
	bool gs = false;

	if (DrawDeck.Contains(13) || DumpDeck.Contains(13))
	{
		ChronicityDeck.Add(13);
		gs = true;
	}
	if (DrawDeck.Contains(14) || DumpDeck.Contains(14))
	{
		ChronicityDeck.Add(14);
		gs = true;
	}
	if (DrawDeck.Contains(15) || DumpDeck.Contains(15))
	{
		ChronicityDeck.Add(15);
		gs = true;
	}
	if (DrawDeck.Contains(16) || DumpDeck.Contains(16))
	{
		ChronicityDeck.Add(16);
		gs = true;
	}
	if (DrawDeck.Contains(17) || DumpDeck.Contains(17))
	{
		ChronicityDeck.Add(17);
		gs = true;
	}
	if (DrawDeck.Contains(18) || DumpDeck.Contains(18))
	{
		ChronicityDeck.Add(18);
		gs = true;
	}
	if (DrawDeck.Contains(25) || DumpDeck.Contains(25))
	{
		ChronicityDeck.Add(25);
		gs = true;
	}
	if (DrawDeck.Contains(26) || DumpDeck.Contains(26))
	{
		ChronicityDeck.Add(26);
		gs = true;
	}
	if (DrawDeck.Contains(27) || DumpDeck.Contains(27))
	{
		ChronicityDeck.Add(27);
		gs = true;
	}
	if (DrawDeck.Contains(31) || DumpDeck.Contains(31))
	{
		ChronicityDeck.Add(31);
		gs = true;
	}
	if (DrawDeck.Contains(35) || DumpDeck.Contains(35))
	{
		ChronicityDeck.Add(35);
		gs = true;
	}
	if (DrawDeck.Contains(50) || DumpDeck.Contains(50))
	{
		ChronicityDeck.Add(50);
		gs = true;
	}
	if (DrawDeck.Contains(51) || DumpDeck.Contains(51))
	{
		ChronicityDeck.Add(51);
		gs = true;
	}
	if (DrawDeck.Contains(41) || DumpDeck.Contains(41))
	{
		ChronicityDeck.Add(41);
		gs = true;
	}
	if (DrawDeck.Contains(42) || DumpDeck.Contains(42))
	{
		ChronicityDeck.Add(42);
		gs = true;
	}
	if (DrawDeck.Contains(43) || DumpDeck.Contains(43))
	{
		ChronicityDeck.Add(43);
		gs = true;
	}
	if (DrawDeck.Contains(113) || DumpDeck.Contains(113))
	{
		ChronicityDeck.Add(113);
		gs = true;
	}
	if (DrawDeck.Contains(114) || DumpDeck.Contains(114))
	{
		ChronicityDeck.Add(114);
		gs = true;
	}
	if (DrawDeck.Contains(115) || DumpDeck.Contains(115))
	{
		ChronicityDeck.Add(115);
		gs = true;
	}
	if (DrawDeck.Contains(116) || DumpDeck.Contains(116))
	{
		ChronicityDeck.Add(116);
		gs = true;
	}
	if (DrawDeck.Contains(117) || DumpDeck.Contains(117))
	{
		ChronicityDeck.Add(117);
		gs = true;
	}
	if (DrawDeck.Contains(118) || DumpDeck.Contains(118))
	{
		ChronicityDeck.Add(118);
		gs = true;
	}
	if (DrawDeck.Contains(125) || DumpDeck.Contains(125))
	{
		ChronicityDeck.Add(125);
		gs = true;
	}
	if (DrawDeck.Contains(126) || DumpDeck.Contains(126))
	{
		ChronicityDeck.Add(126);
		gs = true;
	}
	if (DrawDeck.Contains(127) || DumpDeck.Contains(127))
	{
		ChronicityDeck.Add(127);
		gs = true;
	}
	if (DrawDeck.Contains(131) || DumpDeck.Contains(131))
	{
		ChronicityDeck.Add(131);
		gs = true;
	}
	if (DrawDeck.Contains(135) || DumpDeck.Contains(135))
	{
		ChronicityDeck.Add(135);
		gs = true;
	}
	if (DrawDeck.Contains(150) || DumpDeck.Contains(150))
	{
		ChronicityDeck.Add(150);
		gs = true;
	}
	if (DrawDeck.Contains(151) || DumpDeck.Contains(151))
	{
		ChronicityDeck.Add(151);
		gs = true;
	}
	if (DrawDeck.Contains(141) || DumpDeck.Contains(141))
	{
		ChronicityDeck.Add(141);
		gs = true;
	}
	if (DrawDeck.Contains(142) || DumpDeck.Contains(142))
	{
		ChronicityDeck.Add(142);
		gs = true;
	}
	if (DrawDeck.Contains(143) || DumpDeck.Contains(143))
	{
		ChronicityDeck.Add(143);
		gs = true;
	}

	return gs;
}

void UUI_PlayWindow::LastStagePhase(int Phase)
{
	if (Phase == 0) // 1-1 클리어
	{
		EnemyCount = 3;
		LastOrder += 1;
		SoonSkullHead[1] = 0;
		SoonSkullHead[2] = 0;
		SoonSkullHead[3] = 0;
		EnemyHealthbar1->IamSkeleton = 0;
		EnemyHealthbar2->IamSkeleton = 0;
		EnemyHealthbar3->IamSkeleton = 0;
		EnemyHealthbar1->SkullReviveCool = 0;
		EnemyHealthbar1->Skullmode = false;
		EnemyHealthbar2->SkullReviveCool = 0;
		EnemyHealthbar2->Skullmode = false;
		EnemyHealthbar3->SkullReviveCool = 0;
		EnemyHealthbar3->Skullmode = false;
		Cleanze(1);
		Cleanze(2);
		Cleanze(3);

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1"))
				{
					EnemyLife[1] = true; // 다시 살리고

					Enemy->Tags[1] = FName("SkeletonSoldier");
					Enemy->Tags[2] = FName(" ");
					Enemy->SetStat();
					EnemyHealthbar1->Revive();
				}
				else if (Enemy->ActorHasTag("Enemy2"))
				{
					EnemyLife[2] = true; // 다시 살리고

					Enemy->Tags[1] = FName("Lich");
					Enemy->Tags[2] = FName(" ");
					Enemy->SetStat();
					EnemyHealthbar2->Revive();
				}
				else if (Enemy->ActorHasTag("Enemy3"))
				{
					EnemyLife[3] = true; // 다시 살리고

					Enemy->Tags[1] = FName("SkeletonSoldier");
					Enemy->Tags[2] = FName(" ");
					Enemy->SetStat();
					EnemyHealthbar3->Revive();
				}
			}
		}
	}
	else if (Phase == 1) // 1-2 클리어
	{
		EnemyCount = 3;
		LastOrder += 1;
		SoonSkullHead[1] = 0;
		SoonSkullHead[2] = 0;
		SoonSkullHead[3] = 0;
		EnemyHealthbar1->IamSkeleton = 0;
		EnemyHealthbar2->IamSkeleton = 0;
		EnemyHealthbar3->IamSkeleton = 0;
		EnemyHealthbar1->SkullReviveCool = 0;
		EnemyHealthbar1->Skullmode = false;
		EnemyHealthbar2->SkullReviveCool = 0;
		EnemyHealthbar2->Skullmode = false;
		EnemyHealthbar3->SkullReviveCool = 0;
		EnemyHealthbar3->Skullmode = false;
		Cleanze(1);
		Cleanze(2);
		Cleanze(3);

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1"))
				{
					EnemyLife[1] = true; // 다시 살리고

					Enemy->Tags[1] = FName("SkeletonArcher");
					Enemy->Tags[2] = FName(" ");
					Enemy->SetStat();
					EnemyHealthbar1->Revive();
				}
				else if (Enemy->ActorHasTag("Enemy2"))
				{
					EnemyLife[2] = true; // 다시 살리고

					Enemy->Tags[1] = FName("Durahan");
					Enemy->Tags[2] = FName(" ");
					Enemy->SetStat();
					EnemyHealthbar2->Revive();
				}
				else if (Enemy->ActorHasTag("Enemy3"))
				{
					EnemyLife[3] = true; // 다시 살리고

					Enemy->Tags[1] = FName("SkeletonArcher");
					Enemy->Tags[2] = FName(" ");
					Enemy->SetStat();
					EnemyHealthbar3->Revive();
				}
			}
		}
	}
	else if (Phase == 2) // 1-3 클리어
	{
		AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		AController_MainCharacter* Controller = Cast<AController_MainCharacter>(GetOwningPlayer());
		if (Controller)
		{
			Controller->ShowDialogueWidget(4);
		}

		EnemyCount = 1;
		LastOrder += 1;
		SoonSkullHead[1] = 0;
		SoonSkullHead[2] = 0;
		SoonSkullHead[3] = 0;
		EnemyHealthbar1->IamSkeleton = 0;
		EnemyHealthbar2->IamSkeleton = 0;
		EnemyHealthbar3->IamSkeleton = 0;
		EnemyHealthbar1->SkullReviveCool = 0;
		EnemyHealthbar1->Skullmode = false;
		EnemyHealthbar2->SkullReviveCool = 0;
		EnemyHealthbar2->Skullmode = false;
		EnemyHealthbar3->SkullReviveCool = 0;
		EnemyHealthbar3->Skullmode = false;
		Cleanze(1);
		Cleanze(2);
		Cleanze(3);

		KingHealthText->SetVisibility(ESlateVisibility::Visible);
		KingHealthBar->SetVisibility(ESlateVisibility::Visible);
		Image_Buffmob->SetBrush(ImgKing2);

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1"))
				{
					EnemyLife[1] = true; // 다시 살리고

					Enemy->Tags[1] = FName("MagicBall");
					Enemy->Tags[2] = FName(" ");
					Enemy->SetStat();
					EnemyHealthbar1->Revive();
				}
				else if (Enemy->ActorHasTag("Enemy2"))
				{
					EnemyLife[2] = true; // 다시 살리고

					Enemy->Tags[1] = FName("MagicBall");
					Enemy->Tags[2] = FName(" ");
					Enemy->SetStat();
					EnemyHealthbar2->Revive();
				}
				else if (Enemy->ActorHasTag("Enemy3"))
				{
					EnemyLife[3] = true; // 다시 살리고

					Enemy->Tags[1] = FName("MagicBall");
					Enemy->Tags[2] = FName(" ");
					Enemy->SetStat();
					EnemyHealthbar3->Revive();
				}
			}
		}
	}
	else if (Phase == 3) // 2 클리어
	{
		AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		AController_MainCharacter* Controller = Cast<AController_MainCharacter>(GetOwningPlayer());
		if (Controller)
		{
			Controller->ShowDialogueWidget(5);
		}

		EnemyCount = 3;
		LastOrder += 1;
		SoonSkullHead[1] = 0;
		SoonSkullHead[2] = 0;
		SoonSkullHead[3] = 0;
		EnemyHealthbar1->IamSkeleton = 0;
		EnemyHealthbar2->IamSkeleton = 0;
		EnemyHealthbar3->IamSkeleton = 0;
		EnemyHealthbar1->SkullReviveCool = 0;
		EnemyHealthbar1->Skullmode = false;
		EnemyHealthbar2->SkullReviveCool = 0;
		EnemyHealthbar2->Skullmode = false;
		EnemyHealthbar3->SkullReviveCool = 0;
		EnemyHealthbar3->Skullmode = false;
		EnemyHealthbar1->IamBall = false;
		EnemyHealthbar2->IamBall = false;
		EnemyHealthbar3->IamBall = false;
		Cleanze(1);
		Cleanze(2);
		Cleanze(3);

		KingHealthText->SetVisibility(ESlateVisibility::Collapsed);
		KingHealthBar->SetVisibility(ESlateVisibility::Collapsed);
		Image_Buffmob->SetVisibility(ESlateVisibility::Collapsed);

		PlayAnimation(Anim_Cerberus);
		Image_Cerberus->SetBrush(ImgDemonLordBody);
		Image_demonhead->SetVisibility(ESlateVisibility::Visible);
		Image_demonleft->SetVisibility(ESlateVisibility::Visible);
		Image_demonright->SetVisibility(ESlateVisibility::Visible);

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1"))
				{
					EnemyLife[1] = true; // 다시 살리고

					Enemy->Tags[1] = FName("DemonLordRight");
					Enemy->Tags[2] = FName(" ");
					Enemy->SetStat();
					EnemyHealthbar1->Revive();
				}
				else if (Enemy->ActorHasTag("Enemy2"))
				{
					EnemyLife[2] = true; // 다시 살리고

					Enemy->Tags[1] = FName("DemonLordHead");
					Enemy->Tags[2] = FName(" ");
					Enemy->SetStat();
					EnemyHealthbar2->Revive();
				}
				else if (Enemy->ActorHasTag("Enemy3"))
				{
					EnemyLife[3] = true; // 다시 살리고

					Enemy->Tags[1] = FName("DemonLordLeft");
					Enemy->Tags[2] = FName(" ");
					Enemy->SetStat();
					EnemyHealthbar3->Revive();
				}
			}
		}
	}
	else if (Phase == 4) // 2 클리어
	{
		AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		AController_MainCharacter* Controller = Cast<AController_MainCharacter>(GetOwningPlayer());
		if (Controller)
		{
			Controller->ShowDialogueWidget(6);
		}

		return;
	}
}

void UUI_PlayWindow::UpdateKingHealth()
{
	FString NewWString = FString::FromInt(KingHP);
	FText InWText = FText::FromString(NewWString);
	KingHealthText->SetText(InWText);

	float PerW = float(KingHP) / float(100);
	KingHealthBar->SetPercent(PerW);
}

void UUI_PlayWindow::MawangRevive()
{
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (EnemyHealthbar1->IamDemonLord && EnemyHealthbar1->MawangReviveCool >= EnemyHealthbar1->MawangReviveTime)
			{ // 부활할 턴(2턴)이 왔다면
				if (Enemy->ActorHasTag("Enemy1")) // 몹1
				{
					EnemyHealthbar1->MawangReviveCool = 0; // 변수 초기화
					EnemyCount += 1;
					EnemyLife[1] = true;

					Image_demonright->SetVisibility(ESlateVisibility::Visible);

					Enemy->ReviveMawang();
					EnemyHealthbar1->Revive();
				}
			}
			else if (EnemyHealthbar2->IamDemonLord && EnemyHealthbar2->MawangReviveCool >= EnemyHealthbar1->MawangReviveTime)
			{ // 부활할 턴(2턴)이 왔다면
				if (Enemy->ActorHasTag("Enemy2")) // 몹2
				{
					EnemyHealthbar2->MawangReviveCool = 0; // 변수 초기화
					EnemyCount += 1;
					EnemyLife[2] = true;

					Image_demonhead->SetVisibility(ESlateVisibility::Visible);

					Enemy->ReviveMawang();
					EnemyHealthbar2->Revive();
				}
			}
			else if (EnemyHealthbar3->IamDemonLord && EnemyHealthbar3->MawangReviveCool >= EnemyHealthbar1->MawangReviveTime)
			{ // 부활할 턴(2턴)이 왔다면
				if (Enemy->ActorHasTag("Enemy3")) // 몹1
				{
					EnemyHealthbar3->MawangReviveCool = 0; // 변수 초기화
					EnemyCount += 1;
					EnemyLife[3] = true;

					Image_demonleft->SetVisibility(ESlateVisibility::Visible);

					Enemy->ReviveMawang();
					EnemyHealthbar3->Revive();
				}
			}
		}
	}
}

void UUI_PlayWindow::LoseKingHP(int value)
{
	KingHP -= value;
	if (KingHP < 0)
	{
		KingHP = 0;
	}
	UpdateKingHealth();
}

void UUI_PlayWindow::SpecialBoom() // 이펙트는 따로
{
	AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth; // 풀피 회복

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		MyPlayer->MyLife = true;
		MyPlayer->UpdateHealth();
	}

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (Enemy->ActorHasTag("Enemy1"))
			{
				EnemyHealthbar1->GetDamaged(50);
				EnemyHealthbar1->DamageEffect(realthisturn, 1, true, Enemy->GetStackCorrosion, Enemy->GetStackWeakening, Enemy->GetStackBlooding);
			}
			else if (Enemy->ActorHasTag("Enemy2"))
			{
				EnemyHealthbar2->GetDamaged(50);
				EnemyHealthbar2->DamageEffect(realthisturn, 1, true, Enemy->GetStackCorrosion, Enemy->GetStackWeakening, Enemy->GetStackBlooding);
			}
			else if (Enemy->ActorHasTag("Enemy3"))
			{
				EnemyHealthbar3->GetDamaged(50);
				EnemyHealthbar3->DamageEffect(realthisturn, 1, true, Enemy->GetStackCorrosion, Enemy->GetStackWeakening, Enemy->GetStackBlooding);
			}
		}
	}
	
	CheckLife();
}

bool UUI_PlayWindow::CheckTurnClear()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (EnemyCount == 1 && FieldinAsura) // 필드에 아수라 있고, 적 카운트가 1이면, 부활
	{
		return true;
	}
	else if (FieldinWarlock && !EnemyLife[1] && !EnemyLife[3]) // 흑마 1페이즈에서 1,3적이 죽었으면
	{
		return true;
	}
	else if (EnemyCount == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UUI_PlayWindow::Cleanze(int MobNum)
{
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (Enemy->ActorHasTag("Enemy1") && MobNum == 1)
			{
				Enemy->GetStackBlooding = 0;
				Enemy->GetStackWeakening = 0;
				Enemy->GetStackCorrosion = 0;
			}
			else if (Enemy->ActorHasTag("Enemy2") && MobNum == 2)
			{
				Enemy->GetStackBlooding = 0;
				Enemy->GetStackWeakening = 0;
				Enemy->GetStackCorrosion = 0;
			}
			else if (Enemy->ActorHasTag("Enemy3") && MobNum == 3)
			{
				Enemy->GetStackBlooding = 0;
				Enemy->GetStackWeakening = 0;
				Enemy->GetStackCorrosion = 0;
			}
		}
	}
}

void UUI_PlayWindow::OnButtonEndClicked() // 나중에 숨고르기 or 추가 효과 생기면 갱신할 예정
{
	if (Attack[1] == true && Attack[2] == true && Attack[3] == true)
	{
		AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
		MyController->FreezeStart();

		FString sStr = FString::Printf(TEXT("턴 종료"));
		Text_Notify->SetText(FText::FromString(sStr));

		if (DeckFront)
		{
			PlayAnimation(Anim_TurnEnd);
		}
		else
		{
			PlayAnimation(Anim_TurnEnd2);
		}
	}
	else
	{
		Text_Popup->SetVisibility(ESlateVisibility::Visible);
		Text_Popup->SetRenderOpacity(1);
		Image_Popup->SetVisibility(ESlateVisibility::Visible);
		Image_Popup->SetRenderOpacity(1);
		Button_PopupYes->SetVisibility(ESlateVisibility::Visible);
		Button_PopupYes->SetRenderOpacity(1);
		Button_PopupNo->SetVisibility(ESlateVisibility::Visible);
		Button_PopupNo->SetRenderOpacity(1);
	}
}

void UUI_PlayWindow::TurnClear()
{
	UE_LOG(LogTemp, Log, TEXT("EnemyCount :: %d"), EnemyCount);
	if (EnemyCount == 0)
	{
		if (LastStage)
		{
			LastStagePhase(LastOrder);
		}
		else
		{
			StageClear();
		}
	}

	CombinationBonusDraw = 2; // 기본기 연마 추가 드로우 상한 초기화

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	int k = CardBox->GetChildrenCount(); // k = 손패 카드 수
	int ks = CardBox2->GetChildrenCount(); // k = 손패 카드 수
	for (int x = 0; x < ks; x++)
	{
		CardBox2->RemoveChildAt(0); // 손패 카드들 삭제
	}
	for (int x = 0; x < k; x++)
	{
		CardBox->RemoveChildAt(0); // 손패 카드들 삭제
	}
	
	int y = HandCard.Num(); // y = 진행했던 턴에 받은 카드 수
	for (int x = 0; x < y; x++)
	{
		DumpDeck.Add(HandCard[x]); // 이번 턴에 받은 카드들을 버린 덱으로 추가
	}
	HandCard.Empty();

	MyPlayer->Ironmode = false;
	Attack = { false, false, false, false };
	MobDone = { 0, 0, 0, 0 };
	MobTargetCard = { 0, 0, 0, 0 };
	MobTargetCost = { 0, 0, 0, 0 };
	MobTargetFixCost = { 0, 0, 0, 0 };
	EnemyAttackOrder = { 0, 2, 2, 2 };
	CurrentCost = 3;
	ChangeAttackImage(1, -1, 0);
	ChangeAttackImage(2, -1, 0);
	ChangeAttackImage(3, -1, 0);

	if (OnCombo_Extinction_CostDown1)
	{
		ComboOnByAttackOrder_Extinction[0] = true;
	}
	else
	{
		ComboOnByAttackOrder_Extinction[0] = false;
	}
	ComboOnByAttackOrder_Extinction[1] = false; ComboOnByAttackOrder_Extinction[2] = false; ComboOnByAttackOrder_Extinction[3] = false;

	if (OnCombo_Triangle_CostDown1)
	{
		ComboOnByAttackOrder_Triangle[0] = true;
	}
	else
	{
		ComboOnByAttackOrder_Triangle[0] = false;
	}
	ComboOnByAttackOrder_Triangle[1] = false; ComboOnByAttackOrder_Triangle[2] = false; ComboOnByAttackOrder_Triangle[3] = false;

	CardNumByAttackOrder[0] = 0; CardNumByAttackOrder[1] = 0; CardNumByAttackOrder[2] = 0; CardNumByAttackOrder[3] = 0;



	if (Breathe[1] == true || Breathe[2] == true || Breathe[3] == true)
	{
		Breathe[0] = true;
	}
	Breathe[1] = false;
	Breathe[2] = false;
	Breathe[3] = false;

	UpdateCost();

	//디버프 관련
	MyPlayer->AllBloodEnd = 0;
	WaitBlooding = false;
	MyPlayer->GetStackCorrosion -= 1;
	if (MyPlayer->GetStackCorrosion < 0)
	{
		MyPlayer->GetStackCorrosion = 0;
	}
	MyPlayer->GetStackWeakening -= 1;
	if (MyPlayer->GetStackWeakening < 0)
	{
		MyPlayer->GetStackWeakening = 0;
	}
	MyPlayer->GetStackBlooding -= 1;
	if (MyPlayer->GetStackBlooding < 0)
	{
		MyPlayer->GetStackBlooding = 0;
	}
	MyPlayer->AttackStackCorrosion = 0;
	MyPlayer->AttackStackWeakening = 0;
	MyPlayer->AttackStackBlooding = 0;
	MyPlayer->ConfirmedStackCorrosion = 0;
	MyPlayer->ConfirmedStackWeakening = 0;
	MyPlayer->ConfirmedStackBlooding = 0;

	MyPlayer->GetCharmed = { false, false, false, false };
	if (MyPlayer->GetStackCharmed[1])
	{
		MyPlayer->GetCharmed[1] = true;
		MyPlayer->GetCharmed[0] = true;
	}
	if (MyPlayer->GetStackCharmed[2])
	{
		MyPlayer->GetCharmed[2] = true;
		MyPlayer->GetCharmed[0] = true;
	}
	if (MyPlayer->GetStackCharmed[3])
	{
		MyPlayer->GetCharmed[3] = true;
		MyPlayer->GetCharmed[0] = true;
	}
	MyPlayer->GetStackCharmed = { false, false, false, false };

	//콤보 관련
	MyPlayer->ComboUsing_Triple = 0;
	MyPlayer->ComboUsing_JapHook = 0;
	MyPlayer->ComboUsing_Extinction = 0;
	MyPlayer->ComboConfirm_Triple = 0;
	MyPlayer->ComboConfirm_JapHook = 0;
	MyPlayer->ComboConfirm_Extinction = 0;

	MyHealthbar0->UpdateNagativeEffect(MyPlayer->GetStackCorrosion, MyPlayer->GetStackWeakening, MyPlayer->GetStackBlooding);

	// 아수라 특수 패턴
	if (EnemyCount == 1 && FieldinAsura) // 필드에 아수라 있고, 적 카운트가 1이면, 부활
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && Enemy->ActorHasTag("Enemy2") && (Enemy->ActorHasTag("Asura") || Enemy->ActorHasTag("Asura_Up")) && !(Enemy->AsuraRevivemode))
			{
				EnemyLife[2] = true;
				Enemy->AsuraRevivemode = true;
				Enemy->Revive();
			}
		}
	}
	// 흑마 특수 패턴
	WarlockPhase1Revive();

	if (MyPlayer->ActorHasTag("Dungeon4_3")) // 던전 4-3이면
	{
		if (!EnemyLife[1] && EnemyLife[2])
		{
			if (EnemyHealthbar1->IamSkeleton == 1)
			{
				EnemyHealthbar2->UpdateHealthBar(-40);
			}
			else if (EnemyHealthbar1->IamSkeleton == 2)
			{
				EnemyHealthbar2->UpdateHealthBar(-25);
			}
			else if (EnemyHealthbar1->IamSkeleton == 3)
			{
				EnemyHealthbar2->UpdateHealthBar(-15);
			}

			if (EnemyHealthbar2->CurrentHealth <= 0) // 이 피해로 군주가 죽으면
			{
				EnemyCount -= 1;
				EnemyLife[2] = false;

				if (EnemyCount == 0)
				{
					StageClear();
				}
			}
			else // 안 죽으면
			{
				EnemyLife[1] = true;
				EnemyCount += 1;

				int ld = dist2(rng1);

				if (ld == 0)
				{
					for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
					{
						AEnemyCharacter* Enemy = *Iter;

						if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->Tags[1] = FName("SkeletonSoldier");
							Enemy->SetStat();
							Enemy->Revive();
						}
					}
				}
				else if (ld == 1)
				{
					for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
					{
						AEnemyCharacter* Enemy = *Iter;

						if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->Tags[1] = FName("SkeletonArcher");
							Enemy->SetStat();
							Enemy->Revive();
						}
					}
				}
				else if (ld == 2)
				{
					for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
					{
						AEnemyCharacter* Enemy = *Iter;

						if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->Tags[1] = FName("SkeletonGuardDog");
							Enemy->SetStat();
							Enemy->Revive();
						}
					}
				}
			}
		}

		if (!EnemyLife[3] && EnemyLife[2])
		{
			if (EnemyHealthbar3->IamSkeleton == 1)
			{
				EnemyHealthbar2->UpdateHealthBar(-40);
			}
			else if (EnemyHealthbar3->IamSkeleton == 2)
			{
				EnemyHealthbar2->UpdateHealthBar(-25);
			}
			else if (EnemyHealthbar3->IamSkeleton == 3)
			{
				EnemyHealthbar2->UpdateHealthBar(-15);
			}

			if (EnemyHealthbar2->CurrentHealth <= 0) // 이 피해로 군주가 죽으면
			{
				EnemyCount -= 1;
				EnemyLife[2] = false;

				if (EnemyCount == 0)
				{
					StageClear();
				}
			}
			else // 안 죽으면
			{
				EnemyLife[3] = true;
				EnemyCount += 1;

				int ld = dist2(rng1);

				if (ld == 0)
				{
					for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
					{
						AEnemyCharacter* Enemy = *Iter;

						if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->Tags[1] = FName("SkeletonSoldier");
							Enemy->SetStat();
							Enemy->Revive();
						}
					}
				}
				else if (ld == 1)
				{
					for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
					{
						AEnemyCharacter* Enemy = *Iter;

						if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->Tags[1] = FName("SkeletonArcher");
							Enemy->SetStat();
							Enemy->Revive();
						}
					}
				}
				else if (ld == 2)
				{
					for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
					{
						AEnemyCharacter* Enemy = *Iter;

						if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->Tags[1] = FName("SkeletonGuardDog");
							Enemy->SetStat();
							Enemy->Revive();
						}
					}
				}
			}
		}
	}
	else
	{
		// 적이 해골머리 상태면 부활 쿨 업데이트
		if (EnemyHealthbar1->Skullmode && EnemyLife[1])
		{
			EnemyHealthbar1->SkullReviveCool += 1;
		}
		else if (EnemyHealthbar2->Skullmode && EnemyLife[2])
		{
			EnemyHealthbar2->SkullReviveCool += 1;
		}
		else if (EnemyHealthbar3->Skullmode && EnemyLife[3])
		{
			EnemyHealthbar3->SkullReviveCool += 1;
		}

		// 해골 특수 패턴
		SkullRevive();
	}

	// 마왕 부활 패턴
	if (EnemyHealthbar1->IamDemonLord > 0 && !EnemyLife[1])
	{
		EnemyHealthbar1->MawangReviveCool += 1;
	}
	else if (EnemyHealthbar2->IamDemonLord > 0 && !EnemyLife[2])
	{
		EnemyHealthbar2->MawangReviveCool += 1;
	}
	else if (EnemyHealthbar3->IamDemonLord > 0 && !EnemyLife[3])
	{
		EnemyHealthbar3->MawangReviveCool += 1;
	}

	// 마왕 특수 패턴
	MawangRevive();

	// 흑마 부활 스킬
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && Enemy->LichReviveSkill > 0) // 리치가 누굴 부활시킬려고 한다면
		{
			for (int i = 1; i < 4; i++)
			{
				if (Enemy->ActorHasTag(FName(*("Enemy" + FString::FromInt(i)))))
				{
					if (EnemyLife[i]) // 살아있으면
					{
						LichReviveSkill(Enemy->LichReviveSkill);
						Enemy->LichReviveSkill = 0; // 변수 초기화
					}
				}
			}
		}
	}

	// 백인대장 기믹
	SkeletonFlag();

	int lor = 0; // 기존 머리 공격력
	// 듀라한 머리 공격력 올리기
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && Enemy->ActorHasTag("Durahan"))
		{
			lor = Enemy->DurahanHead;

			if (Enemy->DurahanBuffUp)
			{
				Enemy->DurahanHead += 5;
			}

			Enemy->DurahanTryUp = false;
			Enemy->DurahanBuffUp = false;
		}
	}
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 머리 회수
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (Enemy->TakeDurahanHead)
			{
				Enemy->TakeDurahanHead = false;
				Enemy->BonusDamageValue -= lor;
			}
			Enemy->DurahanParryingMode = false; // 힘 겨루기 모드 초기화
		}
	}

	DurahanGiveHead();

	//왕 2페이즈 마법구 기믹
	if (LastOrder == 3) // 왕 2페이즈일때
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				Enemy->Revive();
			}
		}

		EnemyLife[1] = true;
		EnemyLife[2] = true;
		EnemyLife[3] = true;
		Cleanze(1);
		Cleanze(2);
		Cleanze(3);
	}

	// 새로운 턴을 맞이하기 전 적 개체의 각종 데이터 정리
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			Enemy->ChangeBreatheEffect(false);
			Enemy->ChangeWidgetImage(0, 0);
			Enemy->GetStackCorrosion -= 1;
			if (Enemy->GetStackCorrosion < 0)
			{
				Enemy->GetStackCorrosion = 0;
			}
			Enemy->GetStackWeakening -= 1;
			if (Enemy->GetStackWeakening < 0)
			{
				Enemy->GetStackWeakening = 0;
			}
			Enemy->GetStackBlooding -= 1;
			if (Enemy->GetStackBlooding < 0)
			{
				Enemy->GetStackBlooding = 0;
			}
			Enemy->AttackStackCorrosion = 0;
			Enemy->AttackStackWeakening = 0;
			Enemy->AttackStackBlooding = 0;
			Enemy->ConfirmedStackCorrosion = 0;
			Enemy->ConfirmedStackWeakening = 0;
			Enemy->ConfirmedStackBlooding = 0;
			Enemy->ConfirmedDebuffType = 0; // 1:부식, 2:약화, 3:출혈
			Enemy->ConfirmedDebuffValue = 0;
			Enemy->ChangeDamage(); // 패턴 랜덤 변화
			Enemy->UpdateNagativeEffect();
		}
	}

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			for (int i = 1; i < 4; i++)
			{
				if (Enemy->ActorHasTag(FName(*("Enemy" + FString::FromInt(i)))))
				{
					EnemyAttackOrder[i] = Enemy->AttackOrder;
				}
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("TurnClear"));
	TurnStart();
}

void UUI_PlayWindow::StageClear()
{
	if (LastStage)
	{
		if (!LastClear)
		{
			LastStagePhase(LastOrder);
			return;
		}
	}

	StopAllAnimations();
	TurnStartEnded();

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());

	if (MyPlayer != nullptr && PlayerController != nullptr)
	{
		if (MyPlayer->ActorHasTag("Stage1_1"))
		{
			if (AController_MainCharacter::MyProgress[1] == false)
			{
				AForgottenKnightCharacter::MyGold += 15;
				AForgottenKnightCharacter::MyCurrentEXP += 6;
				AController_MainCharacter::MyProgress[1] = true;

				PlayerController->ShowStageClearWidget(6, 15);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 3;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 3);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage1_2"))
		{
			if (AController_MainCharacter::MyProgress[2] == false)
			{
				AForgottenKnightCharacter::MyGold += 19;
				AForgottenKnightCharacter::MyCurrentEXP += 10;
				AController_MainCharacter::MyProgress[2] = true;

				PlayerController->ShowStageClearWidget(10, 19);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 4;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 4);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage1_3"))
		{
			if (AController_MainCharacter::MyProgress[3] == false)
			{
				AForgottenKnightCharacter::MyGold += 24;
				AForgottenKnightCharacter::MyCurrentEXP += 12;
				AController_MainCharacter::MyProgress[3] = true;

				PlayerController->ShowStageClearWidget(12, 24);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 5;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 5);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage1_4"))
		{
			if (AController_MainCharacter::MyProgress[4] == false)
			{
				AForgottenKnightCharacter::MyGold += 27;
				AForgottenKnightCharacter::MyCurrentEXP += 16;
				AController_MainCharacter::MyProgress[4] = true;

				PlayerController->ShowStageClearWidget(16, 27);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 5;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 5);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage1_5"))
		{
			if (AController_MainCharacter::MyProgress[5] == false)
			{
				AForgottenKnightCharacter::MyGold += 50;
				AForgottenKnightCharacter::MyCurrentEXP += 30;
				AController_MainCharacter::MyProgress[5] = true;

				AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth; // 보스전 전체 회복 1회

				PlayerController->ShowStageClearWidget(30, 50);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 10;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 10);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage1_6"))
		{
			if (AController_MainCharacter::MyProgress[6] == false)
			{
				AForgottenKnightCharacter::MyGold += 32;
				AForgottenKnightCharacter::MyCurrentEXP += 36;
				AController_MainCharacter::MyProgress[6] = true;

				PlayerController->ShowStageClearWidget(36, 32);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 6;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 6);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage1_7"))
		{
			if (AController_MainCharacter::MyProgress[7] == false)
			{
				AForgottenKnightCharacter::MyGold += 45;
				AForgottenKnightCharacter::MyCurrentEXP += 30;
				AController_MainCharacter::MyProgress[7] = true;

				PlayerController->ShowStageClearWidget(30, 45);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 9;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 9);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage1_8"))
		{
			if (AController_MainCharacter::MyProgress[8] == false)
			{
				AForgottenKnightCharacter::MyGold += 55;
				AForgottenKnightCharacter::MyCurrentEXP += 40;
				AController_MainCharacter::MyProgress[8] = true;

				PlayerController->ShowStageClearWidget(40, 55);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 11;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 11);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage1_9"))
		{
			if (AController_MainCharacter::MyProgress[9] == false)
			{
				AForgottenKnightCharacter::MyGold += 85;
				AForgottenKnightCharacter::MyCurrentEXP += 42;
				AController_MainCharacter::MyProgress[9] = true;

				PlayerController->ShowStageClearWidget(42, 85);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 17;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 17);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage1_10"))
		{
			if (!Dial1st)
			{
				PlayerController->ShowDialogueWidget(2);
			}
			else
			{
				if (AController_MainCharacter::MyProgress[10] == false)
				{
					AForgottenKnightCharacter::MyGold += 150;
					AForgottenKnightCharacter::MyCurrentEXP += 50;
					AController_MainCharacter::MyProgress[10] = true;

					AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth; // 보스전 전체 회복 1회

					PlayerController->ShowStageClearWidget(50, 150);
				}
				else
				{
					AForgottenKnightCharacter::MyGold += 30;
					AForgottenKnightCharacter::MyCurrentEXP += 0;

					PlayerController->ShowStageClearWidget(0, 30);
				}
			}
		}
		else if (MyPlayer->ActorHasTag("Stage2_1"))
		{
			if (AController_MainCharacter::MyProgress[11] == false)
			{
				AForgottenKnightCharacter::MyGold += 120;
				AForgottenKnightCharacter::MyCurrentEXP += 75;
				AController_MainCharacter::MyProgress[11] = true;

				PlayerController->ShowStageClearWidget(75, 120);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 24;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 24);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage2_2"))
		{
			if (AController_MainCharacter::MyProgress[12] == false)
			{
				AForgottenKnightCharacter::MyGold += 120;
				AForgottenKnightCharacter::MyCurrentEXP += 75;
				AController_MainCharacter::MyProgress[12] = true;

				PlayerController->ShowStageClearWidget(75, 120);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 24;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 24);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage2_3"))
		{
			if (AController_MainCharacter::MyProgress[13] == false)
			{
				AForgottenKnightCharacter::MyGold += 120;
				AForgottenKnightCharacter::MyCurrentEXP += 75;
				AController_MainCharacter::MyProgress[13] = true;

				PlayerController->ShowStageClearWidget(75, 120);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 24;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 24);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage2_4"))
		{
			if (AController_MainCharacter::MyProgress[14] == false)
			{
				AForgottenKnightCharacter::MyGold += 180;
				AForgottenKnightCharacter::MyCurrentEXP += 100;
				AController_MainCharacter::MyProgress[14] = true;

				PlayerController->ShowStageClearWidget(100, 180);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 36;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 36);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage2_5"))
		{
			if (AController_MainCharacter::MyProgress[15] == false)
			{
				AForgottenKnightCharacter::MyGold += 280;
				AForgottenKnightCharacter::MyCurrentEXP += 150;
				AController_MainCharacter::MyProgress[15] = true;

				AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth; // 보스전 전체 회복 1회

				PlayerController->ShowStageClearWidget(150, 280);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 56;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 56);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage2_6"))
		{
			if (AController_MainCharacter::MyProgress[16] == false)
			{
				AForgottenKnightCharacter::MyGold += 450;
				AForgottenKnightCharacter::MyCurrentEXP += 150;
				AController_MainCharacter::MyProgress[16] = true;

				PlayerController->ShowStageClearWidget(150, 450);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 90;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 90);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage2_7"))
		{
			if (AController_MainCharacter::MyProgress[17] == false)
			{
				AForgottenKnightCharacter::MyGold += 410;
				AForgottenKnightCharacter::MyCurrentEXP += 190;
				AController_MainCharacter::MyProgress[17] = true;

				PlayerController->ShowStageClearWidget(190, 410);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 82;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 82);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage2_8"))
		{
			if (AController_MainCharacter::MyProgress[18] == false)
			{
				AForgottenKnightCharacter::MyGold += 530;
				AForgottenKnightCharacter::MyCurrentEXP += 200;
				AController_MainCharacter::MyProgress[18] = true;

				PlayerController->ShowStageClearWidget(200, 530);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 106;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 106);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage2_9"))
		{
			if (AController_MainCharacter::MyProgress[19] == false)
			{
				AForgottenKnightCharacter::MyGold += 800;
				AForgottenKnightCharacter::MyCurrentEXP += 260;
				AController_MainCharacter::MyProgress[19] = true;

				PlayerController->ShowStageClearWidget(260, 800);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 160;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 160);
			}
		}
		else if (MyPlayer->ActorHasTag("Stage2_10"))
		{
			if (AController_MainCharacter::MyProgress[20] == false)
			{
				AForgottenKnightCharacter::MyGold += 1000;
				AForgottenKnightCharacter::MyCurrentEXP += 500;
				AController_MainCharacter::MyProgress[20] = true;

				PlayerController->ShowStageClearWidget(500, 1000);
			}
			else
			{
				AForgottenKnightCharacter::MyGold += 200;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowStageClearWidget(0, 200);
			}
		}


		if (MyPlayer->ActorHasTag("Dungeon1_1"))
		{
			if (AController_MainCharacter::EventDungeonData[1] == 0) // 첫클이면
			{
				AForgottenKnightCharacter::MyGold += 30;
				AForgottenKnightCharacter::MyCurrentEXP += 18;
				AController_MainCharacter::EventDungeonData[1] = 1;

				PlayerController->ShowDungeonClearWidget(18, 30, 0);
			}
			else if (AController_MainCharacter::EventDungeonData[1] > 0) // 아니면
			{
				AForgottenKnightCharacter::MyGold += 6;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowDungeonClearWidget(0, 6, 0);
			}
		}
		else if (MyPlayer->ActorHasTag("Dungeon1_2"))
		{
			if (AController_MainCharacter::EventDungeonData[1] == 1) // 첫클이면
			{
				AForgottenKnightCharacter::MyGold += 38;
				AForgottenKnightCharacter::MyCurrentEXP += 22;
				AController_MainCharacter::EventDungeonData[1] = 2;

				PlayerController->ShowDungeonClearWidget(22, 38, 0);
			}
			else if (AController_MainCharacter::EventDungeonData[1] > 1) // 아니면
			{
				AForgottenKnightCharacter::MyGold += 8;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowDungeonClearWidget(0, 8, 0);
			}
		}
		else if (MyPlayer->ActorHasTag("Dungeon1_3"))
		{
			if (AController_MainCharacter::EventDungeonData[1] == 2) // 첫클이면
			{
				AForgottenKnightCharacter::MyGold += 30;
				AForgottenKnightCharacter::MyCurrentEXP += 45;
				AController_MainCharacter::EventDungeonData[1] = 3;

				PlayerController->ShowDungeonClearWidget(45, 30, 1);
			}
			else if (AController_MainCharacter::EventDungeonData[1] > 2) // 아니면
			{
				AForgottenKnightCharacter::MyGold += 6;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowDungeonClearWidget(0, 5, -1);
			}
		}
		if (MyPlayer->ActorHasTag("Dungeon2_1"))
		{
			if (AController_MainCharacter::EventDungeonData[2] == 0) // 첫클이면
			{
				AForgottenKnightCharacter::MyGold += 68;
				AForgottenKnightCharacter::MyCurrentEXP += 38;
				AController_MainCharacter::EventDungeonData[2] = 1;

				PlayerController->ShowDungeonClearWidget(38, 68, 0);
			}
			else if (AController_MainCharacter::EventDungeonData[2] > 0) // 아니면
			{
				AForgottenKnightCharacter::MyGold += 14;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowDungeonClearWidget(0, 14, 0);
			}
		}
		else if (MyPlayer->ActorHasTag("Dungeon2_2"))
		{
			if (AController_MainCharacter::EventDungeonData[2] == 1) // 첫클이면
			{
				AForgottenKnightCharacter::MyGold += 83;
				AForgottenKnightCharacter::MyCurrentEXP += 43;
				AController_MainCharacter::EventDungeonData[2] = 2;

				PlayerController->ShowDungeonClearWidget(43, 83, 0);
			}
			else if (AController_MainCharacter::EventDungeonData[2] > 1) // 아니면
			{
				AForgottenKnightCharacter::MyGold += 17;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowDungeonClearWidget(0, 17, 0);
			}
		}
		else if (MyPlayer->ActorHasTag("Dungeon2_3"))
		{
			if (AController_MainCharacter::EventDungeonData[2] == 2) // 첫클이면
			{
				AForgottenKnightCharacter::MyGold += 65;
				AForgottenKnightCharacter::MyCurrentEXP += 40;
				AController_MainCharacter::EventDungeonData[2] = 3;

				PlayerController->ShowDungeonClearWidget(40, 65, 2);
			}
			else if (AController_MainCharacter::EventDungeonData[2] > 2) // 아니면
			{
				AForgottenKnightCharacter::MyGold += 13;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowDungeonClearWidget(0, 13, -1);
			}
		}
		if (MyPlayer->ActorHasTag("Dungeon3_1"))
		{
			if (AController_MainCharacter::EventDungeonData[3] == 0) // 첫클이면
			{
				AForgottenKnightCharacter::MyGold += 220;
				AForgottenKnightCharacter::MyCurrentEXP += 125;
				AController_MainCharacter::EventDungeonData[3] = 1;

				PlayerController->ShowDungeonClearWidget(125, 220, 0);
			}
			else if (AController_MainCharacter::EventDungeonData[3] > 0) // 아니면
			{
				AForgottenKnightCharacter::MyGold += 44;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowDungeonClearWidget(0, 44, 0);
			}
		}
		else if (MyPlayer->ActorHasTag("Dungeon3_2"))
		{
			if (AController_MainCharacter::EventDungeonData[3] == 1) // 첫클이면
			{
				AForgottenKnightCharacter::MyGold += 220;
				AForgottenKnightCharacter::MyCurrentEXP += 125;
				AController_MainCharacter::EventDungeonData[3] = 2;

				PlayerController->ShowDungeonClearWidget(125, 220, 0);
			}
			else if (AController_MainCharacter::EventDungeonData[3] > 1) // 아니면
			{
				AForgottenKnightCharacter::MyGold += 44;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowDungeonClearWidget(0, 44, 0);
			}
		}
		else if (MyPlayer->ActorHasTag("Dungeon3_3"))
		{
			if (AController_MainCharacter::EventDungeonData[3] == 2) // 첫클이면
			{
				AForgottenKnightCharacter::MyGold += 340;
				AForgottenKnightCharacter::MyCurrentEXP += 175;
				AController_MainCharacter::EventDungeonData[3] = 3;

				PlayerController->ShowDungeonClearWidget(175, 340, 3);
			}
			else if (AController_MainCharacter::EventDungeonData[3] > 2) // 아니면
			{
				AForgottenKnightCharacter::MyGold += 68;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowDungeonClearWidget(0, 68, -1);
			}
		}
		if (MyPlayer->ActorHasTag("Dungeon4_1"))
		{
			if (AController_MainCharacter::EventDungeonData[4] == 0) // 첫클이면
			{
				AForgottenKnightCharacter::MyGold += 390;
				AForgottenKnightCharacter::MyCurrentEXP += 210;
				AController_MainCharacter::EventDungeonData[4] = 1;

				PlayerController->ShowDungeonClearWidget(210, 390, 0);
			}
			else if (AController_MainCharacter::EventDungeonData[4] > 0) // 아니면
			{
				AForgottenKnightCharacter::MyGold += 78;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowDungeonClearWidget(0, 78, 0);
			}
		}
		else if (MyPlayer->ActorHasTag("Dungeon4_2"))
		{
			if (AController_MainCharacter::EventDungeonData[4] == 1) // 첫클이면
			{
				AForgottenKnightCharacter::MyGold += 630;
				AForgottenKnightCharacter::MyCurrentEXP += 230;
				AController_MainCharacter::EventDungeonData[4] = 2;

				PlayerController->ShowDungeonClearWidget(230, 630, 0);
			}
			else if (AController_MainCharacter::EventDungeonData[4] > 1) // 아니면
			{
				AForgottenKnightCharacter::MyGold += 126;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowDungeonClearWidget(0, 126, 0);
			}
		}
		else if (MyPlayer->ActorHasTag("Dungeon4_3"))
		{
			if (AController_MainCharacter::EventDungeonData[4] == 2) // 첫클이면
			{
				AForgottenKnightCharacter::MyGold += 100;
				AForgottenKnightCharacter::MyCurrentEXP += 50;
				AController_MainCharacter::EventDungeonData[4] = 3;

				PlayerController->ShowDungeonClearWidget(50, 100, 4);
			}
			else if (AController_MainCharacter::EventDungeonData[4] > 2) // 아니면
			{
				AForgottenKnightCharacter::MyGold += 20;
				AForgottenKnightCharacter::MyCurrentEXP += 0;

				PlayerController->ShowDungeonClearWidget(0, 20, -1);
			}
		}
	}

}

void UUI_PlayWindow::StageDie()
{
	StopAllAnimations();
	TurnStartEnded();

	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowStageDieWidget(EnterHP);
	}
}

void UUI_PlayWindow::OnButtonPopupYesClicked()
{
	Text_Popup->SetVisibility(ESlateVisibility::Collapsed);
	Text_Popup->SetRenderOpacity(0);
	Image_Popup->SetVisibility(ESlateVisibility::Collapsed);
	Image_Popup->SetRenderOpacity(0);
	Button_PopupYes->SetVisibility(ESlateVisibility::Collapsed);
	Button_PopupYes->SetRenderOpacity(0);
	Button_PopupNo->SetVisibility(ESlateVisibility::Collapsed);
	Button_PopupNo->SetRenderOpacity(0);

	int x = 0; // 공격하지 않는 횟수 계산
	for (int i = 1; i < 4; i++)
	{
		if (!Attack[i])
		{
			x += 1;
		}
	}

	if (x > 0)
	{
		for (int i = 0; i < x; i++)
		{
			OnButtonBreathClicked();
		}
	}

	OnButtonEndClicked();
}

void UUI_PlayWindow::OnButtonPopupNoClicked()
{
	Text_Popup->SetVisibility(ESlateVisibility::Collapsed);
	Text_Popup->SetRenderOpacity(0);
	Image_Popup->SetVisibility(ESlateVisibility::Collapsed);
	Image_Popup->SetRenderOpacity(0);
	Button_PopupYes->SetVisibility(ESlateVisibility::Collapsed);
	Button_PopupYes->SetRenderOpacity(0);
	Button_PopupNo->SetVisibility(ESlateVisibility::Collapsed);
	Button_PopupNo->SetRenderOpacity(0);
}

void UUI_PlayWindow::OnButtonBehindHandClicked()
{
	if (CardBox2->RenderTransform.Translation.Y > 490)
	{
		PlayAnimation(Anim_TurnBox2);
		DeckFront = false;
	}
	
}

void UUI_PlayWindow::OnButtonFrontHandClicked()
{
	if (CardBox->RenderTransform.Translation.Y > 490)
	{
		PlayAnimation(Anim_TurnBox1);
		DeckFront = true;
	}
}

void UUI_PlayWindow::OnButtonFirstClicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Attack[3] == true || Attack[2] == true)
	{
		FString sStr = FString::Printf(TEXT("공격을 취소할 수 없습니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
		return;
	}
	if (Attack[1] == true && Attack[2] == false && Attack[3] == false)
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (MobDone[1] == 1 && Enemy->ActorHasTag("Enemy1"))
				{
					Enemy->ChangeWidgetImage(0, 0);
					Enemy->ChangeBreatheEffect(false);
					Attack[1] = false;
					MobDone[1] = 0;
					CurrentCost += MobTargetCost[1];
					ChangeAttackImage(1, -1, 0);

					if (MobTargetCard[1] == 11 || MobTargetCard[1] == 111) // 카드가 철괴면
					{
						MyPlayer->Ironmode = false;

						if (MobTargetCard[1] == 111)
						{
							IronHeal = false;
						}
					}

					if (CardWidget != nullptr)
					{
						UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
						UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);

						if (CardBox->GetChildrenCount() < 5)
						{
							UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[1]); // MobTargetCard[1]
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}
						else
						{
							UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[1]); // MobTargetCard[1]
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}

						MobTargetCost[1] = 0;
						MobTargetCard[1] = 0;
					}
				}
				else if (MobDone[2] == 1 && Enemy->ActorHasTag("Enemy2"))
				{
					Enemy->ChangeWidgetImage(0, 0);
					Enemy->ChangeBreatheEffect(false);
					Attack[1] = false;
					MobDone[2] = 0;
					CurrentCost += MobTargetCost[2];
					ChangeAttackImage(1, -1, 0);

					if (MobTargetCard[2] == 11 || MobTargetCard[2] == 111) // 카드가 철괴면
					{
						MyPlayer->Ironmode = false;

						if (MobTargetCard[2] == 111)
						{
							IronHeal = false;
						}
					}

					if (CardWidget != nullptr)
					{
						UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
						UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
						if (CardBox->GetChildrenCount() < 5)
						{
							UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[2]);
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}
						else
						{
							UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[2]);
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}

						MobTargetCost[2] = 0;
						MobTargetCard[2] = 0;
					}
				}
				else if (MobDone[3] == 1 && Enemy->ActorHasTag("Enemy3"))
				{
					Enemy->ChangeWidgetImage(0, 0);
					Enemy->ChangeBreatheEffect(false);
					Attack[1] = false;
					MobDone[3] = 0;
					CurrentCost += MobTargetCost[3];
					ChangeAttackImage(1, -1, 0);

					if (MobTargetCard[3] == 11 || MobTargetCard[3] == 111) // 카드가 철괴면
					{
						MyPlayer->Ironmode = false;

						if (MobTargetCard[3] == 111)
						{
							IronHeal = false;
						}
					}

					if (CardWidget != nullptr)
					{
						UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
						UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
						if (CardBox->GetChildrenCount() < 5)
						{
							UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[3]);
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}
						else
						{
							UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[3]);
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}

						MobTargetCost[3] = 0;
						MobTargetCard[3] = 0;
					}
				}
				else if (Breathe[1] == true)
				{
					if (Attack[2] == true)
					{
						if (MobDone[1] == 2 && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->ChangeBreatheEffect(false);
						}
						else if (MobDone[2] == 2 && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->ChangeBreatheEffect(false);
						}
						else if (MobDone[3] == 2 && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->ChangeBreatheEffect(false);
						}
					}
				}
			}
		}
		if (Breathe[1] == true)
		{
			Attack[1] = false;
			Breathe[1] = false;
			ChangeAttackImage(1, -1, 0);
		}

		if (OnCombo_Extinction_CostDown1 && CardNumByAttackOrder[1] == 31 && !ComboOnByAttackOrder_Extinction[1])
		{ // 절멸 콤보on인 상태인데, 취소할 카드가 절멸 콤보를 on 하는 카드고, 취소할 턴 이전에 콤보가 off이면
			OnCombo_Extinction_CostDown1 = false; // 콤보off
		}
		else if (!OnCombo_Extinction_CostDown1 && (CardNumByAttackOrder[1] == 32 || CardNumByAttackOrder[1] == 132 || CardNumByAttackOrder[1] == 35 || CardNumByAttackOrder[1] == 135) && ComboOnByAttackOrder_Extinction[1])
		{ // 절멸 콤보off인 상태인데, 취소할 카드가 절멸 콤보를 사용하는 카드고, 취소할 턴 이전에 콤보가 on이면
			OnCombo_Extinction_CostDown1 = true; // 콤보on
		}
		ComboOnByAttackOrder_Extinction[1] = false;

		if (!OnCombo_Triangle_CostDown1 && (CardNumByAttackOrder[1] == 32 || CardNumByAttackOrder[1] == 132) && ComboOnByAttackOrder_Triangle[1])
		{ // 절멸 콤보off인 상태인데, 취소할 카드가 절멸 콤보를 사용하는 카드고, 취소할 턴 이전에 콤보가 on이면
			OnCombo_Triangle_CostDown1 = true; // 콤보on
		}
		ComboOnByAttackOrder_Triangle[1] = false;

		CardNumByAttackOrder[1] = 0;
		ChangeHandCost();
		UpdateCost();
		ChangeTurnText();
	}
	else
	{
		FString sStr = FString::Printf(TEXT("취소할 공격이 없습니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}
}

void UUI_PlayWindow::OnButtonSecondClicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Attack[3] == true)
	{
		FString sStr = FString::Printf(TEXT("공격을 취소할 수 없습니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
		return;
	}
	else if (Attack[2] == true && Attack[1] == true && Attack[3] == false)
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (MobDone[1] == 2 && Enemy->ActorHasTag("Enemy1"))
				{
					Enemy->ChangeWidgetImage(0, 0);
					Enemy->ChangeBreatheEffect(false);
					Attack[2] = false;
					MobDone[1] = 0;
					CurrentCost += MobTargetCost[1];
					ChangeAttackImage(2, -1, 0);

					if (MobTargetCard[1] == 11 || MobTargetCard[1] == 111) // 카드가 철괴면
					{
						MyPlayer->Ironmode = false;

						if (MobTargetCard[1] == 111)
						{
							IronHeal = false;
						}
					}

					if (CardWidget != nullptr)
					{
						UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
						UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
						if (CardBox->GetChildrenCount() < 5)
						{
							UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[1]); // MobTargetCard[1]
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}
						else
						{
							UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[1]); // MobTargetCard[1]
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}

						MobTargetCost[1] = 0;
						MobTargetCard[1] = 0;
					}
				}
				else if (MobDone[2] == 2 && Enemy->ActorHasTag("Enemy2"))
				{
					Enemy->ChangeWidgetImage(0, 0);
					Enemy->ChangeBreatheEffect(false);
					Attack[2] = false;
					MobDone[2] = 0;
					CurrentCost += MobTargetCost[2];
					ChangeAttackImage(2, -1, 0);

					if (MobTargetCard[2] == 11 || MobTargetCard[2] == 111) // 카드가 철괴면
					{
						MyPlayer->Ironmode = false;

						if (MobTargetCard[2] == 111)
						{
							IronHeal = false;
						}
					}

					if (CardWidget != nullptr)
					{
						UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
						UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
						if (CardBox->GetChildrenCount() < 5)
						{
							UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[2]);
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}
						else
						{
							UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[2]);
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}

						MobTargetCost[2] = 0;
						MobTargetCard[2] = 0;
					}
				}
				else if (MobDone[3] == 2 && Enemy->ActorHasTag("Enemy3"))
				{
					Enemy->ChangeWidgetImage(0, 0);
					Enemy->ChangeBreatheEffect(false);
					Attack[2] = false;
					MobDone[3] = 0;
					CurrentCost += MobTargetCost[3];
					ChangeAttackImage(2, -1, 0);

					if (MobTargetCard[3] == 11 || MobTargetCard[3] == 111) // 카드가 철괴면
					{
						MyPlayer->Ironmode = false;

						if (MobTargetCard[3] == 111)
						{
							IronHeal = false;
						}
					}

					if (CardWidget != nullptr)
					{
						UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
						UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
						if (CardBox->GetChildrenCount() < 5)
						{
							UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[3]);
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}
						else
						{
							UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[3]);
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}

						MobTargetCost[3] = 0;
						MobTargetCard[3] = 0;
					}
				}
				else if (Breathe[2] == true)
				{
					if (Attack[3] == true)
					{

						if (MobDone[1] == 3 && Enemy->ActorHasTag("Enemy1"))
						{
							Enemy->ChangeBreatheEffect(false);
						}
						else if (MobDone[2] == 3 && Enemy->ActorHasTag("Enemy2"))
						{
							Enemy->ChangeBreatheEffect(false);
						}
						else if (MobDone[3] == 3 && Enemy->ActorHasTag("Enemy3"))
						{
							Enemy->ChangeBreatheEffect(false);
						}

					}
				}
			}
		}
		if (Breathe[2] == true)
		{
			Attack[2] = false;
			Breathe[2] = false;
			ChangeAttackImage(2, -1, 0);
		}

		if (OnCombo_Extinction_CostDown1 && CardNumByAttackOrder[2] == 31 && !ComboOnByAttackOrder_Extinction[2])
		{ // 절멸 콤보on인 상태인데, 취소할 카드가 절멸 콤보를 on 하는 카드고, 취소할 턴 이전에 콤보가 off이면
			OnCombo_Extinction_CostDown1 = false; // 콤보off
		}
		else if (!OnCombo_Extinction_CostDown1 && (CardNumByAttackOrder[2] == 32 || CardNumByAttackOrder[2] == 132 || CardNumByAttackOrder[2] == 35 || CardNumByAttackOrder[2] == 135) && ComboOnByAttackOrder_Extinction[2])
		{ // 절멸 콤보off인 상태인데, 취소할 카드가 절멸 콤보를 사용하는 카드고, 취소할 턴 이전에 콤보가 on이면
			OnCombo_Extinction_CostDown1 = true; // 콤보on
		}
		ComboOnByAttackOrder_Extinction[2] = false;

		if (!OnCombo_Triangle_CostDown1 && (CardNumByAttackOrder[2] == 32 || CardNumByAttackOrder[2] == 132) && ComboOnByAttackOrder_Triangle[2])
		{ // 절멸 콤보off인 상태인데, 취소할 카드가 절멸 콤보를 사용하는 카드고, 취소할 턴 이전에 콤보가 on이면
			OnCombo_Triangle_CostDown1 = true; // 콤보on
		}
		ComboOnByAttackOrder_Triangle[2] = false;

		CardNumByAttackOrder[2] = 0;
		ChangeHandCost();
		UpdateCost();
		ChangeTurnText();
	}
	else
	{
		FString sStr = FString::Printf(TEXT("취소할 공격이 없습니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}
}

void UUI_PlayWindow::OnButtonThirdClicked()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Attack[3] == true && Attack[2] == true && Attack[1] == true)
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (MobDone[1] == 3 && Enemy->ActorHasTag("Enemy1"))
				{
					Enemy->ChangeWidgetImage(0, 0);
					Enemy->ChangeBreatheEffect(false);
					Attack[3] = false;
					MobDone[1] = 0;
					CurrentCost += MobTargetCost[1];
					ChangeAttackImage(3, -1, 0);

					if (MobTargetCard[1] == 11 || MobTargetCard[1] == 111) // 카드가 철괴면
					{
						MyPlayer->Ironmode = false;

						if (MobTargetCard[1] == 111)
						{
							IronHeal = false;
						}
					}

					if (CardWidget != nullptr)
					{
						UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
						UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
						if (CardBox->GetChildrenCount() < 5)
						{
							UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[1]); // MobTargetCard[1]
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}
						else
						{
							UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[1]); // MobTargetCard[1]
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}

						MobTargetCost[1] = 0;
						MobTargetCard[1] = 0;
					}
				}
				else if (MobDone[2] == 3 && Enemy->ActorHasTag("Enemy2"))
				{
					Enemy->ChangeWidgetImage(0, 0);
					Enemy->ChangeBreatheEffect(false);
					Attack[3] = false;
					MobDone[2] = 0;
					CurrentCost += MobTargetCost[2];
					ChangeAttackImage(3, -1, 0);

					if (MobTargetCard[2] == 11 || MobTargetCard[2] == 111) // 카드가 철괴면
					{
						MyPlayer->Ironmode = false;

						if (MobTargetCard[2] == 111)
						{
							IronHeal = false;
						}
					}

					if (CardWidget != nullptr)
					{
						UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
						UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
						if (CardBox->GetChildrenCount() < 5)
						{
							UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[2]);
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}
						else
						{
							UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[2]);
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}

						MobTargetCost[2] = 0;
						MobTargetCard[2] = 0;
					}
				}
				else if (MobDone[3] == 3 && Enemy->ActorHasTag("Enemy3"))
				{
					Enemy->ChangeWidgetImage(0, 0);
					Enemy->ChangeBreatheEffect(false);
					Attack[3] = false;
					MobDone[3] = 0;
					CurrentCost += MobTargetCost[3];
					ChangeAttackImage(3, -1, 0);

					if (MobTargetCard[3] == 11 || MobTargetCard[3] == 111) // 카드가 철괴면
					{
						MyPlayer->Ironmode = false;

						if (MobTargetCard[3] == 111)
						{
							IronHeal = false;
						}
					}

					if (CardWidget != nullptr)
					{
						UUserWidget* NewWidget = CreateWidget(GetWorld(), CardWidget);
						UUI_PlayCard* NewCard = Cast<UUI_PlayCard>(NewWidget);
						if (CardBox->GetChildrenCount() < 5)
						{
							UPanelSlot* CardSlot = CardBox->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[3]);
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}
						else
						{
							UPanelSlot* CardSlot = CardBox2->AddChild(NewCard);
							NewCard->InsertImage(MobTargetCard[3]);
							UHorizontalBoxSlot* BoxSlot = Cast< UHorizontalBoxSlot>(CardSlot);
							BoxSlot->SetPadding(10);
							FSlateChildSize Size(ESlateSizeRule::Type::Fill);
							BoxSlot->SetSize(Size);
							BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
							BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
						}

						MobTargetCost[3] = 0;
						MobTargetCard[3] = 0;
					}
				}
				else if (Breathe[3] == true)
				{
					Attack[3] = false;
					Breathe[3] = false;
					ChangeAttackImage(3, -1, 0);
				}
			}
		}

		if (OnCombo_Extinction_CostDown1 && CardNumByAttackOrder[3] == 31 && !ComboOnByAttackOrder_Extinction[3])
		{ // 절멸 콤보on인 상태인데, 취소할 카드가 절멸 콤보를 on 하는 카드고, 취소할 턴 이전에 콤보가 off이면
			OnCombo_Extinction_CostDown1 = false; // 콤보off
		}
		else if (!OnCombo_Extinction_CostDown1 && (CardNumByAttackOrder[3] == 32 || CardNumByAttackOrder[3] == 132 || CardNumByAttackOrder[3] == 35 || CardNumByAttackOrder[3] == 135) && ComboOnByAttackOrder_Extinction[3])
		{ // 절멸 콤보off인 상태인데, 취소할 카드가 절멸 콤보를 사용하는 카드고, 취소할 턴 이전에 콤보가 on이면
			OnCombo_Extinction_CostDown1 = true; // 콤보on
		}
		ComboOnByAttackOrder_Extinction[3] = false;

		if (!OnCombo_Triangle_CostDown1 && (CardNumByAttackOrder[3] == 32 || CardNumByAttackOrder[3] == 132) && ComboOnByAttackOrder_Triangle[3])
		{ // 절멸 콤보off인 상태인데, 취소할 카드가 절멸 콤보를 사용하는 카드고, 취소할 턴 이전에 콤보가 on이면
			OnCombo_Triangle_CostDown1 = true; // 콤보on
		}
		ComboOnByAttackOrder_Triangle[3] = false;

		CardNumByAttackOrder[3] = 0;
		ChangeHandCost();
		UpdateCost();
		ChangeTurnText();
	}
	else
	{
		FString sStr = FString::Printf(TEXT("취소할 공격이 없습니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}
}

void UUI_PlayWindow::OnButtonBreathClicked()
{
	if (Attack[1] == false)
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;
			if (Enemy != nullptr)
			{
				if (Enemy->IamScent)
				{
					FString sStr = FString::Printf(TEXT("향기 상태인 적이 존재합니다"));
					Text_Notify->SetText(FText::FromString(sStr));
					PlayAnimation(Anim_Message);
					return;
				}
			}
		}

		AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (MyPlayer != nullptr)
		{
			Breathe[1] = true;
			Attack[1] = true;
			ChangeAttackImage(1, 0, 0);
		}
	}
	else if (Attack[2] == false)
	{
		AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (MyPlayer != nullptr)
		{
			Breathe[2] = true;
			Attack[2] = true;
			ChangeAttackImage(2, 0, 0);
		}
	}
	else if (Attack[3] == false)
	{
		AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (MyPlayer != nullptr)
		{
			Breathe[3] = true;
			Attack[3] = true;
			ChangeAttackImage(3, 0, 0);
		}
	}
	else
	{
		FString sStr = FString::Printf(TEXT("모든 공격이 완료되어있습니다"));
		Text_Notify->SetText(FText::FromString(sStr));
		PlayAnimation(Anim_Message);
	}

	ChangeTurnText();
}

void UUI_PlayWindow::OnButtonDeckClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowInventoryCardsWidget();
	}
}

void UUI_PlayWindow::OnButtonDrawDeckClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowDrawDeckWidget(DrawDeck);
	}
}

void UUI_PlayWindow::OnButtonDumpDeckClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowDrawDeckWidget(DumpDeck);
	}
}

void UUI_PlayWindow::TurnStartEnded() // 턴 시작
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
	MyController->FreezeEnd();
}
// 선제공격 턴
void UUI_PlayWindow::TurnEndEnded() // 턴 끝나고, 플레이어 -> 적 공격
{
	UE_LOG(LogTemp, Log, TEXT("Turn1End"));
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (MyPlayer->Ironmode) // 철괴 모드이면
	{
		MyPlayer->ParryingEffect(3);

		if (IronHeal)
		{
			AForgottenKnightCharacter::MyCurrentHealth += 10;
			MyPlayer->PlayHealEffect(10);
			MyPlayer->UpdateHealth();
			IronHeal = false;
		}

		return;
	}

	bool enemyhit = false; // 적을 가격했는지 여부
	bool PerfectParrying = false; // 완전 패링 여부
	bool MyAttack = true; // 이번 턴에 내가 공격했는지 여부
	bool WhoAttackMe = false; // 이번 턴에 누가 나를 공격한 지 여부
	TArray<int32> myhit = { false, false, false, false }; // 내가 특정 몹에게 가격당한 여부
	int ThisTurn = 1; // 지금 턴
	realthisturn = ThisTurn;

	int AttackedMob = 0; // 이번 공격에서 공격받을 적 식별
	for (int i = 1; i < 4; i++)
	{
		if (MobDone[i] == ThisTurn) // 내가 i몹에게 이번 턴 공격을 하면
		{
			AttackedMob = i;
		}
	}

	bool ParryingOn = false;

	// 내가 공격할 대상이 이번 턴에 공격하는지 여부 확인
	if (EnemyAttackOrder[AttackedMob] == ThisTurn && EnemyLife[AttackedMob])
	{
		ParryingOn = true; // 이번 턴 내 공격은 패링이 발생
	}

	if (!ParryingOn && AttackedMob != 0) // 내 공격이 패링되지 않으면
	{
		enemyhit = Attacking(AttackedMob, ThisTurn); // 내가 적 공격
		MyAttack = true;
	}

	if (ParryingOn) // 내 공격이 패링되면
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if ((AttackedMob == 1 && Enemy->ActorHasTag("Enemy1")) || (AttackedMob == 2 && Enemy->ActorHasTag("Enemy2")) || (AttackedMob == 3 && Enemy->ActorHasTag("Enemy3")))
				{
					Enemy->LastTurnParrying = true;
				}
			}
		}

		int k = ParryingWho(AttackedMob, ThisTurn); // 패링
		if (k == 0) // 내가 공격받음
		{
			myhit[AttackedMob] = Attacked(AttackedMob, ThisTurn);
			MyAttack = false; // 이번 턴에 난 공격 못함. 따라서 다음 턴 넘어가는 로직은 맞선 몹의 애니메이션에서 실행
			WhoAttackMe = true; // 내가 이번 턴에 공격받음
		}
		else if (k == 1) // 완전히 패링 되면
		{
			PerfectParrying = true;
			MyAttack = false; // 이번 턴에 난 공격 못함. 따라서 다음 턴 넘어가는 로직은 패링 애니메이션에서 실행
		}
		else if (k == 2) // 적이 공격받음
		{
			enemyhit = Attacking(AttackedMob, ThisTurn);
			MyAttack = true; // 이번 턴에 난 공격함. 따라서 다음 턴 넘어가는 로직은 내 공격 애니메이션에서 실행
		}
	}


	for (int i = 1; i < 4; i++)
	{ // 해당 턴에 이 몹이 공격하고, 생존해있고, 맞공격이 아니면
		if (EnemyAttackOrder[i] == ThisTurn && EnemyLife[i] && AttackedMob != i)
		{
			myhit[i] = Attacked(i, ThisTurn); // 적이 내게 공격
			WhoAttackMe = true; // 내가 이번 턴에 공격받음
		}
	}

	// 이번 턴에 공격한 적 내부 체력 변화 패턴 반영 && 향기 상태 업데이트
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			for (int i = 1; i < 4; i++)
			{
				if (EnemyAttackOrder[i] == ThisTurn && EnemyLife[i] && Enemy->ActorHasTag(FName(*("Enemy" + FString::FromInt(i)))))
				{
					Enemy->UpdateHealth(Enemy->TurnEndHeal);
					if (Enemy->TurnEndHeal > 0)
					{
						Enemy->PlayHealEffect(Enemy->TurnEndHeal);
					}
					Enemy->IamScent = Enemy->ConfirmedScent;
				}
			}
		}
	}

	// 히트 계산(적중 시 디버프 계산)
	if (enemyhit) // 내 공격이 적에 적중 시
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && MyPlayer != nullptr)
			{
				if ((AttackedMob == 1 && Enemy->ActorHasTag("Enemy1")) || (AttackedMob == 2 && Enemy->ActorHasTag("Enemy2")) || (AttackedMob == 3 && Enemy->ActorHasTag("Enemy3")))
				{
					Enemy->GetStackCorrosion += MyPlayer->AttackStackCorrosion;
					Enemy->GetStackWeakening += MyPlayer->AttackStackWeakening;
					Enemy->GetStackBlooding += MyPlayer->AttackStackBlooding;
					MyPlayer->AttackStackCorrosion = 0;
					MyPlayer->AttackStackWeakening = 0;
					MyPlayer->AttackStackBlooding = 0;
				}
			}
		}
	}
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && MyPlayer != nullptr)
		{ // 적 공격이 내게 적중시
			if ((Enemy->ActorHasTag("Enemy1") && myhit[1]) || (Enemy->ActorHasTag("Enemy2") && myhit[2]) || (Enemy->ActorHasTag("Enemy3") && myhit[3]))
			{
				MyPlayer->GetStackCorrosion += Enemy->AttackStackCorrosion;
				MyPlayer->GetStackWeakening += Enemy->AttackStackWeakening;
				MyPlayer->GetStackBlooding += Enemy->AttackStackBlooding;

				if (Enemy->DurahanTryUp)
				{
					Enemy->DurahanTryUp = false;
					Enemy->DurahanBuffUp = true;
				}
			}
		}
	}

	// 콤보 계산
	if (MyPlayer->ComboUsing_Triple) // 이 연쇄 카드 사용했으면
	{
		MyPlayer->Combo_Triple = MyPlayer->ComboConfirm_Triple;
	}
	if (MyPlayer->ComboUsing_JapHook)
	{
		MyPlayer->Combo_JapHook = MyPlayer->ComboConfirm_JapHook;
	}
	if (MyPlayer->ComboUsing_Extinction)
	{
		MyPlayer->Combo_Extinction = MyPlayer->ComboConfirm_Extinction;
	}
	MyPlayer->ComboUsing_Triple = 0;
	MyPlayer->ComboUsing_JapHook = 0;
	MyPlayer->ComboUsing_Extinction = 0;
	MyPlayer->ComboConfirm_Triple = 0;
	MyPlayer->ComboConfirm_JapHook = 0;
	MyPlayer->ComboConfirm_Extinction = 0;

	// 확정 디버프 계산
	MyPlayer->GetStackCorrosion += MyPlayer->ConfirmedStackCorrosion;
	MyPlayer->GetStackWeakening += MyPlayer->ConfirmedStackWeakening;
	MyPlayer->GetStackBlooding += MyPlayer->ConfirmedStackBlooding;
	MyPlayer->ConfirmedStackCorrosion = 0;
	MyPlayer->ConfirmedStackWeakening = 0;
	MyPlayer->ConfirmedStackBlooding = 0;
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			Enemy->GetStackCorrosion += Enemy->ConfirmedStackCorrosion;
			Enemy->GetStackWeakening += Enemy->ConfirmedStackWeakening;
			Enemy->GetStackBlooding += Enemy->ConfirmedStackBlooding;
			Enemy->ConfirmedStackCorrosion = 0;
			Enemy->ConfirmedStackWeakening = 0;
			Enemy->ConfirmedStackBlooding = 0;
		}
	}
	// 적 숨고르기 이펙트
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			for (int i = 1; i < 4; i++)
			{
				if (EnemyAttackOrder[i] == ThisTurn && EnemyLife[i] && Enemy->ActorHasTag(FName(*("Enemy" + FString::FromInt(i)))))
				{
					if (Enemy->ConfirmedBreathe)
					{
						Enemy->InBreathe = true;
						Enemy->ConfirmedBreathe = false;
						Enemy->OnBreatheEffect(0);
					}
				}
			}
		}
	}

	// 내 공격 애니메이션 실행
	if (AttackedMob != 0 && MyAttack) // 적을 지정해 공격하려 했고, 그 공격이 패링되지 않았으면
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && MyPlayer != nullptr)
			{
				if (AttackedMob == 1 && Enemy->ActorHasTag("Enemy1"))
				{
					Enemy->DamageEffect(ThisTurn, MobTargetCard[1], true);
				}
				else if (AttackedMob == 2 && Enemy->ActorHasTag("Enemy2"))
				{
					Enemy->DamageEffect(ThisTurn, MobTargetCard[2], true);
				}
				else if (AttackedMob == 3 && Enemy->ActorHasTag("Enemy3"))
				{
					Enemy->DamageEffect(ThisTurn, MobTargetCard[3], true);
				}
			}
		}
	}
	else if (AttackedMob != 0 && !MyAttack) // 적을 지정 공격했지만, 패링되었거나 역으로 당했으면
	{
		if (PerfectParrying) // 완전 패링이 이뤄졌으면
		{
			MyPlayer->ParryingEffect(ThisTurn);
		}
		else // 역으로 당했으면
		{
			MyPlayer->DamageEffect(ThisTurn, 0, true);
			return;
		}
	}

	if (AttackedMob == 0) // 적을 지정하지 않고, 숨고르기 시
	{
		MyHealthbar0->OnBreatheEffect(ThisTurn);
	}
	else // 숨고르기 안하고 공격했을 시
	{
		MyPlayer->AttackUp = 0;
	}

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			Enemy->UpdateNagativeEffect(); // 확정 디버프 갱신
		}
	}

	// 적 공격 대미지 이펙트 애니메이션 실행
	bool finalhit = false;
	if (myhit[1] || myhit[2] || myhit[3])
	{
		finalhit = true;
	}
	if (WhoAttackMe)
	{
		MyPlayer->DamageEffect(-2, 0, finalhit);
	}

	// 생사 여부 계산
	CheckLife();
}
// 패링 턴
void UUI_PlayWindow::Attack1Ended() // 플레이어 -> 적 공격이 끝나고, 적 -> 플레이어 공격
{
	UE_LOG(LogTemp, Log, TEXT("Turn2End"));
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	bool enemyhit = false; // 적을 가격했는지 여부
	bool PerfectParrying = false; // 완전 패링 여부
	bool MyAttack = true; // 이번 턴에 내가 공격했는지 여부
	bool WhoAttackMe = false; // 이번 턴에 누가 나를 공격한 지 여부
	TArray<int32> myhit = { false, false, false, false }; // 내가 특정 몹에게 가격당한 여부
	int ThisTurn = 2; // 지금 턴
	realthisturn = ThisTurn;

	int AttackedMob = 0; // 이번 공격에서 공격받을 적 식별
	for (int i = 1; i < 4; i++)
	{
		if (MobDone[i] == ThisTurn) // 내가 i몹에게 이번 턴 공격을 하면
		{
			AttackedMob = i;
		}
	}

	bool ParryingOn = false;

	// 내가 공격할 대상이 이번 턴에 공격하는지 여부 확인
	if (EnemyAttackOrder[AttackedMob] == ThisTurn && EnemyLife[AttackedMob])
	{
		ParryingOn = true; // 이번 턴 내 공격은 패링이 발생
	}

	if (!ParryingOn && AttackedMob != 0) // 내 공격이 패링되지 않으면
	{
		enemyhit = Attacking(AttackedMob, ThisTurn); // 내가 적 공격
		MyAttack = true;
	}
	else if (ParryingOn) // 내 공격이 패링되면
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if ((AttackedMob == 1 && Enemy->ActorHasTag("Enemy1")) || (AttackedMob == 2 && Enemy->ActorHasTag("Enemy2")) || (AttackedMob == 3 && Enemy->ActorHasTag("Enemy3")))
				{
					Enemy->LastTurnParrying = true;
				}
			}
		}

		int k = ParryingWho(AttackedMob, ThisTurn); // 패링
		if (k == 0) // 내가 공격받음
		{
			myhit[AttackedMob] = Attacked(AttackedMob, ThisTurn);
			MyAttack = false; // 이번 턴에 난 공격 못함. 따라서 다음 턴 넘어가는 로직은 맞선 몹의 애니메이션에서 실행
			WhoAttackMe = true; // 내가 이번 턴에 공격받음
		}
		else if (k == 1) // 완전히 패링 되면
		{
			PerfectParrying = true;
			MyAttack = false; // 이번 턴에 난 공격 못함. 따라서 다음 턴 넘어가는 로직은 패링 애니메이션에서 실행
		}
		else if (k == 2) // 적이 공격받음
		{
			enemyhit = Attacking(AttackedMob, ThisTurn);
			MyAttack = true; // 이번 턴에 난 공격함. 따라서 다음 턴 넘어가는 로직은 내 공격 애니메이션에서 실행
		}
	}

	for (int i = 1; i < 4; i++)
	{ // 해당 턴에 이 몹이 공격하고, 생존해있고, 맞공격이 아니면
		if (EnemyAttackOrder[i] == ThisTurn && EnemyLife[i] && AttackedMob != i)
		{
			myhit[i] = Attacked(i, ThisTurn); // 적이 내게 공격
			WhoAttackMe = true; // 내가 이번 턴에 공격받음
		}
	}

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{ // 연속 공격 매커니즘(패링 함수에만 있음)
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			for (int i = 1; i < 4; i++)
			{
				if (EnemyLife[i] && Enemy->ActorHasTag(FName(*("Enemy" + FString::FromInt(i)))))
				{
					if (Enemy->AttackTryNum > 1) // 연속 공격이면
					{
						for (int xx = 1; xx < Enemy->AttackTryNum; xx++)
						{
							myhit[i] = BonusAttacked(i, ThisTurn); // 적이 내게 공격
							WhoAttackMe = true; // 내가 이번 턴에 공격받음
						}
					}
				}
			}
		}
	}

	// 이번 턴에 공격한 적 내부 체력 변화 패턴 반영 && 향기 상태 업데이트
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			for (int i = 1; i < 4; i++)
			{
				if (EnemyAttackOrder[i] == ThisTurn && EnemyLife[i] && Enemy->ActorHasTag(FName(*("Enemy" + FString::FromInt(i)))))
				{
					Enemy->UpdateHealth(Enemy->TurnEndHeal);
					if (Enemy->TurnEndHeal > 0)
					{
						Enemy->PlayHealEffect(Enemy->TurnEndHeal);
					}
					Enemy->IamScent = Enemy->ConfirmedScent;
				}
			}
		}
	}

	// 히트 계산(적중 시 디버프 계산)
	if (enemyhit) // 내 공격이 적에 적중 시
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && MyPlayer != nullptr)
			{
				if ((AttackedMob == 1 && Enemy->ActorHasTag("Enemy1")) || (AttackedMob == 2 && Enemy->ActorHasTag("Enemy2")) || (AttackedMob == 3 && Enemy->ActorHasTag("Enemy3")))
				{
					Enemy->GetStackCorrosion += MyPlayer->AttackStackCorrosion;
					Enemy->GetStackWeakening += MyPlayer->AttackStackWeakening;
					Enemy->GetStackBlooding += MyPlayer->AttackStackBlooding;
					MyPlayer->AttackStackCorrosion = 0;
					MyPlayer->AttackStackWeakening = 0;
					MyPlayer->AttackStackBlooding = 0;
				}
			}
		}
	}
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && MyPlayer != nullptr)
		{ // 적 공격이 내게 적중시
			if ((Enemy->ActorHasTag("Enemy1") && myhit[1]) || (Enemy->ActorHasTag("Enemy2") && myhit[2]) || (Enemy->ActorHasTag("Enemy3") && myhit[3]))
			{
				MyPlayer->GetStackCorrosion += Enemy->AttackStackCorrosion;
				MyPlayer->GetStackWeakening += Enemy->AttackStackWeakening;
				MyPlayer->GetStackBlooding += Enemy->AttackStackBlooding;

				if (Enemy->DurahanTryUp)
				{
					Enemy->DurahanTryUp = false;
					Enemy->DurahanBuffUp = true;
				}
			}
		}
	}

	// 콤보 계산
	if (MyPlayer->ComboUsing_Triple) // 이 연쇄 카드 사용했으면
	{
		MyPlayer->Combo_Triple = MyPlayer->ComboConfirm_Triple;
	}
	if (MyPlayer->ComboUsing_JapHook)
	{
		MyPlayer->Combo_JapHook = MyPlayer->ComboConfirm_JapHook;
	}
	if (MyPlayer->ComboUsing_Extinction)
	{
		MyPlayer->Combo_Extinction = MyPlayer->ComboConfirm_Extinction;
	}
	MyPlayer->ComboUsing_Triple = 0;
	MyPlayer->ComboUsing_JapHook = 0;
	MyPlayer->ComboUsing_Extinction = 0;
	MyPlayer->ComboConfirm_Triple = 0;
	MyPlayer->ComboConfirm_JapHook = 0;
	MyPlayer->ComboConfirm_Extinction = 0;

	// 확정 디버프 계산
	MyPlayer->GetStackCorrosion += MyPlayer->ConfirmedStackCorrosion;
	MyPlayer->GetStackWeakening += MyPlayer->ConfirmedStackWeakening;
	MyPlayer->GetStackBlooding += MyPlayer->ConfirmedStackBlooding;
	MyPlayer->ConfirmedStackCorrosion = 0;
	MyPlayer->ConfirmedStackWeakening = 0;
	MyPlayer->ConfirmedStackBlooding = 0;
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			Enemy->GetStackCorrosion += Enemy->ConfirmedStackCorrosion;
			Enemy->GetStackWeakening += Enemy->ConfirmedStackWeakening;
			Enemy->GetStackBlooding += Enemy->ConfirmedStackBlooding;
			Enemy->ConfirmedStackCorrosion = 0;
			Enemy->ConfirmedStackWeakening = 0;
			Enemy->ConfirmedStackBlooding = 0;
		}
	}
	// 적 숨고르기 이펙트
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			for (int i = 1; i < 4; i++)
			{
				if (EnemyAttackOrder[i] == ThisTurn && EnemyLife[i] && Enemy->ActorHasTag(FName(*("Enemy" + FString::FromInt(i)))))
				{
					if (Enemy->ConfirmedBreathe)
					{
						Enemy->InBreathe = true;
						Enemy->ConfirmedBreathe = false;
						Enemy->OnBreatheEffect(0);
					}
				}
			}
		}
	}

	//페어리 체력 깎이는 매커니즘
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (Enemy->ActorHasTag("Enemy1") && Enemy->FairyTurn == 3 && Enemy->ActorHasTag("FallenFairy") && EnemyLife[1])
			{
				Enemy->LoseHP(Enemy->FairyLoseHPValue);
				if (Enemy->ShowCurrentHP() == 0)
				{
					EnemyHealthbar1->Playdieanim();
				}
			}
			else if (Enemy->ActorHasTag("Enemy2") && Enemy->FairyTurn == 3 && Enemy->ActorHasTag("FallenFairy") && EnemyLife[2])
			{
				Enemy->LoseHP(Enemy->FairyLoseHPValue);
				if (Enemy->ShowCurrentHP() == 0)
				{
					EnemyHealthbar2->Playdieanim();
				}
			}
			else if (Enemy->ActorHasTag("Enemy3") && Enemy->FairyTurn == 3 && Enemy->ActorHasTag("FallenFairy") && EnemyLife[3])
			{
				Enemy->LoseHP(Enemy->FairyLoseHPValue);
				if (Enemy->ShowCurrentHP() == 0)
				{
					EnemyHealthbar3->Playdieanim();
				}
			}
			else if (Enemy->ActorHasTag("Enemy1") && Enemy->FairyTurn == 4 && Enemy->ActorHasTag("FallenFairy") && EnemyLife[1])
			{
				Enemy->LoseHP(Enemy->ShowCurrentHP());
				EnemyHealthbar1->Playdieanim();
			}
			else if (Enemy->ActorHasTag("Enemy2") && Enemy->FairyTurn == 4 && Enemy->ActorHasTag("FallenFairy") && EnemyLife[2])
			{
				Enemy->LoseHP(Enemy->ShowCurrentHP());
				EnemyHealthbar2->Playdieanim();
			}
			else if (Enemy->ActorHasTag("Enemy3") && Enemy->FairyTurn == 4 && Enemy->ActorHasTag("FallenFairy") && EnemyLife[3])
			{
				Enemy->LoseHP(Enemy->ShowCurrentHP());
				EnemyHealthbar3->Playdieanim();
			}
		}
	}

	// 마왕 버프 관련
	bool mawangon = false;
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (Enemy->ActorHasTag("Enemy1") && Enemy->AllStrongBuff && EnemyLife[1])
			{
				Enemy->AllStrongBuff = false;
				mawangon = true;
			}

			if (Enemy->SoonPowerHit)
			{
				Enemy->SoonPowerHit = false;
				Enemy->GetPowerHit = true;
			}

			if (Enemy->SoonLosePower)
			{
				Enemy->SoonLosePower = false;
				Enemy->GetPowerHit = false;
			}
		}
	}
	if (mawangon)
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				Enemy->BonusDamageValue += 1;
			}
		}
	}

	// 내 공격 애니메이션 실행
	if (AttackedMob != 0 && MyAttack) // 적을 지정해 공격하려 했고, 그 공격이 패링되지 않았으면
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && MyPlayer != nullptr)
			{
				if (AttackedMob == 1 && Enemy->ActorHasTag("Enemy1"))
				{
					Enemy->DamageEffect(ThisTurn, MobTargetCard[1], true);
				}
				else if (AttackedMob == 2 && Enemy->ActorHasTag("Enemy2"))
				{
					Enemy->DamageEffect(ThisTurn, MobTargetCard[2], true);
				}
				else if (AttackedMob == 3 && Enemy->ActorHasTag("Enemy3"))
				{
					Enemy->DamageEffect(ThisTurn, MobTargetCard[3], true);
				}
			}
		}
	}
	else if (AttackedMob != 0 && !MyAttack) // 적을 지정 공격했지만, 패링되었거나 역으로 당했으면
	{
		if (PerfectParrying) // 완전 패링이 이뤄졌으면
		{
			MyPlayer->ParryingEffect(ThisTurn);
		}
		else // 역으로 당했으면
		{
			MyPlayer->DamageEffect(ThisTurn, 0, true);
			return;
		}
	}

	if (AttackedMob == 0) // 적을 지정하지 않고, 숨고르기 시
	{
		MyHealthbar0->OnBreatheEffect(ThisTurn);
	}
	else
	{
		MyPlayer->AttackUp = 0;
	}

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			Enemy->UpdateNagativeEffect(); // 확정 디버프 갱신
		}
	}

	// 적 공격 대미지 이펙트 애니메이션 실행
	bool finalhit = false;
	if (myhit[1] || myhit[2] || myhit[3])
	{
		finalhit = true;
	}
	if (WhoAttackMe)
	{
		MyPlayer->DamageEffect(-2, 0, finalhit);
	}

	// 볼 발사 후 감추기
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (Enemy->ActorHasTag("MagicBall"))
			{
				if (Enemy->ActorHasTag("Enemy1") && EnemyLife[1])
				{
					EnemyHealthbar1->Playdieanim();
				}
				else if (Enemy->ActorHasTag("Enemy2") && EnemyLife[2])
				{
					EnemyHealthbar2->Playdieanim();
				}
				else if (Enemy->ActorHasTag("Enemy3") && EnemyLife[3])
				{
					EnemyHealthbar3->Playdieanim();
				}
			}
		}
	}

	// 생사 여부 계산
	CheckLife();
}
// 카운터 턴
void UUI_PlayWindow::Attack2Ended() // 적 -> 플레이어 끝나고 플레이어 -> 적 공격
{
	UE_LOG(LogTemp, Log, TEXT("Turn3End"));
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	bool enemyhit = false; // 적을 가격했는지 여부
	bool PerfectParrying = false; // 완전 패링 여부
	bool MyAttack = true; // 이번 턴에 내가 공격했는지 여부
	bool WhoAttackMe = false; // 이번 턴에 누가 나를 공격한 지 여부
	TArray<int32> myhit = { false, false, false, false }; // 내가 특정 몹에게 가격당한 여부
	int ThisTurn = 3; // 지금 턴
	realthisturn = ThisTurn;

	int AttackedMob = 0; // 이번 공격에서 공격받을 적 식별
	for (int i = 1; i < 4; i++)
	{
		if (MobDone[i] == ThisTurn) // 내가 i몹에게 이번 턴 공격을 하면
		{
			AttackedMob = i;
		}
	}

	bool ParryingOn = false;

	// 내가 공격할 대상이 이번 턴에 공격하는지 여부 확인
	if (EnemyAttackOrder[AttackedMob] == ThisTurn && EnemyLife[AttackedMob])
	{
		ParryingOn = true; // 이번 턴 내 공격은 패링이 발생
	}

	if (!ParryingOn && AttackedMob != 0) // 내 공격이 패링되지 않으면
	{
		enemyhit = Attacking(AttackedMob, ThisTurn); // 내가 적 공격
		MyAttack = true;
	}
	else if (ParryingOn) // 내 공격이 패링되면
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if ((AttackedMob == 1 && Enemy->ActorHasTag("Enemy1")) || (AttackedMob == 2 && Enemy->ActorHasTag("Enemy2")) || (AttackedMob == 3 && Enemy->ActorHasTag("Enemy3")))
				{
					Enemy->LastTurnParrying = true;
				}
			}
		}

		int k = ParryingWho(AttackedMob, ThisTurn); // 패링
		if (k == 0) // 내가 공격받음
		{
			myhit[AttackedMob] = Attacked(AttackedMob, ThisTurn);
			MyAttack = false; // 이번 턴에 난 공격 못함. 따라서 다음 턴 넘어가는 로직은 맞선 몹의 애니메이션에서 실행
			WhoAttackMe = true; // 내가 이번 턴에 공격받음
		}
		else if (k == 1) // 완전히 패링 되면
		{
			PerfectParrying = true;
			MyAttack = false; // 이번 턴에 난 공격 못함. 따라서 다음 턴 넘어가는 로직은 패링 애니메이션에서 실행
		}
		else if (k == 2) // 적이 공격받음
		{
			enemyhit = Attacking(AttackedMob, ThisTurn);
			MyAttack = true; // 이번 턴에 난 공격함. 따라서 다음 턴 넘어가는 로직은 내 공격 애니메이션에서 실행
		}
	}

	for (int i = 1; i < 4; i++)
	{ // 해당 턴에 이 몹이 공격하고, 생존해있고, 맞공격이 아니면
		if (EnemyAttackOrder[i] == ThisTurn && EnemyLife[i] && AttackedMob != i)
		{
			myhit[i] = Attacked(i, ThisTurn); // 적이 내게 공격
			WhoAttackMe = true; // 내가 이번 턴에 공격받음
		}
	}

	// 이번 턴에 공격한 적 내부 체력 변화 패턴 반영 && 향기 상태 업데이트
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			for (int i = 1; i < 4; i++)
			{
				if (EnemyAttackOrder[i] == ThisTurn && EnemyLife[i] && Enemy->ActorHasTag(FName(*("Enemy" + FString::FromInt(i)))))
				{
					Enemy->UpdateHealth(Enemy->TurnEndHeal);
					if (Enemy->TurnEndHeal > 0)
					{
						Enemy->PlayHealEffect(Enemy->TurnEndHeal);
					}
					Enemy->IamScent = Enemy->ConfirmedScent;
				}
			}
		}
	}

	// 히트 계산(적중 시 디버프 계산)
	if (enemyhit) // 내 공격이 적에 적중 시
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && MyPlayer != nullptr)
			{
				if ((AttackedMob == 1 && Enemy->ActorHasTag("Enemy1")) || (AttackedMob == 2 && Enemy->ActorHasTag("Enemy2")) || (AttackedMob == 3 && Enemy->ActorHasTag("Enemy3")))
				{
					Enemy->GetStackCorrosion += MyPlayer->AttackStackCorrosion;
					Enemy->GetStackWeakening += MyPlayer->AttackStackWeakening;
					Enemy->GetStackBlooding += MyPlayer->AttackStackBlooding;
					MyPlayer->AttackStackCorrosion = 0;
					MyPlayer->AttackStackWeakening = 0;
					MyPlayer->AttackStackBlooding = 0;
				}
			}
		}
	}
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && MyPlayer != nullptr)
		{ // 적 공격이 내게 적중시
			if ((Enemy->ActorHasTag("Enemy1") && myhit[1]) || (Enemy->ActorHasTag("Enemy2") && myhit[2]) || (Enemy->ActorHasTag("Enemy3") && myhit[3]))
			{
				MyPlayer->GetStackCorrosion += Enemy->AttackStackCorrosion;
				MyPlayer->GetStackWeakening += Enemy->AttackStackWeakening;
				MyPlayer->GetStackBlooding += Enemy->AttackStackBlooding;

				if (Enemy->DurahanTryUp)
				{
					Enemy->DurahanTryUp = false;
					Enemy->DurahanBuffUp = true;
				}
			}
		}
	}

	// 콤보 계산
	if (MyPlayer->ComboUsing_Triple) // 이 연쇄 카드 사용했으면
	{
		MyPlayer->Combo_Triple = MyPlayer->ComboConfirm_Triple;
	}
	if (MyPlayer->ComboUsing_JapHook)
	{
		MyPlayer->Combo_JapHook = MyPlayer->ComboConfirm_JapHook;
	}
	if (MyPlayer->ComboUsing_Extinction)
	{
		MyPlayer->Combo_Extinction = MyPlayer->ComboConfirm_Extinction;
	}
	MyPlayer->ComboUsing_Triple = 0;
	MyPlayer->ComboUsing_JapHook = 0;
	MyPlayer->ComboUsing_Extinction = 0;
	MyPlayer->ComboConfirm_Triple = 0;
	MyPlayer->ComboConfirm_JapHook = 0;
	MyPlayer->ComboConfirm_Extinction = 0;

	// 확정 디버프 계산
	MyPlayer->GetStackCorrosion += MyPlayer->ConfirmedStackCorrosion;
	MyPlayer->GetStackWeakening += MyPlayer->ConfirmedStackWeakening;
	MyPlayer->GetStackBlooding += MyPlayer->ConfirmedStackBlooding;
	MyPlayer->ConfirmedStackCorrosion = 0;
	MyPlayer->ConfirmedStackWeakening = 0;
	MyPlayer->ConfirmedStackBlooding = 0;
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			Enemy->GetStackCorrosion += Enemy->ConfirmedStackCorrosion;
			Enemy->GetStackWeakening += Enemy->ConfirmedStackWeakening;
			Enemy->GetStackBlooding += Enemy->ConfirmedStackBlooding;
			Enemy->ConfirmedStackCorrosion = 0;
			Enemy->ConfirmedStackWeakening = 0;
			Enemy->ConfirmedStackBlooding = 0;
		}
	}
	// 적 숨고르기 이펙트
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			for (int i = 1; i < 4; i++)
			{
				if (EnemyAttackOrder[i] == ThisTurn && EnemyLife[i] && Enemy->ActorHasTag(FName(*("Enemy" + FString::FromInt(i)))))
				{
					if (Enemy->ConfirmedBreathe)
					{
						Enemy->InBreathe = true;
						Enemy->ConfirmedBreathe = false;
						Enemy->OnBreatheEffect(0);
					}
				}
			}
		}
	}

	// 내 공격 애니메이션 실행
	if (AttackedMob != 0 && MyAttack) // 적을 지정해 공격하려 했고, 그 공격이 패링되지 않았으면
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && MyPlayer != nullptr)
			{
				if (AttackedMob == 1 && Enemy->ActorHasTag("Enemy1"))
				{
					Enemy->DamageEffect(ThisTurn, MobTargetCard[1], true);
				}
				else if (AttackedMob == 2 && Enemy->ActorHasTag("Enemy2"))
				{
					Enemy->DamageEffect(ThisTurn, MobTargetCard[2], true);
				}
				else if (AttackedMob == 3 && Enemy->ActorHasTag("Enemy3"))
				{
					Enemy->DamageEffect(ThisTurn, MobTargetCard[3], true);
				}
			}
		}
	}
	else if (AttackedMob != 0 && !MyAttack) // 적을 지정 공격했지만, 패링되었거나 역으로 당했으면
	{
		if (PerfectParrying) // 완전 패링이 이뤄졌으면
		{
			MyPlayer->ParryingEffect(ThisTurn);
		}
		else // 역으로 당했으면
		{
			MyPlayer->DamageEffect(ThisTurn, 0, true);
			return;
		}
	}

	if (AttackedMob == 0) // 적을 지정하지 않고, 숨고르기 시
	{
		MyHealthbar0->OnBreatheEffect(ThisTurn);
	}
	else
	{
		MyPlayer->AttackUp = 0;
	}

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			Enemy->UpdateNagativeEffect(); // 확정 디버프 갱신
		}
	}

	// 적 공격 대미지 이펙트 애니메이션 실행
	bool finalhit = false;
	if (myhit[1] || myhit[2] || myhit[3])
	{
		finalhit = true;
	}
	if (WhoAttackMe)
	{
		MyPlayer->DamageEffect(-2, 0, finalhit);
	}

	// 생사 여부 계산
	CheckLife();
}
// 모든 공격 후(출혈 매커니즘 처리)
void UUI_PlayWindow::Attack3Ended() // 모든 공격 끝나고, 출혈 공격 계산
{
	realthisturn = -1;

	UE_LOG(LogTemp, Log, TEXT("BloodEnd"));
	// 출혈 대미지 발생
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	int k = 0; // 출혈 디버프를 받은 개체 수 확인
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 적 개체 확인
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (Enemy->LifeCheck() == true) // 생존한 적 확인
			{
				Enemy->BloodingDamage(); // 출혈 대미지 계산

				if (Enemy->ActorHasTag("Enemy1") && Enemy->GetStackBlooding > 0)
				{
					k += 1;
					Enemy->BloodEffect();
				}
				else if (Enemy->ActorHasTag("Enemy2") && Enemy->GetStackBlooding > 0)
				{
					k += 1;
					Enemy->BloodEffect();
				}
				else if (Enemy->ActorHasTag("Enemy3") && Enemy->GetStackBlooding > 0)
				{
					k += 1;
					Enemy->BloodEffect();
				}
			}
		}
	}

	if (MyPlayer != nullptr)
	{
		MyHealthbar0->GetBloodDamaged(MyPlayer->GetStackBlooding);
		if (MyPlayer->GetStackBlooding > 0 && !(MyPlayer->Ironmode))
		{
			k += 1;
			MyHealthbar0->BloodEffect();
		}
	}

	if (k == 0) // 출혈 관련이 없는 경우
	{
		TurnClear();
	}
	else
	{
		//MyPlayer->AllBloodEnd = k;
		//WaitBlooding = true;
		PlayAnimation(Anim_BloodSet);
	}

	CheckLife();
}
// 내가 적 공격
bool UUI_PlayWindow::Attacking(int MobNum, int turn)
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	bool enemyhit = false;

	FName EnemyTag = FName(*("Enemy" + FString::FromInt(MobNum)));

	int lol = 0;
	if (turn == 3)
	{
		if (MobNum == 1 && EnemyHealthbar1->IamBall)
		{
			lol = 1;
		}
		else if (MobNum == 2 && EnemyHealthbar2->IamBall)
		{
			lol = 2;
		}
		else if (MobNum == 3 && EnemyHealthbar3->IamBall)
		{
			lol = 3;
		}
	}

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && MyPlayer != nullptr)
		{
			if (lol > 0)
			{
				if (lol > 0 && Enemy->ActorHasTag(EnemyTag))
				{
					if ((Breathe[0] || Breathe[1] || Breathe[2]) && MobTargetCard[MobNum] != 0)
					{
						enemyhit = Enemy->CalculateTurnEnd(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], true, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackWeakening != 0, MyPlayer->GetStackWeakening != 0);
						if (MyHealthbar0)
						{
							MyHealthbar0->LoseBreathe();
						}
						Breathe[0] = false;
						Breathe[1] = false;
						Breathe[2] = false;
					}
					else
					{
						enemyhit = Enemy->CalculateTurnEnd(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], false, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackWeakening != 0, MyPlayer->GetStackWeakening != 0);
					}
				}
			}
			else if (EnemyLife[MobNum] && Enemy->ActorHasTag(EnemyTag))
			{ // turn을 나눈 이유 : 숨고르기 턴 계산 때문
				if (turn == 1) // 선제공격 턴이면
				{
					if (Breathe[0] && MobTargetCard[MobNum] != 0)
					{
						enemyhit = Enemy->CalculateTurnEnd(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], true, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackWeakening != 0, MyPlayer->GetStackWeakening != 0);
						if (MyHealthbar0)
						{
							MyHealthbar0->LoseBreathe();
						}
						Breathe[0] = false;
					}
					else
					{
						enemyhit = Enemy->CalculateTurnEnd(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], false, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackWeakening != 0, MyPlayer->GetStackWeakening != 0);
					}
				}
				else if (turn == 2) // 패링 턴이면
				{
					if ((Breathe[0] || Breathe[1]) && MobTargetCard[MobNum] != 0)
					{
						enemyhit = Enemy->CalculateTurnEnd(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], true, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackWeakening != 0, MyPlayer->GetStackWeakening != 0);
						if (MyHealthbar0)
						{
							MyHealthbar0->LoseBreathe();
						}
						Breathe[0] = false;
						Breathe[1] = false;
					}
					else
					{
						enemyhit = Enemy->CalculateTurnEnd(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], false, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackWeakening != 0, MyPlayer->GetStackWeakening != 0);
					}
				}
				else if (turn == 3) // 카운터 턴이면
				{
					if ((Breathe[0] || Breathe[1] || Breathe[2]) && MobTargetCard[MobNum] != 0)
					{
						enemyhit = Enemy->CalculateTurnEnd(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], true, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackWeakening != 0, MyPlayer->GetStackWeakening != 0);
						if (MyHealthbar0)
						{
							MyHealthbar0->LoseBreathe();
						}
						Breathe[0] = false;
						Breathe[1] = false;
						Breathe[2] = false;
					}
					else
					{
						enemyhit = Enemy->CalculateTurnEnd(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], false, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackWeakening != 0, MyPlayer->GetStackWeakening != 0);
					}
				}
			}
		}
	}
	return enemyhit;
}
// 적이 날 공격
bool UUI_PlayWindow::Attacked(int MobNum, int turn)
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	bool myhit = false;

	FName EnemyTag = FName(*("Enemy" + FString::FromInt(MobNum)));

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && MyPlayer != nullptr)
		{
			if (EnemyLife[MobNum] && Enemy->ActorHasTag(EnemyTag))
			{
				Enemy->GoAttack(); // 공격 애니메이션

				if (turn == 1) // 선제공격 턴이면
				{// 굳이 숨고르기 갈래 안나눠도 되지만, 일단 넣어놓음
					if (Breathe[0] && MobTargetCard[MobNum] != 0)
					{
						myhit = MyPlayer->CalculateTurnEnd(MobNum, Enemy->ConfirmedDebuffType, Enemy->ConfirmedDebuffValue, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], true, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
					else
					{
						myhit = MyPlayer->CalculateTurnEnd(MobNum, Enemy->ConfirmedDebuffType, Enemy->ConfirmedDebuffValue, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], false, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
				}
				else if (turn == 2) // 패링 턴이면
				{
					if ((Breathe[0] || Breathe[1]) && MobTargetCard[MobNum] != 0)
					{
						myhit = MyPlayer->CalculateTurnEnd(MobNum, Enemy->ConfirmedDebuffType, Enemy->ConfirmedDebuffValue, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], true, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
					else
					{
						myhit = MyPlayer->CalculateTurnEnd(MobNum, Enemy->ConfirmedDebuffType, Enemy->ConfirmedDebuffValue, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], false, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
				}
				else if (turn == 3) // 카운터 턴이면
				{
					if ((Breathe[0] || Breathe[1] || Breathe[2]) && MobTargetCard[MobNum] != 0)
					{
						myhit = MyPlayer->CalculateTurnEnd(MobNum, Enemy->ConfirmedDebuffType, Enemy->ConfirmedDebuffValue, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], true, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
					else
					{
						myhit = MyPlayer->CalculateTurnEnd(MobNum, Enemy->ConfirmedDebuffType, Enemy->ConfirmedDebuffValue, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], false, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
				}

			}
		}
	}

	return myhit;
}
// 패링 시 세 가지 경우 구별
int UUI_PlayWindow::ParryingWho(int MobNum, int turn)
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	int who = 0;

	FName EnemyTag = FName(*("Enemy" + FString::FromInt(MobNum)));

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && MyPlayer != nullptr)
		{
			if (EnemyLife[MobNum] && Enemy->ActorHasTag(EnemyTag))
			{
				if (turn == 1)
				{
					if ((Breathe[0]) && MobTargetCard[MobNum] != 0)
					{
						who = MyPlayer->ParryingWho(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], true, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
					else
					{
						who = MyPlayer->ParryingWho(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], false, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
				}
				else if (turn == 2)
				{
					if ((Breathe[0] || Breathe[1]) && MobTargetCard[MobNum] != 0)
					{
						who = MyPlayer->ParryingWho(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], true, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
					else
					{
						who = MyPlayer->ParryingWho(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], false, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
				}
				else if (turn == 3)
				{
					if ((Breathe[0] || Breathe[1] || Breathe[2]) && MobTargetCard[MobNum] != 0)
					{
						who = MyPlayer->ParryingWho(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], true, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
					else
					{
						who = MyPlayer->ParryingWho(MobNum, Enemy->DamageValue, MobTargetCard[MobNum], MobDone[MobNum], false, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
				}
			}
		}
	}
	return who;
}
// 추가공격으로 날 공격
bool UUI_PlayWindow::BonusAttacked(int MobNum, int turn)
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	bool myhit = false;

	FName EnemyTag = FName(*("Enemy" + FString::FromInt(MobNum)));

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && MyPlayer != nullptr)
		{
			if (EnemyLife[MobNum] && Enemy->ActorHasTag(EnemyTag))
			{
				if (turn == 1) // 선제공격 턴이면
				{// 굳이 숨고르기 갈래 안나눠도 되지만, 일단 넣어놓음
					if (Breathe[0] && MobTargetCard[MobNum] != 0)
					{
						myhit = MyPlayer->CalculateTurnEnd(MobNum, Enemy->ConfirmedDebuffType, Enemy->ConfirmedDebuffValue, Enemy->DamageValue, MobTargetCard[MobNum], 1, true, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
					else
					{
						myhit = MyPlayer->CalculateTurnEnd(MobNum, Enemy->ConfirmedDebuffType, Enemy->ConfirmedDebuffValue, Enemy->DamageValue, MobTargetCard[MobNum], 1, false, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
				}
				else if (turn == 2) // 패링 턴이면
				{
					if ((Breathe[0] || Breathe[1]) && MobTargetCard[MobNum] != 0)
					{
						myhit = MyPlayer->CalculateTurnEnd(MobNum, Enemy->ConfirmedDebuffType, Enemy->ConfirmedDebuffValue, Enemy->DamageValue, MobTargetCard[MobNum], 1, true, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
					else
					{
						myhit = MyPlayer->CalculateTurnEnd(MobNum, Enemy->ConfirmedDebuffType, Enemy->ConfirmedDebuffValue, Enemy->DamageValue, MobTargetCard[MobNum], 1, false, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
				}
				else if (turn == 3) // 카운터 턴이면
				{
					if ((Breathe[0] || Breathe[1] || Breathe[2]) && MobTargetCard[MobNum] != 0)
					{
						myhit = MyPlayer->CalculateTurnEnd(MobNum, Enemy->ConfirmedDebuffType, Enemy->ConfirmedDebuffValue, Enemy->DamageValue, MobTargetCard[MobNum], 1, true, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
					else
					{
						myhit = MyPlayer->CalculateTurnEnd(MobNum, Enemy->ConfirmedDebuffType, Enemy->ConfirmedDebuffValue, Enemy->DamageValue, MobTargetCard[MobNum], 1, false, MyPlayer->GetStackCorrosion != 0, Enemy->GetStackCorrosion != 0, MyPlayer->GetStackWeakening != 0, Enemy->GetStackWeakening != 0);
					}
				}

			}
		}
	}

	return myhit;
}

void UUI_PlayWindow::BloodSetEnded()
{
	TurnClear();
}
