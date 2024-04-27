// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CircularThrobber.h"
#include "Styling/SlateBrush.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "Animation/WidgetAnimation.h"
#include "EngineUtils.h"
#include "EnemyCharacter.h"
#include "UI_PlayWindow.h"

void UEnemyHealthBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AttackDelegate.BindDynamic(this, &UEnemyHealthBar::AttackEnded);
	BindToAnimationFinished(Anim_Attack, AttackDelegate);
	BloodDelegate.BindDynamic(this, &UEnemyHealthBar::BloodEnded);
	BindToAnimationFinished(Anim_Blood, BloodDelegate);
	BreatheDelegate.BindDynamic(this, &UEnemyHealthBar::BreatheEnded);
	BindToAnimationFinished(Anim_Breathe, BreatheDelegate);

	turn = 0;
	BreatheValue = floor(float(AForgottenKnightCharacter::MySheildLv - 1) * 0.5f) + 2; // 방어구 강화에 따른 숨고르기 대미지 증가량 변화

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

}

void UEnemyHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Image_AddEffect1->IsHovered() && IamShadowShadowSheild)
	{

		Text_PopupPassive->SetVisibility(ESlateVisibility::Visible);
		Image_PopupPassive->SetVisibility(ESlateVisibility::Visible);
		FString sStr = FString::Printf(TEXT("피해를 입을 때마다 자신을 제외한 모든 아군의 피해량이 증가합니다"));
		Text_PopupPassive->SetText(FText::FromString(sStr));
	}
	else if (Image_AddEffect1->IsHovered() && IamWarlock)
	{

		Text_PopupPassive->SetVisibility(ESlateVisibility::Visible);
		Image_PopupPassive->SetVisibility(ESlateVisibility::Visible);
		FString sStr = FString::Printf(TEXT("\n직접적인 공격에 의한 피해를 입지 않습니다"));
		Text_PopupPassive->SetText(FText::FromString(sStr));
	}
	else
	{
		Text_PopupPassive->SetVisibility(ESlateVisibility::Collapsed);
		Image_PopupPassive->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (DebuffImmune)
	{
		FName ATag = FName(*("Enemy" + FString::FromInt(MyIndex)));

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && (Enemy->ActorHasTag(ATag)))
			{
				Enemy->CleanzeDebuff();
			}
		}
	}

	if (IamWarlock || Lichimmune || IamCenturion)
	{
		Image_Immune->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_Immune->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (Image_Immune->IsHovered())
	{
		Text_PopupPassive->SetVisibility(ESlateVisibility::Visible);
		Image_PopupPassive->SetVisibility(ESlateVisibility::Visible);
		FString sStr = FString::Printf(TEXT("\n피해 면역 상태 입니다"));
		Text_PopupPassive->SetText(FText::FromString(sStr));
	}
	else
	{
		Text_PopupPassive->SetVisibility(ESlateVisibility::Collapsed);
		Image_PopupPassive->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UEnemyHealthBar::StatSet(int Health)
{
	MaxHealth = Health;
	CurrentHealth = Health;

	FString NewWString = FString::FromInt(Health);
	FText InWText = FText::FromString(NewWString);
	CurrentHealthText->SetText(InWText);

	NewWString = FString::FromInt(Health);
	InWText = FText::FromString(NewWString);
	MaxHealthText->SetText(InWText);
	HealthBar->SetPercent(1);
}

void UEnemyHealthBar::StathalfSet(int Health)
{
	MaxHealth = Health;
	CurrentHealth = Health / 2;

	FString NewWString = FString::FromInt(Health);
	FText InWText = FText::FromString(NewWString);
	CurrentHealthText->SetText(InWText);

	NewWString = FString::FromInt(Health);
	InWText = FText::FromString(NewWString);
	MaxHealthText->SetText(InWText);
	HealthBar->SetPercent(float(CurrentHealth) / float(MaxHealth));
}

void UEnemyHealthBar::UpdateNagativeEffect(int StackCorrosion, int StackWeaken, int StackBlooding)
{
	if (IamWarlock)
	{
		return;
	}

	CleanNagativeEffect();

	int numneg = 0;
	if (StackCorrosion > 0)
	{
		numneg += 1;
	}
	if (StackWeaken > 0)
	{
		numneg += 1;
	}
	if (StackBlooding > 0)
	{
		numneg += 1;
	}

	TArray<int> xArray = { 0, 0, 0, 0, 0 };

	for (int i = 0; i < numneg; i++)
	{
		if (Image_AddEffect1->GetVisibility() == ESlateVisibility::Collapsed)
		{
			if (IamShadowShadowSheild || IamWarlock)
			{
				Image_AddEffect1->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect1->SetBrush(Image_SpecialPassive);
				FText InText = FText::FromString("");
				Text_AddEffect1->SetText(InText);
				xArray[0] = -1;
			}
			else if (StackCorrosion > 0)
			{
				Image_AddEffect1->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect1->SetBrush(Image_Corrosion);
				FString NewString = FString::FromInt(StackCorrosion);
				FText InText = FText::FromString(NewString);
				Text_AddEffect1->SetText(InText);
				xArray[0] = 1;
			}
			else if (StackWeaken > 0)
			{
				Image_AddEffect1->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect1->SetBrush(Image_Weaken);
				FString NewString = FString::FromInt(StackWeaken);
				FText InText = FText::FromString(NewString);
				Text_AddEffect1->SetText(InText);
				xArray[0] = 2;
			}
			else if (StackBlooding > 0)
			{
				Image_AddEffect1->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect1->SetBrush(Image_Blooding);
				FString NewString = FString::FromInt(StackBlooding);
				FText InText = FText::FromString(NewString);
				Text_AddEffect1->SetText(InText);
				xArray[0] = 3;
			}
		}
		else if (Image_AddEffect2->GetVisibility() == ESlateVisibility::Collapsed)
		{
			if (StackCorrosion > 0 && xArray[0] != 1)
			{
				Image_AddEffect2->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect2->SetBrush(Image_Corrosion);
				FString NewString = FString::FromInt(StackCorrosion);
				FText InText = FText::FromString(NewString);
				Text_AddEffect2->SetText(InText);
				xArray[1] = 1;
			}
			else if (StackWeaken > 0 && xArray[0] != 2)
			{
				Image_AddEffect2->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect2->SetBrush(Image_Weaken);
				FString NewString = FString::FromInt(StackWeaken);
				FText InText = FText::FromString(NewString);
				Text_AddEffect2->SetText(InText);
				xArray[1] = 2;
			}
			else if (StackBlooding > 0 && xArray[0] != 3)
			{
				Image_AddEffect2->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect2->SetBrush(Image_Blooding);
				FString NewString = FString::FromInt(StackBlooding);
				FText InText = FText::FromString(NewString);
				Text_AddEffect2->SetText(InText);
				xArray[1] = 3;
			}
		}
		else if (Image_AddEffect3->GetVisibility() == ESlateVisibility::Collapsed)
		{
			if (StackCorrosion > 0 && xArray[0] != 1 && xArray[1] != 1)
			{
				Image_AddEffect3->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect3->SetBrush(Image_Corrosion);
				FString NewString = FString::FromInt(StackCorrosion);
				FText InText = FText::FromString(NewString);
				Text_AddEffect3->SetText(InText);
				xArray[2] = 1;
			}
			else if (StackWeaken > 0 && xArray[0] != 2 && xArray[1] != 2)
			{
				Image_AddEffect3->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect3->SetBrush(Image_Weaken);
				FString NewString = FString::FromInt(StackWeaken);
				FText InText = FText::FromString(NewString);
				Text_AddEffect3->SetText(InText);
				xArray[2] = 2;
			}
			else if (StackBlooding > 0 && xArray[0] != 3 && xArray[1] != 3)
			{
				Image_AddEffect3->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect3->SetBrush(Image_Blooding);
				FString NewString = FString::FromInt(StackBlooding);
				FText InText = FText::FromString(NewString);
				Text_AddEffect3->SetText(InText);
				xArray[2] = 3;
			}
		}
		else if (Image_AddEffect4->GetVisibility() == ESlateVisibility::Collapsed)
		{
			if (StackCorrosion > 0 && xArray[0] != 1 && xArray[1] != 1 && xArray[2] != 1)
			{
				Image_AddEffect4->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect4->SetBrush(Image_Corrosion);
				FString NewString = FString::FromInt(StackCorrosion);
				FText InText = FText::FromString(NewString);
				Text_AddEffect4->SetText(InText);
				xArray[3] = 1;
			}
			else if (StackWeaken > 0 && xArray[0] != 2 && xArray[1] != 2 && xArray[2] != 2)
			{
				Image_AddEffect4->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect4->SetBrush(Image_Weaken);
				FString NewString = FString::FromInt(StackWeaken);
				FText InText = FText::FromString(NewString);
				Text_AddEffect4->SetText(InText);
				xArray[3] = 2;
			}
			else if (StackBlooding > 0 && xArray[0] != 3 && xArray[1] != 3 && xArray[2] != 3)
			{
				Image_AddEffect4->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect4->SetBrush(Image_Blooding);
				FString NewString = FString::FromInt(StackBlooding);
				FText InText = FText::FromString(NewString);
				Text_AddEffect4->SetText(InText);
				xArray[3] = 3;
			}
		}
		else if (Image_AddEffect5->GetVisibility() == ESlateVisibility::Collapsed)
		{

		}
	}
}

void UEnemyHealthBar::CleanNagativeEffect()
{
	Image_AddEffect1->SetVisibility(ESlateVisibility::Collapsed);
	Image_AddEffect2->SetVisibility(ESlateVisibility::Collapsed);
	Image_AddEffect3->SetVisibility(ESlateVisibility::Collapsed);
	Image_AddEffect4->SetVisibility(ESlateVisibility::Collapsed);
	Image_AddEffect5->SetVisibility(ESlateVisibility::Collapsed);
	FText InText = FText::FromString("");
	Text_AddEffect1->SetText(InText);
	Text_AddEffect2->SetText(InText);
	Text_AddEffect3->SetText(InText);
	Text_AddEffect4->SetText(InText);
	Text_AddEffect5->SetText(InText);
}

void UEnemyHealthBar::DamageEffect(int Turn, int CardNum, bool Hit, int StackCorrosion, int StackWeaken, int StackBlooding)
{
	if (IamWarlock || Lichimmune || IamCenturion)
	{
		Hit = false;
	}

	turn = Turn; // 현재 턴 정보 전달

	//광역 대미지 이펙트적용
	if (GetAttakedWhole)
	{
		GetAttakedWhole = false;
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{ // 적 공격이 내게 적중시
				if ((Enemy->ActorHasTag("Enemy1")) || MyIndex != 1)
				{
					Enemy->UpdateHealth(0);
				}
				else if ((Enemy->ActorHasTag("Enemy2")) || MyIndex != 2)
				{
					Enemy->UpdateHealth(0);
				}
				else if ((Enemy->ActorHasTag("Enemy3")) || MyIndex != 3)
				{
					Enemy->UpdateHealth(0);
				}
			}
		}
	}

	if (CurrentHealth <= 0) // 생사 여부 확인
	{
		PlayAnimation(Anim_Die);
	}

	if (CardNum > 0) // 현재는 카드 종류 상관없이 해당 애니메이션 실행
	{
		Image_Effect->SetBrush(DamageEffect1);
		PlayAnimation(Anim_Attack);
	}
	else // 숨고르기 사용 시
	{
		AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
		MyController->CallAttack(turn);
	}

	UE_LOG(LogTemp, Log, TEXT("HealthBar Changed"));
	FString NewString = FString::FromInt(CurrentHealth);
	FText InText = FText::FromString(NewString);
	CurrentHealthText->SetText(InText);
	float Per = float(CurrentHealth) / float(MaxHealth);
	HealthBar->SetPercent(Per);

	if (Hit) // 공격 적중 여부 확인, 적중 시 디버프를 주는 효과 업데이트
	{
		UpdateNagativeEffect(StackCorrosion, StackWeaken, StackBlooding);
	}
}

void UEnemyHealthBar::AttackEnded()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
	int x = turn;
	turn = 0;
	MyController->CallAttack(x);
}

void UEnemyHealthBar::BloodEnded()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	MyPlayer->AllBloodEnd -= 1;
}

void UEnemyHealthBar::AttackSet(int Damage)
{
	FString NewString = FString::FromInt(Damage);
	FText InText = FText::FromString(NewString);
	DamageText->SetText(InText);

	if (NoScan)
	{
		InText = FText::FromString("?");
		DamageText->SetText(InText);
	}
}

void UEnemyHealthBar::GainNagative()
{
	if (!NoScan)
	{
		NagativeEffect->SetVisibility(ESlateVisibility::Visible);
	}
}

void UEnemyHealthBar::LoseNagative()
{
	NagativeEffect->SetVisibility(ESlateVisibility::Collapsed);
}

void UEnemyHealthBar::GainPositive()
{
	if (!NoScan)
	{
		PositiveEffect->SetVisibility(ESlateVisibility::Visible);
	}
}

void UEnemyHealthBar::LosePositive()
{
	PositiveEffect->SetVisibility(ESlateVisibility::Collapsed);
}

void UEnemyHealthBar::SetMobImage(FString MobName)
{
	FString sStr;
	if (MobName == "Slime")
	{
		Image_Mob->SetBrush(ImgSlime);
		//sStr = FString::Printf(TEXT("슬라임"));
		//Text_MobName->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Goblin")
	{
		Image_Mob->SetBrush(ImgGoblin);
		PlayAnimation(Resize_Goblin);
	}
	else if (MobName == "Troll")
	{
		Image_Mob->SetBrush(ImgTroll);
		PlayAnimation(Resize_Troll);
	}
	else if (MobName == "Spider") // 수정
	{
		Image_Mob->SetBrush(ImgSpider);
		PlayAnimation(Resize_Goblin);
		IamSpider = true;
	}
	else if (MobName == "Mantis")
	{
		Image_Mob->SetBrush(ImgMantis);
		PlayAnimation(Resize_Mantis);
	}
	else if (MobName == "Golem")
	{
		Image_Mob->SetBrush(ImgGolem);
		PlayAnimation(Resize_Golem);
	}
	else if (MobName == "Asura") // 수정
	{
		Image_Mob->SetBrush(ImgAsura);
		PlayAnimation(Resize_Asura);
		IamAsura = true;
	}
	else if (MobName == "Asura_Up") // 수정
	{
		Image_Mob->SetBrush(ImgAsura);
		PlayAnimation(Resize_Asura);
		IamAsura = true;
		IamAsuraup = true;
	}
	else if (MobName == "ShadowSpear") // 수정
	{
		Image_Mob->SetBrush(ImgShadowSpear);
		PlayAnimation(Resize_Goblin);
	}
	else if (MobName == "ShadowSheild") // 수정
	{
		Image_Mob->SetBrush(ImgShadowSheild);
		PlayAnimation(Resize_Goblin);
		IamShadowShadowSheild = true;
	}
	else if (MobName == "ShadowAxe") // 수정
	{
		Image_Mob->SetBrush(ImgShadowAxe);
		PlayAnimation(Resize_Goblin);
	}
	else if (MobName == "Griffon") // 수정
	{
		Image_Mob->SetBrush(ImgGriffon);
		PlayAnimation(Resize_Golem);
	}
	else if (MobName == "Rafflesia") // 수정
	{
		Image_Mob->SetBrush(ImgRafflesia);
		PlayAnimation(Resize_Goblin);
	}
	else if (MobName == "Warlock2Phase") // 수정
	{
		Image_Mob->SetBrush(ImgWarlock2Phase);
		PlayAnimation(Resize_NormalSize);
		IamWarlock = false;
		IamWarlock2Phase = true;
		Image_Mob->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (MobName == "Warlock") // 수정
	{
		Image_Mob->SetBrush(ImgWarlock);
		PlayAnimation(Resize_Golem);
		IamWarlock = true;
	}
	else if (MobName == "SkullHead") // 수정
	{
		Image_Mob->SetBrush(ImgSkullHead);
		PlayAnimation(Resize_NormalSize);
		Skullmode = true;
	}
	else if (MobName == "SkeletonSoldier") // 수정
	{
		Image_Mob->SetBrush(ImgSkeletonSoldier);
		PlayAnimation(Resize_Troll);
		Skullmode = false;
		IamSkeleton = 1;
	}
	else if (MobName == "SkeletonArcher") // 수정
	{
		Image_Mob->SetBrush(ImgSkeletonArcher);
		PlayAnimation(Resize_Troll);
		Skullmode = false;
		IamSkeleton = 2;
	}
	else if (MobName == "SkeletonGuardDog") // 수정
	{
		Image_Mob->SetBrush(ImgSkeletonGuardDog);
		PlayAnimation(Resize_Goblin);
		Skullmode = false;
		IamSkeleton = 3;
	}
	else if (MobName == "Lich") // 수정
	{
		Image_Mob->SetBrush(ImgLich);
		PlayAnimation(Resize_Troll);
		Skullmode = false;
		IamSkeleton = 4;
		IamLich = true;
	}
	else if (MobName == "Beholder")
	{
		Image_Mob->SetBrush(ImgBeholder);
		PlayAnimation(Resize_Troll);
	}
	else if (MobName == "FallenFairy")
	{
		Image_Mob->SetBrush(ImgFallenFairy);
		PlayAnimation(Resize_Troll);
	}
	else if (MobName == "Baphomet")
	{
		Image_Mob->SetBrush(ImgBaphomet);
		PlayAnimation(Resize_Asura);
	}
	else if (MobName == "Durahan")
	{
		Image_Mob->SetBrush(ImgDurahan);
		PlayAnimation(Resize_Golem);
	}
	else if (MobName == "SkeletonCenturion")
	{
		Image_Mob->SetBrush(ImgSkeletonCenturion);
		PlayAnimation(Resize_Troll);
		IamCenturion = true;
	}
	else if (MobName == "DemonLordHead")
	{
		Image_Mob->SetBrush(ImgDemonLordHead);
		PlayAnimation(Resize_NormalSize);
		IamDemonLord = 2;
		MawangReviveTime = 2;
		Image_Mob->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (MobName == "DemonLordLeft")
	{
		Image_Mob->SetBrush(ImgDemonLordLeft);
		PlayAnimation(Resize_NormalSize);
		IamDemonLord = 1;
		MawangReviveTime = 3;
		Image_Mob->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (MobName == "DemonLordRight")
	{
		Image_Mob->SetBrush(ImgDemonLordRight);
		PlayAnimation(Resize_NormalSize);
		IamDemonLord = 3;
		MawangReviveTime = 3;
		Image_Mob->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (MobName == "Ball1")
	{
		Image_Mob->SetBrush(ImgBall1);
		PlayAnimation(Resize_NormalSize);
		IamBall = true;
	}
	else if (MobName == "Ball2")
	{
		Image_Mob->SetBrush(ImgBall2);
		PlayAnimation(Resize_NormalSize);
		IamBall = true;
	}
	else if (MobName == "Ball3")
	{
		Image_Mob->SetBrush(ImgBall3);
		PlayAnimation(Resize_NormalSize);
		IamBall = true;
	}
	else if (MobName == "Ball4")
	{
		Image_Mob->SetBrush(ImgBall4);
		PlayAnimation(Resize_NormalSize);
		IamBall = true;
	}
	else if (MobName == "Ball5")
	{
		Image_Mob->SetBrush(ImgBall5);
		PlayAnimation(Resize_NormalSize);
		IamBall = true;
	}
	else if (MobName == "Ball6")
	{
		Image_Mob->SetBrush(ImgBall6);
		PlayAnimation(Resize_NormalSize);
		IamBall = true;
	}
	else if (MobName == "Ball7")
	{
		Image_Mob->SetBrush(ImgBall7);
		PlayAnimation(Resize_NormalSize);
		IamBall = true;
	}

	if (MobName != "Spider")
	{
		IamSpider = false;
	}

	if ((MobName != "Ball1") && (MobName != "Ball2") && (MobName != "Ball3") && (MobName != "Ball4") && (MobName != "Ball5") && (MobName != "Ball6") && (MobName != "Ball7"))
	{
		IamBall = false;
	}

	if (MobName != "Lich")
	{
		IamLich = false;
	}

	if (MobName != "SkullHead")
	{
		Skullmode = false;
	}
}

void UEnemyHealthBar::MadnessChange(int MobNum, int Damage)
{
	if (Damage > 1) // 대미지가 2 이상이면 치환
	{
		int Bloodvalue = Damage / 2;

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1") && MobNum == 1)
				{
					Enemy->ConfirmedStackBlooding += Bloodvalue;
				}
				else if (Enemy->ActorHasTag("Enemy2") && MobNum == 2)
				{
					Enemy->ConfirmedStackBlooding += Bloodvalue;
				}
				else if (Enemy->ActorHasTag("Enemy3") && MobNum == 3)
				{
					Enemy->ConfirmedStackBlooding += Bloodvalue;
				}
			}
		}


	}
}

/*void UEnemyHealthBar::SetAttackSpeed(int Speed)
{
	if (Speed == 1) // 빠공
	{
		Image_AttackSpeed->SetVisibility(ESlateVisibility::Visible);
		Image_AttackSpeed->SetBrush(Image_AttackFast);
	}
	else if (Speed == 2) // 기본공
	{
		Image_AttackSpeed->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (Speed == 3) // 느린공
	{
		Image_AttackSpeed->SetVisibility(ESlateVisibility::Visible);
		Image_AttackSpeed->SetBrush(Image_AttackSlow);
	}
}*/

void UEnemyHealthBar::CallMyIndex(int MyOrder)
{
	MyIndex = MyOrder;
}

void UEnemyHealthBar::ParryingEffect()
{
	Image_Effect->SetBrush(ParryingEffect1);
	PlayAnimation(Anim_Attack);
}

void UEnemyHealthBar::BloodEffect()
{
	if (IamBall)
	{
		return;
	}

	if (CurrentHealth <= 0) // 생사 여부 확인
	{
		PlayAnimation(Anim_Die);
	}
	PlayAnimation(Anim_Blood);

	FString NewString = FString::FromInt(CurrentHealth);
	FText InText = FText::FromString(NewString);
	CurrentHealthText->SetText(InText);
	float Per = float(CurrentHealth) / float(MaxHealth);
	HealthBar->SetPercent(Per);

}

void UEnemyHealthBar::OnBreatheEffect(int Turn)
{
	Breatheturn = Turn;

	PlayAnimation(Anim_Breathe);
}

void UEnemyHealthBar::GoAttack()
{
	if (IamWarlock)
	{
		return;
	}

	PlayAnimation(Anim_GoAttack);

}

void UEnemyHealthBar::UpdateHealthBar(int heal)
{
	CurrentHealth += heal;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	else if (CurrentHealth < 0)
	{
		CurrentHealth = 0;
	}

	FString NewWString = FString::FromInt(CurrentHealth);
	FText InWText = FText::FromString(NewWString);
	CurrentHealthText->SetText(InWText);

	NewWString = FString::FromInt(MaxHealth);
	InWText = FText::FromString(NewWString);
	MaxHealthText->SetText(InWText);

	float PerW = float(CurrentHealth) / float(MaxHealth);
	HealthBar->SetPercent(PerW);
}

void UEnemyHealthBar::Revive()
{
	PlayAnimation(Anim_Revive);

	if (IamAsura)
	{
		StatSet(30);
	}
	else if (IamAsuraup)
	{
		StatSet(39);
	}
}

void UEnemyHealthBar::ConnectImage(int CardNum, int Order)
{
	if (InImage != nullptr)
	{
		FString NewString;
		FText InText;

		if (CardNum == 0)
		{
			InImage->SetVisibility(ESlateVisibility::Hidden);
			InImage_Background->SetVisibility(ESlateVisibility::Hidden);
			Image_CardAttack->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);
		}
		else if (CardNum == 1 || CardNum == 101)
		{
			InImage->SetBrush(Image1);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);
			if (CardNum == 1)
			{
				NewString = FString::FromInt(CardDamage[1]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[1]+2);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 2 || CardNum == 102)
		{
			InImage->SetBrush(Image2);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 2)
			{
				NewString = FString::FromInt(CardDamage[2]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[2] + 1);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
			if (CardNum == 2)
			{
				NewString = FString::FromInt(4);
			}
			else
			{
				NewString = FString::FromInt(6);
			}
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Blooding);
		}
		else if (CardNum == 3 || CardNum == 103)
		{
			InImage->SetBrush(Image3);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (Order == 2)
			{
				if (CardNum == 3)
				{
					NewString = FString::FromInt(CardDamage[3] * 2);
				}
				else
				{
					NewString = FString::FromInt(CardDamage[3] * 3);
				}
			}
			else
			{
				NewString = FString::FromInt(CardDamage[3]);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 4 || CardNum == 104)
		{
			InImage->SetBrush(Image4);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 4)
			{
				NewString = FString::FromInt(CardDamage[4]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[4] + 4);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 5 || CardNum == 105)
		{
			InImage->SetBrush(Image5);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			if (Order == 1)
			{
				Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

				if (CardNum == 5)
				{
					NewString = FString::FromInt(2);
				}
				else
				{
					NewString = FString::FromInt(3);
				}
				InText = FText::FromString(NewString);
				Text_CardAttack->SetText(InText);
			}
			else if (Order == 2)
			{
				Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

				if (CardNum == 5)
				{
					NewString = FString::FromInt(4);
				}
				else
				{
					NewString = FString::FromInt(6);
				}
				InText = FText::FromString(NewString);
				Text_CardAttack->SetText(InText);
				NewString = FString::FromInt(2);
				InText = FText::FromString(NewString);
				Text_CardEffect1->SetText(InText);
				Image_CardEffect1->SetBrush(Image_Blooding);
			}
			else if (Order == 3)
			{
				Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

				if (CardNum == 5)
				{
					NewString = FString::FromInt(6);
				}
				else
				{
					NewString = FString::FromInt(8);
				}
				InText = FText::FromString(NewString);
				Text_CardAttack->SetText(InText);
			}
		}
		else if (CardNum == 6 || CardNum == 106)
		{
			InImage->SetBrush(Image6);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			if (Order == 1)
			{
				Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

				if (CardNum == 6)
				{
					NewString = FString::FromInt(4);
				}
				else
				{
					NewString = FString::FromInt(5);
				}
				InText = FText::FromString(NewString);
				Text_CardAttack->SetText(InText);
				if (CardNum == 6)
				{
					NewString = FString::FromInt(2);
				}
				else
				{
					NewString = FString::FromInt(3);
				}
				InText = FText::FromString(NewString);
				Text_CardEffect1->SetText(InText);
				Image_CardEffect1->SetBrush(Image_Weaken);
			}
			else if (Order == 2)
			{
				Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

				if (CardNum == 6)
				{
					NewString = FString::FromInt(6);
				}
				else
				{
					NewString = FString::FromInt(8);
				}
				InText = FText::FromString(NewString);
				Text_CardAttack->SetText(InText);
				if (CardNum == 6)
				{
					NewString = FString::FromInt(2);
				}
				else
				{
					NewString = FString::FromInt(3);
				}
				InText = FText::FromString(NewString);
				Text_CardEffect1->SetText(InText);
				Image_CardEffect1->SetBrush(Image_Corrosion);
			}
			else if (Order == 3)
			{
				Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				LifeStealEffect->SetVisibility(ESlateVisibility::Visible);

				if (CardNum == 6)
				{
					NewString = FString::FromInt(6);
				}
				else
				{
					NewString = FString::FromInt(8);
				}
				InText = FText::FromString(NewString);
				Text_CardAttack->SetText(InText);
			}
		}
		else if (CardNum == 7 || CardNum == 107)
		{
			InImage->SetBrush(Image7);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 7)
			{
				NewString = FString::FromInt(CardDamage[7]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[7] + 1);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
			NewString = FString::FromInt(2);
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Weaken);
		}
		else if (CardNum == 8 || CardNum == 108)
		{
			InImage->SetBrush(Image8);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 8)
			{
				NewString = FString::FromInt(CardDamage[8]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[8] + 1);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 9 || CardNum == 109)
		{
			InImage->SetBrush(Image9);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			NewString = FString::FromInt(CardDamage[9]);
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 10 || CardNum == 110)
		{
			InImage->SetBrush(Image10);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Visible);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Visible);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 10)
			{
				NewString = FString::FromInt(CardDamage[10]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[10] + 3);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
			NewString = FString::FromInt(2);
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Text_CardEffect2->SetText(InText);
			Text_CardEffect3->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Corrosion);
			Image_CardEffect2->SetBrush(Image_Weaken);
			Image_CardEffect3->SetBrush(Image_Blooding);
		}
		else if (CardNum == 11 || CardNum == 111)
		{
			InImage->SetBrush(Image11);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);
			NewString = FString::FromInt(0);
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 12 || CardNum == 112)
		{
			InImage->SetBrush(Image12);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Visible);

			if (CardNum == 12)
			{
				NewString = FString::FromInt(CardDamage[12]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[12] + 5);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 13 || CardNum == 113)
		{
			InImage->SetBrush(Image13);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 13)
			{
				NewString = FString::FromInt(CardDamage[13]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[13] + 1);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 14 || CardNum == 114)
		{
			InImage->SetBrush(Image14);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 14)
			{
				NewString = FString::FromInt(CardDamage[14]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[14] + 2);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 15 || CardNum == 115)
		{
			InImage->SetBrush(Image15);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 15)
			{
				NewString = FString::FromInt(CardDamage[15]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[15] + 2);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 16 || CardNum == 116)
		{
			InImage->SetBrush(Image16);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 16)
			{
				NewString = FString::FromInt(CardDamage[16]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[16] + 1);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 17 || CardNum == 117)
		{
			InImage->SetBrush(Image17);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			NewString = FString::FromInt(CardDamage[17]);
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 18 || CardNum == 118)
		{
			InImage->SetBrush(Image18);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 18)
			{
				NewString = FString::FromInt(CardDamage[18]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[18] + 2);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 19 || CardNum == 119)
		{
			InImage->SetBrush(Image19);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 19)
			{
				NewString = FString::FromInt(CardDamage[19]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[19]);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);

			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter) // 플레이어가 공격
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr)
				{
					if (Enemy->ActorHasTag("Enemy1") && MyIndex == 1)
					{
						if (Enemy->GetStackBlooding > 0)
						{
							if (CardNum == 19)
							{
								NewString = FString::FromInt(4);
							}
							else
							{
								NewString = FString::FromInt(6);
							}
						}
					}
					else if (Enemy->ActorHasTag("Enemy2") && MyIndex == 2)
					{
						if (Enemy->GetStackBlooding > 0)
						{
							if (CardNum == 19)
							{
								NewString = FString::FromInt(4);
							}
							else
							{
								NewString = FString::FromInt(6);
							}
						}
					}
					else if (Enemy->ActorHasTag("Enemy3") && MyIndex == 3)
					{
						if (Enemy->GetStackBlooding > 0)
						{
							if (CardNum == 19)
							{
								NewString = FString::FromInt(4);
							}
							else
							{
								NewString = FString::FromInt(6);
							}
						}
					}
				}
			}
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Blooding);
		}
		else if (CardNum == 20 || CardNum == 120)
		{
			InImage->SetBrush(Image20);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			if (Order == 1)
			{
				Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

				if (CardNum == 20)
				{
					NewString = FString::FromInt(8);
				}
				else
				{
					NewString = FString::FromInt(10);
				}
				InText = FText::FromString(NewString);
				Text_CardAttack->SetText(InText);
			}
			else if (Order == 2)
			{
				Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

				if (CardNum == 20)
				{
					NewString = FString::FromInt(6);
				}
				else
				{
					NewString = FString::FromInt(8);
				}
				InText = FText::FromString(NewString);
				Text_CardAttack->SetText(InText);
			}
			else if (Order == 3)
			{
				Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

				if (CardNum == 20)
				{
					NewString = FString::FromInt(4);
				}
				else
				{
					NewString = FString::FromInt(6);
				}
				InText = FText::FromString(NewString);
				Text_CardAttack->SetText(InText);
			}
		}
		else if (CardNum == 21 || CardNum == 121)
		{
			InImage->SetBrush(Image21);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			NewString = FString::FromInt(CardDamage[21]);
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 22 || CardNum == 122)
		{
			InImage->SetBrush(Image22);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			NewString = FString::FromInt(CardDamage[22]);
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 24 || CardNum == 124)
		{
			InImage->SetBrush(Image24);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 24)
			{
				NewString = FString::FromInt(CardDamage[24]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[24] + 1);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
			if (CardNum == 24)
			{
				NewString = FString::FromInt(2);
			}
			else
			{
				NewString = FString::FromInt(3);
			}
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Corrosion);
		}
		else if (CardNum == 25 || CardNum == 125)
		{
			InImage->SetBrush(Image25);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 25)
			{
				NewString = FString::FromInt(CardDamage[25]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[25] + 1);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
			if (CardNum == 25)
			{
				NewString = FString::FromInt(3);
			}
			else
			{
				NewString = FString::FromInt(4);
			}
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Blooding);
		}
		else if (CardNum == 26 || CardNum == 126)
		{
			InImage->SetBrush(Image26);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 26)
			{
				NewString = FString::FromInt(CardDamage[26]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[26] + 1);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
			if (CardNum == 26)
			{
				NewString = FString::FromInt(3);
			}
			else
			{
				NewString = FString::FromInt(4);
			}
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Corrosion);
		}
		else if (CardNum == 27 || CardNum == 127)
		{
			InImage->SetBrush(Image27);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 27)
			{
				NewString = FString::FromInt(CardDamage[27]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[27] + 1);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
			if (CardNum == 27)
			{
				NewString = FString::FromInt(3);
			}
			else
			{
				NewString = FString::FromInt(4);
			}
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Weaken);
		}
		else if (CardNum == 31 || CardNum == 131)
		{
			InImage->SetBrush(Image31);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			if (CardNum == 31)
			{
				NewString = FString::FromInt(CardDamage[31]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[31] + 2);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 32 || CardNum == 132)
		{
			InImage->SetBrush(Image32);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

			NewString = FString::FromInt(CardDamage[32]);
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 34 || CardNum == 134)
		{
			InImage->SetBrush(Image34);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			if (Order == 1)
			{
				Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect2->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect3->SetVisibility(ESlateVisibility::Visible);
				Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect2->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect3->SetVisibility(ESlateVisibility::Visible);
				LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

				if (CardNum == 34)
				{
					NewString = FString::FromInt(1);
				}
				else
				{
					NewString = FString::FromInt(2);
				}
				InText = FText::FromString(NewString);
				Text_CardAttack->SetText(InText);
				if (CardNum == 34)
				{
					NewString = FString::FromInt(2);
				}
				else
				{
					NewString = FString::FromInt(3);
				}
				InText = FText::FromString(NewString);
				Text_CardEffect1->SetText(InText);
				Image_CardEffect1->SetBrush(Image_Corrosion);
				Text_CardEffect2->SetText(InText);
				Image_CardEffect2->SetBrush(Image_Weaken);
				Text_CardEffect3->SetText(InText);
				Image_CardEffect3->SetBrush(Image_Blooding);
			}
			else if (Order == 2)
			{
				Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

				if (CardNum == 34)
				{
					InText = FText::FromString("=");
				}
				else
				{
					InText = FText::FromString("+4");
				}
				Text_CardAttack->SetText(InText);
			}
			else if (Order == 3)
			{
				Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
				LifeStealEffect->SetVisibility(ESlateVisibility::Visible);

				if (CardNum == 34)
				{
					NewString = FString::FromInt(10);
				}
				else
				{
					NewString = FString::FromInt(12);
				}
				InText = FText::FromString(NewString);
				Text_CardAttack->SetText(InText);
			}
		}
		else if (CardNum == 35 || CardNum == 135)
		{
		InImage->SetBrush(Image35);
		InImage->SetVisibility(ESlateVisibility::Visible);
		InImage_Background->SetVisibility(ESlateVisibility::Visible);

		Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
		Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
		Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
		Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
		Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
		Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
		Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
		Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
		LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

		int kg = MaxHealth - CurrentHealth; // 잃은 체력

		if (CardNum == 35)
		{
			if (kg > 30)
			{
				kg = 30;
			}
		}
		else
		{
			if (kg > 50)
			{
				kg = 50;
			}
		}
		NewString = FString::FromInt(kg);
		InText = FText::FromString(NewString);
		Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 36 || CardNum == 136)
		{
		InImage->SetBrush(Image36);
		InImage->SetVisibility(ESlateVisibility::Visible);
		InImage_Background->SetVisibility(ESlateVisibility::Visible);

		Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
		Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
		Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
		Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
		Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
		Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
		Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
		Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
		LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

		int bae = 1;

		if (CardNum == 36)
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr)
				{
					if (Enemy->ActorHasTag("Enemy1") && MyIndex == 1)
					{
						if (Enemy->GetStackBlooding > 0)
						{
							bae *= 2;
						}
						else if (Enemy->GetStackCorrosion > 0)
						{
							bae *= 2;
						}
						else if (Enemy->GetStackWeakening > 0)
						{
							bae *= 2;
						}
					}
					else if (Enemy->ActorHasTag("Enemy2") && MyIndex == 2)
					{
						if (Enemy->GetStackBlooding > 0)
						{
							bae *= 2;
						}
						else if (Enemy->GetStackCorrosion > 0)
						{
							bae *= 2;
						}
						else if (Enemy->GetStackWeakening > 0)
						{
							bae *= 2;
						}
					}
					else if (Enemy->ActorHasTag("Enemy3") && MyIndex == 3)
					{
						if (Enemy->GetStackBlooding > 0)
						{
							bae *= 2;
						}
						else if (Enemy->GetStackCorrosion > 0)
						{
							bae *= 2;
						}
						else if (Enemy->GetStackWeakening > 0)
						{
							bae *= 2;
						}
					}
				}
			}
		}
		else
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr)
				{
					if (Enemy->ActorHasTag("Enemy1") && MyIndex == 1)
					{
						if (Enemy->GetStackBlooding > 0)
						{
							bae *= 3;
						}
						else if (Enemy->GetStackCorrosion > 0)
						{
							bae *= 3;
						}
						else if (Enemy->GetStackWeakening > 0)
						{
							bae *= 3;
						}
					}
					else if (Enemy->ActorHasTag("Enemy2") && MyIndex == 2)
					{
						if (Enemy->GetStackBlooding > 0)
						{
							bae *= 3;
						}
						else if (Enemy->GetStackCorrosion > 0)
						{
							bae *= 3;
						}
						else if (Enemy->GetStackWeakening > 0)
						{
							bae *= 3;
						}
					}
					else if (Enemy->ActorHasTag("Enemy3") && MyIndex == 3)
					{
						if (Enemy->GetStackBlooding > 0)
						{
							bae *= 3;
						}
						else if (Enemy->GetStackCorrosion > 0)
						{
							bae *= 3;
						}
						else if (Enemy->GetStackWeakening > 0)
						{
							bae *= 3;
						}
					}
				}
			}
		}
		NewString = FString::FromInt(bae);
		InText = FText::FromString(NewString);
		Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 41 || CardNum == 141)
		{
			InImage->SetBrush(Image41);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);
			if (CardNum == 41)
			{
				NewString = FString::FromInt(CardDamage[41]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[41] + 0);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
			if (CardNum == 41)
			{
				NewString = FString::FromInt(2);
			}
			else
			{
				NewString = FString::FromInt(4);
			}
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Blooding);
		}
		else if (CardNum == 42 || CardNum == 142)
		{
			InImage->SetBrush(Image42);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);
			if (CardNum == 42)
			{
				NewString = FString::FromInt(CardDamage[42]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[42] + 1);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
			if (CardNum == 42)
			{
				NewString = FString::FromInt(4);
			}
			else
			{
				NewString = FString::FromInt(6);
			}
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Blooding);
		}
		else if (CardNum == 43 || CardNum == 143)
		{
			InImage->SetBrush(Image43);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);
			if (CardNum == 43)
			{
				NewString = FString::FromInt(CardDamage[43]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[43] + 1);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
			if (CardNum == 43)
			{
				NewString = FString::FromInt(2);
			}
			else
			{
				NewString = FString::FromInt(4);
			}
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Blooding);
		}
		else if (CardNum == 45 || CardNum == 145)
		{
			InImage->SetBrush(Image45);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);
			if (CardNum == 45)
			{
				NewString = FString::FromInt(CardDamage[45]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[45] + 3);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
			if (CardNum == 45)
			{
				NewString = ("x");
			}
			else
			{
				NewString = ("x");
			}
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Blooding);
		}
		else if (CardNum == 46 || CardNum == 146)
		{
			InImage->SetBrush(Image46);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);
			if (CardNum == 46)
			{
				NewString = FString::FromInt(CardDamage[46]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[46] + 4);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 47 || CardNum == 147)
		{
			InImage->SetBrush(Image47);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);
			if (CardNum == 47)
			{
				NewString = FString::FromInt(CardDamage[47]);
			}
			else
			{
				NewString = FString::FromInt(CardDamage[47] + 8);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 48 || CardNum == 148)
		{
			InImage->SetBrush(Image48);
			InImage->SetVisibility(ESlateVisibility::Visible);
			InImage_Background->SetVisibility(ESlateVisibility::Visible);

			Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
			Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
			Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
			LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);
			if (CardNum == 48)
			{
				Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
				NewString = FString::FromInt(CardDamage[48]);
			}
			else
			{
				Image_CardEffect1->SetVisibility(ESlateVisibility::Visible);
				Text_CardEffect1->SetVisibility(ESlateVisibility::Visible);
				NewString = FString::FromInt(CardDamage[48] + 0);
			}
			InText = FText::FromString(NewString);
			Text_CardAttack->SetText(InText);
			if (CardNum == 148)
			{
				NewString = FString::FromInt(2);
			}
			InText = FText::FromString(NewString);
			Text_CardEffect1->SetText(InText);
			Image_CardEffect1->SetBrush(Image_Corrosion);
		}
		else if (CardNum == 49 || CardNum == 149)
		{
		InImage->SetBrush(Image49);
		InImage->SetVisibility(ESlateVisibility::Visible);
		InImage_Background->SetVisibility(ESlateVisibility::Visible);

		Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
		Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
		Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
		Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
		Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
		Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
		Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
		Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
		LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);
		if (CardNum == 49)
		{
			NewString = FString::FromInt(CardDamage[49]);
		}
		else
		{
			NewString = FString::FromInt(CardDamage[49] + 2);
		}
		InText = FText::FromString(NewString);
		Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 50 || CardNum == 150)
		{
		InImage->SetBrush(Image50);
		InImage->SetVisibility(ESlateVisibility::Visible);
		InImage_Background->SetVisibility(ESlateVisibility::Visible);

		Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
		Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
		Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
		Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
		Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
		Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
		Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
		Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
		LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);
		if (CardNum == 50)
		{
			NewString = FString::FromInt(CardDamage[50]);
		}
		else
		{
			NewString = FString::FromInt(CardDamage[50] + 2);
		}
		InText = FText::FromString(NewString);
		Text_CardAttack->SetText(InText);
		}
		else if (CardNum == 53 || CardNum == 153)
		{
		InImage->SetBrush(Image53);
		InImage->SetVisibility(ESlateVisibility::Visible);
		InImage_Background->SetVisibility(ESlateVisibility::Visible);

		Image_CardAttack->SetVisibility(ESlateVisibility::Visible);
		Image_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
		Image_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
		Image_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
		Text_CardAttack->SetVisibility(ESlateVisibility::Visible);
		Text_CardEffect1->SetVisibility(ESlateVisibility::Hidden);
		Text_CardEffect2->SetVisibility(ESlateVisibility::Hidden);
		Text_CardEffect3->SetVisibility(ESlateVisibility::Hidden);
		LifeStealEffect->SetVisibility(ESlateVisibility::Collapsed);

		NewString = ("?");
		InText = FText::FromString(NewString);
		Text_CardAttack->SetText(InText);
		}

	}

	if (Order == 0)
	{
		FText InText = FText::FromString(FString(TEXT(" ")));
		OrderText->SetText(InText);
	}
	else
	{
		FString NewString = FString::FromInt(Order);
		FText InText = FText::FromString(NewString);
		OrderText->SetText(InText);
	}

}

void UEnemyHealthBar::ChangeBreatheEffect(bool TurnOn)
{
	if (TurnOn == true)
	{
		BreatheEffect->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BreatheEffect->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UEnemyHealthBar::GetDamaged(int Damage)
{
	FName EnemyTag = FName(*("Enemy" + FString::FromInt(MyIndex)));

	//오의 절멸3 광역 대미지 적용
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		if (MyPlayer->ExtinctionWholeAttackDone)
		{
			MyPlayer->ExtinctionWholeAttackDone = false;
			GetAttakedWhole = true;
			AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
			if (MyController)
			{
				UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
				if (MyWindow)
				{
					UEnemyHealthBar* healthbar = nullptr;
					if (MyIndex == 1)
					{
						healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
						if (healthbar)
						{
							healthbar->GetDamaged(Damage);
						}
						healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
						if (healthbar)
						{
							healthbar->GetDamaged(Damage);
						}
					}
					else if (MyIndex == 2)
					{
						healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
						if (healthbar)
						{
							healthbar->GetDamaged(Damage);
						}
						healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
						if (healthbar)
						{
							healthbar->GetDamaged(Damage);
						}
					}
					else if (MyIndex == 3)
					{
						healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
						if (healthbar)
						{
							healthbar->GetDamaged(Damage);
						}
						healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
						if (healthbar)
						{
							healthbar->GetDamaged(Damage);
						}
					}
				}
			}
		}
	}

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && (Enemy->ActorHasTag(EnemyTag)) && Enemy->Life)
		{
			if (Enemy->firstturnbeholder == 1 && Enemy->ActorHasTag("Beholder")) // 눈 뜬 상태
			{
				Damage += Damage / 2;
				Enemy->GetStackBlooding += 2;
				Enemy->GetStackCorrosion += 2;
			}
			else if (Enemy->firstturnbeholder == 2 && Enemy->ActorHasTag("Beholder")) // 눈 감은 상태
			{
				Damage -= Damage / 2;
				MyPlayer->GetStackWeakening += 2;
				AForgottenKnightCharacter::MyCurrentHealth -= 10;
				MyPlayer->GetStackCharmed[MyIndex] = true;
			}
			else if (Enemy->ActorHasTag("FallenFairy"))
			{
				Damage -= (Damage * 7) / 10;
			}
			else if (Enemy->GetPowerHit)
			{
				Damage = round(float(Damage) * 1.25);
			}
		}
	}

	Damage -= BonusDefenseValue; // 뎀감 적용

	if (Damage < 0)
	{
		Damage = 0;
	}

	if (IamWarlock || Lichimmune || IamCenturion)
	{
		Damage = 0;
	}

	// 마법구 기믹
	if (IamBall)
	{
		if (Attackorderthisturn == 3) // 카운터 턴에는 그대로 피해 받음
		{
			int overdamage = Damage;
			if (MyPlayer)
			{
				AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
				if (MyController)
				{
					MyController->LoseKingHP(overdamage);
				}
			}
		}
		else if ((Damage - CurrentHealth > 0)) // 마법구에게 초과대미지를 주었다면
		{
			int overdamage = Damage - CurrentHealth;

			if (MyPlayer)
			{
				AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
				if (MyController)
				{
					MyController->LoseKingHP(overdamage);
				}
			}
		}

		Attackorderthisturn = 0; // 초기화
	}

	CurrentHealth -= Damage;

	FString NewWString = FString::FromInt(Damage);
	FText InWText = FText::FromString(NewWString);
	Text_Damage->SetText(InWText);

	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
	}

	if (Damage > 0)
	{
		if (IamSpider && (CurrentHealth * 3 <= MaxHealth))
		{
			Image_Mob->SetBrush(ImgSpider3);
			
		}
		else if (IamSpider && (round((CurrentHealth * 3) * 0.5) <= MaxHealth))
		{
			Image_Mob->SetBrush(ImgSpider2);
		}

		if (IamShadowShadowSheild) // 내가 그림자 방패병이면, 피해 받고 아군 댐+1
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr && !(Enemy->ActorHasTag(EnemyTag)) && Enemy->Life)
				{
					Enemy->BonusDamageValue += 1;
				}
			}
		}
		return true;
	}

	return false;
}

void UEnemyHealthBar::GetBloodDamaged(int Damage)
{
	if (IamWarlock || Lichimmune || IamBall)
	{
		return;
	}

	CurrentHealth -= Damage;

	FString NewWString = FString::FromInt(Damage);
	FText InWText = FText::FromString(NewWString);
	Text_Damage->SetText(InWText);

	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
	}
}

bool UEnemyHealthBar::TurnEnd(int mobnum, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken)
{
	if (IamWarlock || Lichimmune)
	{
		return 0;
	}

	Attackorderthisturn = Order;

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (GetWeaken && EnemyCorrosion) // 나(적) 약화, 상대(나) 부식
	{
		Damage = round(float(Damage) * myweak * 1.25f);
	}
	else if (GetWeaken && !EnemyCorrosion) // 나(적) 약화, 상대(나) 부식x
	{
		Damage = round(float(Damage) * myweak);
	}
	else if (!GetWeaken && EnemyCorrosion) // 나(적) 약화x, 상대(나) 부식
	{
		Damage = round(float(Damage) * 1.25f);
	}
	else if (!GetWeaken && !EnemyCorrosion) // 나(적) 약화x, 상대(나) 부식x
	{
		Damage = round(float(Damage));
	}

	int findamage = 0;
	if (Card < 100) // 일반 카드
	{ // 카드 대미지 + 무기 레벨에 따른 추가 대미지
		findamage = CardDamage[Card] + 2*(AForgottenKnightCharacter::MySwordLv - 1);
	}
	else // 강화 카드
	{
		findamage = CardDamage[Card - 100] + 2*(AForgottenKnightCharacter::MySwordLv - 1);
	}

	if (MyPlayer->BeMadness) // 광기 발현 상태이면
	{
		if (Card != 5 && Card != 6 && Card != 20 && Card != 34 && Card != 105 && Card != 106 && Card != 120 && Card != 134) // 변화 카드가 아니면
		{
			int reald = findamage;
			MadnessChange(mobnum, reald);
			findamage = 1;
		}
	}

	if (BreatheOn) // 숨 고르기 사용 시
	{
		findamage += BreatheValue;
	}

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

	if (Card == 0)
	{
		return false;
	}
	else if (Card == 1 || Card == 101) // 피해를 6 줍니다.(CardDamage[1])
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 2;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 2 || Card == 102) // 피해를 1 주고 출혈을 4 부여합니다.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 1;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 3 || Card == 103) // 피해를 4 줍니다. 패링시 피해가 2배가 됩니다.
	{
		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card == 3)
			{
				findamage += findamage;
			}
			else
			{
				findamage += findamage * 2;
			}

			if (findamage - Damage > 0)
			{
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 4 || Card == 104) // 카운터 시에만 사용 가능. 피해를 8 줍니다
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 4;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return false;
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			return false;
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 5 || Card == 105) // 선공:피해2, 패링:피해4 적중 시 출혈2, 카운터:피해6
	{
		if (Order == 1)
		{
			if (Card == 5)
			{
				findamage = 2;
			}
			else
			{
				findamage = 3;
			}
		}
		else if (Order == 2)
		{
			if (Card == 5)
			{
				findamage = 4;
			}
			else
			{
				findamage = 6;
			}
		}
		else if (Order == 3)
		{
			if (Card == 5)
			{
				findamage = 6;
			}
			else
			{
				findamage = 8;
			}
		}

		if (MyPlayer->BeMadness) // 광기 발현 상태이면
		{
			int reald = findamage;
			MadnessChange(mobnum, reald);
			findamage = 1;
		}

		if (BreatheOn) // 숨 고르기 사용 시
		{
			findamage += BreatheValue;
		}

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
		findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0) // 적중 시
			{
				MyPlayer->AttackStackBlooding = 2;
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 6 || Card == 106) // 선공:피해4약화2, 패링:피해6부식2, 카운터:피해6피흡
	{
		if (Order == 1)
		{
			if (Card == 6)
			{
				findamage = 4;
			}
			else
			{
				findamage = 5;
			}
		}
		else if (Order == 2)
		{
			if (Card == 6)
			{
				findamage = 6;
			}
			else
			{
				findamage = 8;
			}
		}
		else if (Order == 3)
		{
			if (Card == 6)
			{
				findamage = 6;
			}
			else
			{
				findamage = 8;
			}
		}

		if (MyPlayer->BeMadness) // 광기 발현 상태이면
		{
			int reald = findamage;
			MadnessChange(mobnum, reald);
			findamage = 1;
		}

		if (BreatheOn) // 숨 고르기 사용 시
		{
			findamage += BreatheValue;
		}

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
		findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				int kk = round(float(findamage) * 0.25f); // 회복량

				if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
				{
					kk += 5;
				}

				AForgottenKnightCharacter::MyCurrentHealth += kk;
				MyPlayer->UpdateHealth();
				MyPlayer->PlayHealEffect(kk);
				return GetDamaged(findamage);
			}
			else
			{
				int kk = round(float(findamage * 2) * 0.25f); // 회복량

				if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
				{
					kk += 5;
				}

				AForgottenKnightCharacter::MyCurrentHealth += kk;
				MyPlayer->UpdateHealth();
				MyPlayer->PlayHealEffect(kk);
				return GetDamaged(findamage * 2);
			}
			
		}
	}
	else if (Card == 7 || Card == 107) // 피해를 1 주고, 약화를 2 부여합니다.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 1;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 8 || Card == 108) // 패링 시에만 사용 가능, 피해 3 주고, 막지 못한 피해를 최대 3 감소
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 1;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return false;
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			return false;
		}
	}
	else if (Card == 9 || Card == 109) // 피해 1 준다. 이번 전투에서 피해를 받을 때마다 피해 2증가
	{
		if (Card == 9)
		{
			findamage += 2 * (MyPlayer->GameHitNum);
		}
		else
		{
			findamage += 3 * (MyPlayer->GameHitNum);
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 10 || Card == 110) // 피해를 6 줍니다. 부식,약화,출혈을 2씩 부여합니다.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 3;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 11 || Card == 111) // 서로 피해 안받음.
	{
		FString NewWString = FString::FromInt(0);
		FText InWText = FText::FromString(NewWString);
		Text_Damage->SetText(InWText);

		return false;
	}
	else if (Card == 12 || Card == 112) // 피해를 15 줍니다. 생명력 흡수
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 5;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			int kk = round(float(findamage) * 0.25f); // 회복량

			if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
			{
				kk += 5;
			}

			AForgottenKnightCharacter::MyCurrentHealth += kk;
			MyPlayer->UpdateHealth();
			MyPlayer->PlayHealEffect(kk);
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				int kk = round(float(findamage - Damage) * 0.25f); // 회복량

				if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
				{
					kk += 5;
				}

				AForgottenKnightCharacter::MyCurrentHealth += kk;
				MyPlayer->UpdateHealth();
				MyPlayer->PlayHealEffect(kk);
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				int kk = round(float(findamage) * 0.25f); // 회복량

				if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
				{
					kk += 5;
				}

				AForgottenKnightCharacter::MyCurrentHealth += kk;
				MyPlayer->UpdateHealth();
				MyPlayer->PlayHealEffect(kk);
				return GetDamaged(findamage);
			}
			else
			{
				int kk = round(float(findamage * 2) * 0.25f); // 회복량

				if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
				{
					kk += 5;
				}

				AForgottenKnightCharacter::MyCurrentHealth += kk;
				MyPlayer->UpdateHealth();
				MyPlayer->PlayHealEffect(kk);
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 13 || Card == 113) // 삼중연쇄, 피해2 , 선제공격시 다음 콤보 피해 4증가
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 1;
		}
		MyPlayer->ComboUsing_Triple = true;

		if (MyPlayer->Combo_Triple == 1)
		{
			findamage += 4;
		}
		else if (MyPlayer->Combo_Triple == 11)
		{
			findamage += 5;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			if (MyPlayer->Combo_Triple == 2)
			{
				int kk = round(float(findamage) * 0.25f); // 회복량

				if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
				{
					kk += 5;
				}

				AForgottenKnightCharacter::MyCurrentHealth += kk;
				MyPlayer->UpdateHealth();
				MyPlayer->PlayHealEffect(kk);
			}
			if (Card == 13)
			{
				MyPlayer->ComboConfirm_Triple = 1;
			}
			else
			{
				MyPlayer->ComboConfirm_Triple = 11; // 강화형태
			}
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				if (MyPlayer->Combo_Triple == 2)
				{
					int kk = round(float(findamage - Damage) * 0.25f); // 회복량

					if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
					{
						kk += 5;
					}

					AForgottenKnightCharacter::MyCurrentHealth += kk;
					MyPlayer->UpdateHealth();
					MyPlayer->PlayHealEffect(kk);
				}

				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				if (MyPlayer->Combo_Triple == 2)
				{
					int kk = round(float(findamage) * 0.25f); // 회복량

					if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
					{
						kk += 5;
					}

					AForgottenKnightCharacter::MyCurrentHealth += kk;
					MyPlayer->UpdateHealth();
					MyPlayer->PlayHealEffect(kk);
				}

				return GetDamaged(findamage);
			}
			else
			{
				if (MyPlayer->Combo_Triple == 2)
				{
					int kk = round(float(findamage * 2) * 0.25f); // 회복량

					if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
					{
						kk += 5;
					}

					AForgottenKnightCharacter::MyCurrentHealth += kk;
					MyPlayer->UpdateHealth();
					MyPlayer->PlayHealEffect(kk);
				}

				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 14 || Card == 114) // 삼중연쇄, 피해4 , 패링 시 다음 연쇄에 생흡 적용
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 2;
		}
		MyPlayer->ComboUsing_Triple = true;

		if (MyPlayer->Combo_Triple == 1)
		{
			findamage += 4;
		}
		else if (MyPlayer->Combo_Triple == 11)
		{
			findamage += 5;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			if (MyPlayer->Combo_Triple == 2)
			{
				int kk = round(float(findamage) * 0.25f); // 회복량

				if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
				{
					kk += 5;
				}

				AForgottenKnightCharacter::MyCurrentHealth += kk;
				MyPlayer->UpdateHealth();
				MyPlayer->PlayHealEffect(kk);
			}

			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				if (MyPlayer->Combo_Triple == 2)
				{
					int kk = round(float(findamage - Damage) * 0.25f); // 회복량

					if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
					{
						kk += 5;
					}

					AForgottenKnightCharacter::MyCurrentHealth += kk;
					MyPlayer->UpdateHealth();
					MyPlayer->PlayHealEffect(kk);
				}
				MyPlayer->ComboConfirm_Triple = 2;
				return GetDamaged(findamage - Damage);
			}
			else
			{
				MyPlayer->ComboConfirm_Triple = 2;
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				if (MyPlayer->Combo_Triple == 2)
				{
					int kk = round(float(findamage) * 0.25f); // 회복량

					if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
					{
						kk += 5;
					}

					AForgottenKnightCharacter::MyCurrentHealth += kk;
					MyPlayer->UpdateHealth();
					MyPlayer->PlayHealEffect(kk);
				}

				return GetDamaged(findamage);
			}
			else
			{
				if (MyPlayer->Combo_Triple == 2)
				{
					int kk = round(float(findamage * 2) * 0.25f); // 회복량

					if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
					{
						kk += 5;
					}

					AForgottenKnightCharacter::MyCurrentHealth += kk;
					MyPlayer->UpdateHealth();
					MyPlayer->PlayHealEffect(kk);
				}

				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 15 || Card == 115) // 삼중연쇄, 피해6, 카운터 시에만 사용 가능
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 2;
		}
		MyPlayer->ComboUsing_Triple = true;

		if (MyPlayer->Combo_Triple == 1)
		{
			findamage += 4;
		}
		else if (MyPlayer->Combo_Triple == 11)
		{
			findamage += 5;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			if (MyPlayer->Combo_Triple == 2)
			{
				int kk = round(float(findamage) * 0.25f); // 회복량

				if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
				{
					kk += 5;
				}

				AForgottenKnightCharacter::MyCurrentHealth += kk;
				MyPlayer->UpdateHealth();
				MyPlayer->PlayHealEffect(kk);
			}

			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				if (MyPlayer->Combo_Triple == 2)
				{
					int kk = round(float(findamage - Damage) * 0.25f); // 회복량

					if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
					{
						kk += 5;
					}

					AForgottenKnightCharacter::MyCurrentHealth += kk;
					MyPlayer->UpdateHealth();
					MyPlayer->PlayHealEffect(kk);
				}
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				if (MyPlayer->Combo_Triple == 2)
				{
					int kk = round(float(findamage) * 0.25f); // 회복량

					if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
					{
						kk += 5;
					}

					AForgottenKnightCharacter::MyCurrentHealth += kk;
					MyPlayer->UpdateHealth();
					MyPlayer->PlayHealEffect(kk);
				}

				return GetDamaged(findamage);
			}
			else
			{
				if (MyPlayer->Combo_Triple == 2)
				{
					int kk = round(float(findamage * 2) * 0.25f); // 회복량

					if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
					{
						kk += 5;
					}

					AForgottenKnightCharacter::MyCurrentHealth += kk;
					MyPlayer->UpdateHealth();
					MyPlayer->PlayHealEffect(kk);
				}

				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 16 || Card == 116) // 잽잽훅연쇄, 피해1, 다음 연쇄에 모든 적 부식 1 부여
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 1;
		}
		MyPlayer->ComboUsing_JapHook = true;

		if (MyPlayer->Combo_JapHook == 1)
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr)
				{
					Enemy->ConfirmedStackCorrosion += 1;
				}
			}
		}

		if (MyPlayer->Combo_JapHook == 2)
		{
			findamage += 6;
		}
		else if (MyPlayer->Combo_JapHook == 12)
		{
			findamage += 8;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		MyPlayer->ComboConfirm_JapHook = 1;

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 17 || Card == 117) // 잽잽훅연쇄, 피해2, 다음 연쇄 피해 6 증가
	{
		MyPlayer->ComboUsing_JapHook = true;

		if (MyPlayer->Combo_JapHook == 1)
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr)
				{
					Enemy->ConfirmedStackCorrosion += 1;
				}
			}
		}

		if (MyPlayer->Combo_JapHook == 2)
		{
			findamage += 6;
		}
		else if (MyPlayer->Combo_JapHook == 12)
		{
			findamage += 8;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Card == 16)
		{
			MyPlayer->ComboConfirm_JapHook = 2;
		}
		else
		{
			MyPlayer->ComboConfirm_JapHook = 12;
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 18 || Card == 118) // 잽잽훅연쇄, 피해4
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 2;
		}
		MyPlayer->ComboUsing_JapHook = true;

		if (MyPlayer->Combo_JapHook == 1)
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr)
				{
					Enemy->ConfirmedStackCorrosion += 1;
				}
			}
		}

		if (MyPlayer->Combo_JapHook == 2)
		{
			findamage += 6;
		}
		else if (MyPlayer->Combo_JapHook == 12)
		{
			findamage += 8;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 19 || Card == 119) // 피해2와 출혈2, 출혈 보유시 추가 출혈2
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 20 || Card == 120) // 선공:피해8, 패링:피해6, 카운터:피해4+체력3회복
	{
	if (Order == 1)
	{
		if (Card == 20)
		{
			findamage = 8;
		}
		else
		{
			findamage = 10;
		}
	}
	else if (Order == 2)
	{
		if (Card == 20)
		{
			findamage = 6;
		}
		else
		{
			findamage = 8;
		}
	}
	else if (Order == 3)
	{
		if (Card == 20)
		{
			findamage = 4;
		}
		else
		{
			findamage = 6;
		}
	}

	if (MyPlayer->BeMadness) // 광기 발현 상태이면
	{
		int reald = findamage;
		MadnessChange(mobnum, reald);
		findamage = 1;
	}

	if (BreatheOn) // 숨 고르기 사용 시
	{
		findamage += BreatheValue;
	}

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
	findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		AForgottenKnightCharacter::MyCurrentHealth += 3;
		MyPlayer->UpdateHealth();
		MyPlayer->PlayHealEffect(3);
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 21 || Card == 121) // 피해4 + 카드 1장 드로우
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		MyPlayer->BonusDrawNum += 2;
	}
	else
	{
		MyPlayer->BonusDrawNum += 1;
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 22 || Card == 122) // 피해2 + 버려진 카드 더미에서 한장 드로우
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{

	}

	MyPlayer->BonusDrawDumpDeck += 1;

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 24 || Card == 124) // 피해4와 부식2부여
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		findamage += 1;
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 31 || Card == 131) // 절멸연쇄, 피해6. 다음 연쇄 코스트 1 감소
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 2;
		}
		MyPlayer->ComboUsing_Extinction = true;

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		MyPlayer->Combo_Extinction = 1;

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 32 || Card == 132) // 연계기, 피해6, 콤보 취급
	{
		MyPlayer->ComboUsing_Triple = true;
		MyPlayer->ComboUsing_JapHook = true;
		MyPlayer->ComboUsing_Extinction = true;
		MyPlayer->ComboUsing_Madness = true;

		if (MyPlayer->Combo_Triple == 1)
		{
			findamage += 4;
		}
		else if (MyPlayer->Combo_Triple == 11)
		{
			findamage += 5;
		}

		if (MyPlayer->Combo_JapHook == 1)
		{
			for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
			{
				AEnemyCharacter* Enemy = *Iter;

				if (Enemy != nullptr)
				{
					Enemy->ConfirmedStackCorrosion += 1;
				}
			}
		}

		if (MyPlayer->Combo_JapHook == 2)
		{
			findamage += 6;
		}
		else if (MyPlayer->Combo_JapHook == 12)
		{
			findamage += 8;
		}

		if (MyPlayer->Combo_Madness == 1 || MyPlayer->Combo_Madness == 3)
		{
			findamage += 2;
		}

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			if (MyPlayer->Combo_Triple == 2)
			{
				int kk = round(float(findamage) * 0.25f); // 회복량

				if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
				{
					kk += 5;
				}

				AForgottenKnightCharacter::MyCurrentHealth += kk;
				MyPlayer->UpdateHealth();
				MyPlayer->PlayHealEffect(kk);
			}
			if (MyPlayer->ExtinctionWholeAttack) // 광역 공격을 실시하면
			{
				MyPlayer->ExtinctionWholeAttack = false;
				MyPlayer->ExtinctionWholeAttackDone = true;
			}
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (findamage - Damage > 0)
			{
				if (MyPlayer->Combo_Triple == 2)
				{
					int kk = round(float(findamage - Damage) * 0.25f); // 회복량

					if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
					{
						kk += 5;
					}

					AForgottenKnightCharacter::MyCurrentHealth += kk;
					MyPlayer->UpdateHealth();
					MyPlayer->PlayHealEffect(kk);
				}
				if (MyPlayer->ExtinctionWholeAttack) // 광역 공격을 실시하면
				{
					MyPlayer->ExtinctionWholeAttack = false;
					MyPlayer->ExtinctionWholeAttackDone = true;
				}
				return GetDamaged(findamage - Damage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (MyPlayer->ExtinctionWholeAttack) // 광역 공격을 실시하면
			{
				MyPlayer->ExtinctionWholeAttack = false;
				MyPlayer->ExtinctionWholeAttackDone = true;
			}
			if (IamBall)
			{
				if (MyPlayer->Combo_Triple == 2)
				{
					int kk = round(float(findamage) * 0.25f); // 회복량

					if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
					{
						kk += 5;
					}

					AForgottenKnightCharacter::MyCurrentHealth += kk;
					MyPlayer->UpdateHealth();
					MyPlayer->PlayHealEffect(kk);
				}
				return GetDamaged(findamage);
			}
			else
			{
				if (MyPlayer->Combo_Triple == 2)
				{
					int kk = round(float(findamage * 2) * 0.25f); // 회복량

					if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 다이아 팔찌 = 회복량 +5
					{
						kk += 5;
					}

					AForgottenKnightCharacter::MyCurrentHealth += kk;
					MyPlayer->UpdateHealth();
					MyPlayer->PlayHealEffect(kk);
				}
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 34 || Card == 134) // 선공:피해1, 패링:상대와 동일, 카운터:피해10
	{
		if (Order == 1)
		{
			if (Card == 34)
			{
				findamage = 1;
			}
			else
			{
				findamage = 2;
			}
		}
		else if (Order == 2)
		{
			findamage = 0;
		}
		else if (Order == 3)
		{
			if (Card == 34)
			{
				findamage = 10;
			}
			else
			{
				findamage = 12;
			}
		}

		if (MyPlayer->BeMadness) // 광기 발현 상태이면
		{
			int reald = findamage;
			MadnessChange(mobnum, reald);
			findamage = 1;
		}

		if (BreatheOn) // 숨 고르기 사용 시
		{
			findamage += BreatheValue;
		}

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
		findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

		if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
		{
			findamage = round(float(findamage) * mycor * 0.75f);
		}
		else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
		{
			findamage = round(float(findamage));
		}

		if (Order == 1) // 몹이 선으로 맞음
		{
			return GetDamaged(findamage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card == 34)
			{
				return false;
			}
			else
			{
				return GetDamaged(4);
			}
		}
		else if (Order == 3) // 몹이 후로 맞음.
		{
			if (Card == 34)
			{
				MyPlayer->ConfirmedStackWeakening += 2;
			}
			else
			{
				MyPlayer->ConfirmedStackWeakening += 1;
			}
			if (IamBall)
			{
				return GetDamaged(findamage);
			}
			else
			{
				return GetDamaged(findamage * 2);
			}
		}
	}
	else if (Card == 35 || Card == 135) // 잃은 체력 비례 피해
	{
	findamage = ShowLoseHP();

	if (Card > 100) // 강화 카드면 추가 효과
	{
		findamage += 10;
	}

	MyPlayer->ComboUsing_Extinction = true;

	if (BreatheOn) // 숨 고르기 사용 시
	{
		findamage += BreatheValue;
	}

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
	findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Card > 100) // 강화 카드면 추가 효과
	{
		if (findamage > 50)
		{
			findamage = 50;
		}
	}
	else
	{
		if (findamage > 30)
		{
			findamage = 30;
		}
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		if (MyPlayer->ExtinctionWholeAttack) // 광역 공격을 실시하면
		{
			MyPlayer->ExtinctionWholeAttack = false;
			MyPlayer->ExtinctionWholeAttackDone = true;
		}
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			if (MyPlayer->ExtinctionWholeAttack) // 광역 공격을 실시하면
			{
				MyPlayer->ExtinctionWholeAttack = false;
				MyPlayer->ExtinctionWholeAttackDone = true;
			}
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (MyPlayer->ExtinctionWholeAttack) // 광역 공격을 실시하면
		{
			MyPlayer->ExtinctionWholeAttack = false;
			MyPlayer->ExtinctionWholeAttackDone = true;
		}
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 36 || Card == 136) // 피해를 6 줍니다.(CardDamage[1])
	{

	int kh = findamage;

	if (Card > 100) // 강화 카드면 추가 효과
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;
			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1") && mobnum == 1)
				{
					if (Enemy->GetStackBlooding > 0)
					{
						findamage += kh + kh;
					}
					if (Enemy->GetStackWeakening > 0)
					{
						findamage += kh + kh;
					}
					if (Enemy->GetStackCorrosion > 0)
					{
						findamage += kh + kh;
					}
				}
				else if (Enemy->ActorHasTag("Enemy2") && mobnum == 2)
				{
					if (Enemy->GetStackBlooding > 0)
					{
						findamage += kh + kh;
					}
					if (Enemy->GetStackWeakening > 0)
					{
						findamage += kh + kh;
					}
					if (Enemy->GetStackCorrosion > 0)
					{
						findamage += kh + kh;
					}
				}
				else if (Enemy->ActorHasTag("Enemy3") && mobnum == 3)
				{
					if (Enemy->GetStackBlooding > 0)
					{
						findamage += kh + kh;
					}
					if (Enemy->GetStackWeakening > 0)
					{
						findamage += kh + kh;
					}
					if (Enemy->GetStackCorrosion > 0)
					{
						findamage += kh + kh;
					}
				}
			}
		}
	}
	else
	{
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;
			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1") && mobnum == 1)
				{
					if (Enemy->GetStackBlooding > 0)
					{
						findamage += kh;
					}
					if (Enemy->GetStackWeakening > 0)
					{
						findamage += kh;
					}
					if (Enemy->GetStackCorrosion > 0)
					{
						findamage += kh;
					}
				}
				else if (Enemy->ActorHasTag("Enemy2") && mobnum == 2)
				{
					if (Enemy->GetStackBlooding > 0)
					{
						findamage += kh;
					}
					if (Enemy->GetStackWeakening > 0)
					{
						findamage += kh;
					}
					if (Enemy->GetStackCorrosion > 0)
					{
						findamage += kh;
					}
				}
				else if (Enemy->ActorHasTag("Enemy3") && mobnum == 3)
				{
					if (Enemy->GetStackBlooding > 0)
					{
						findamage += kh;
					}
					if (Enemy->GetStackWeakening > 0)
					{
						findamage += kh;
					}
					if (Enemy->GetStackCorrosion > 0)
					{
						findamage += kh;
					}
				}
			}
		}
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 41 || Card == 141) // 광란1 / 피해4 출혈2, 다음 콤보 피해 +2
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		findamage += 0;
	}
	MyPlayer->ComboUsing_Madness = true;
	MyPlayer->ComboConfirm_Madness = 1;

	if (MyPlayer->Combo_Madness == 1 || MyPlayer->Combo_Madness == 3)
	{
		findamage += 2;
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 42 || Card == 142) // 광란2 / 피해2 출혈4, 다음 콤보 출혈 +2
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		findamage += 1;
	}
	MyPlayer->ComboUsing_Madness = true;
	MyPlayer->ComboConfirm_Madness = 2;

	if (MyPlayer->Combo_Madness == 1 || MyPlayer->Combo_Madness == 3)
	{
		findamage += 2;
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 43 || Card == 143) // 광란3 / 피해1 출혈2, 다음 콤보 피해+2, 출혈 +2
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		findamage += 1;
	}
	MyPlayer->ComboUsing_Madness = true;
	MyPlayer->ComboConfirm_Madness = 3;

	if (MyPlayer->Combo_Madness == 1 || MyPlayer->Combo_Madness == 3)
	{
		findamage += 2;
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 45 || Card == 145) // 피의 축제 / 피해6, 선택한 적의 출혈만큼 모든 적에게 출혈 부여
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		findamage += 3;
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 46 || Card == 146) // 내려베기 / 피해12
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		findamage += 4;
	}

	if (GetCorrosion && !EnemyWeaken) // 내가 부식되고, 상대가 약화되지 않았다면
	{
		findamage = round(float(findamage) * 1.25f); // 대미지 25퍼 증가
	}
	else if (!GetCorrosion && EnemyWeaken) // 내가 부식되지 않고, 상대가 약화되었다면
	{
		findamage = round(float(findamage) * 0.75f); // 대미지 25퍼 감소
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 47 || Card == 147) // 모아베기 / 피해16
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		findamage += 8;
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 48 || Card == 148) // 견제 / 피해3
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		findamage += 0;
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 49 || Card == 149) // 위기모면 / 피해8
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		findamage += 2;
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 50 || Card == 150) // 오의절멸2 / 피해2
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		findamage += 2;
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}
	else if (Card == 53 || Card == 153) // 상처폭발 / 적이 보유한 출혈량 만큼 피해
	{
	if (Card > 100) // 강화 카드면 추가 효과
	{
		findamage += 0;
	}

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr)
		{
			if (Enemy->ActorHasTag("Enemy1") && MyIndex == 1)
			{
				findamage += Enemy->GetStackBlooding;
			}
			else if (Enemy->ActorHasTag("Enemy2") && MyIndex == 2)
			{
				findamage += Enemy->GetStackBlooding;
			}
			else if (Enemy->ActorHasTag("Enemy3") && MyIndex == 3)
			{
				findamage += Enemy->GetStackBlooding;
			}
		}
	}

	if (GetCorrosion && EnemyWeaken) // 나(적) 부식, 상대(나) 약화
	{
		findamage = round(float(findamage) * mycor * 0.75f);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나(적) 부식, 상대(나) 약화x
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나(적) 부식x, 상대(나) 약화
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나(적) 부식x, 상대(나) 약화x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 몹이 선으로 맞음
	{
		return GetDamaged(findamage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (findamage - Damage > 0)
		{
			return GetDamaged(findamage - Damage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 몹이 후로 맞음.
	{
		if (IamBall)
		{
			return GetDamaged(findamage);
		}
		else
		{
			return GetDamaged(findamage * 2);
		}
	}
	}

	return false;
}

void UEnemyHealthBar::BreatheEnded()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
	int x = Breatheturn;
	Breatheturn = 0;
	MyController->CallAttack(x);
}

void UEnemyHealthBar::PlayHealEffect(int value)
{
	if (IamWarlock || Lichimmune)
	{
		return;
	}

	FString NewString = FString("+" + FString::FromInt(value));
	FText InText = FText::FromString(NewString);
	Text_Heal->SetText(InText);
	PlayAnimation(Anim_Heal);
}

int UEnemyHealthBar::ShowLoseHP()
{
	return MaxHealth - CurrentHealth;
}

int UEnemyHealthBar::ShowCurrentHP()
{
	return CurrentHealth;
}

void UEnemyHealthBar::CenturionFlagOn(int Flattype)
{
	if (Flattype == 0) // 붉은 깃발
	{
		CenturionBlueimmune = false;
		Image_Mob->SetBrush(ImgSkeletonCenturionRed);
		PlayAnimation(Resize_NormalSize);
	}
	else if (Flattype == 1) // 푸른 깃발
	{
		CenturionBlueimmune = true;
		Image_Mob->SetBrush(ImgSkeletonCenturionBlue);
		PlayAnimation(Resize_NormalSize);
	}
	else if (Flattype == 2) // 초록 깃발
	{
		CenturionBlueimmune = false;
		Image_Mob->SetBrush(ImgSkeletonCenturionGreen);
		PlayAnimation(Resize_NormalSize);
	}
}

void UEnemyHealthBar::LoseHP(int value)
{
	CurrentHealth -= value;
	if (CurrentHealth < 0)
	{
		CurrentHealth = 0;
	}
}

void UEnemyHealthBar::Playdieanim()
{
	PlayAnimation(Anim_Die);
}

void UEnemyHealthBar::BeholderEye(bool turnon)
{
	if (turnon)
	{
		Image_Mob->SetBrush(ImgBeholder); // 눈 뜸
		PlayAnimation(Resize_Troll);
	}
	else
	{
		Image_Mob->SetBrush(ImgBeholder2); // 눈 감음
		PlayAnimation(Resize_Troll);
	}
}
