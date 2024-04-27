// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHealthBar.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"
#include "Kismet/GameplayStatics.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "Animation/WidgetAnimation.h"
#include "EnemyCharacter.h"
#include "UI_PlayWindow.h"
#include "EngineUtils.h"
#include "cmath"
#include "iostream"

void UMyHealthBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AttackDelegate.BindDynamic(this, &UMyHealthBar::AttackEnded);
	BindToAnimationFinished(Anim_Attack, AttackDelegate);
	BloodDelegate.BindDynamic(this, &UMyHealthBar::BloodEnded);
	BindToAnimationFinished(Anim_Blood, BloodDelegate);
	BreatheDelegate.BindDynamic(this, &UMyHealthBar::BreatheEnded);
	BindToAnimationFinished(Anim_Breathe, BreatheDelegate);
	ParryingDelegate.BindDynamic(this, &UMyHealthBar::ParryingEnded);
	BindToAnimationFinished(Anim_Parrying, ParryingDelegate);

	//StartCurrentHealth = AForgottenKnightCharacter::MyCurrentHealth;
	turn = 0;
	BreatheValue = floor(float(AForgottenKnightCharacter::MySheildLv - 1) * 0.5f) + 2; // 방어구 강화에 따른 숨고르기 대미지 증가량 변화
	UE_LOG(LogTemp, Log, TEXT("BreatheValue = %d"), BreatheValue);

	UpdateHealthBar();

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

bool UMyHealthBar::GetDamaged(int Damage)
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (damagehalf == MyPlayer->FindFirstAttackMob())
	{
		Damage = round(float(Damage) * 0.5f);
		damagehalf = 0;
	}

	Damage -= AForgottenKnightCharacter::MySheildLv - 1; // 방어구 뎀감 적용
	
	if (AForgottenKnightCharacter::MyAccessoryData[0] == 2) // 철 목걸이 = 뎀감1
	{
		Damage -= 1;
	}
	else if (AForgottenKnightCharacter::MyAccessoryData[0] == 4) // 철 목걸이+ = 뎀감2
	{
		Damage -= 2;
	}

	Damage -= BonusDefenseValue;

	if (Damage < 0)
	{
		Damage = 0;
	}

	if (AForgottenKnightCharacter::MyAccessoryData[0] == 5 && !BeDamaged && (Damage > 0)) // 진주 목걸이 = 첫 피해 무효화(디버프 포함)
	{
		Damage = 0;
		BeDamaged = true;

		MyPlayer->ConfirmStackClean();
	}

	AForgottenKnightCharacter::MyCurrentHealth -= Damage;

	totaldamage += Damage;

	if (AForgottenKnightCharacter::MyCurrentHealth <= 0)
	{
		//AForgottenKnightCharacter::MyCurrentHealth = StartCurrentHealth;

		MyPlayer->MyLife = false;
	}
	if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
	{
		AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
	}

	if (Damage > 0)
	{

		MyPlayer->GameHitNum += 1;
		BeDamaged = true; // 대미지 받음
		return true;
	}
	
	return false;
}

void UMyHealthBar::GetBloodDamaged(int Damage)
{
	if (AForgottenKnightCharacter::MyAccessoryData[0] == 5 && !BeDamaged && (Damage > 0)) // 진주 목걸이 = 첫 피해 무효화(디버프 포함)
	{
		Damage = 0;
		BeDamaged = true;

		AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		MyPlayer->ConfirmStackClean();
	}

	AForgottenKnightCharacter::MyCurrentHealth -= Damage;

	totaldamage += Damage;

	if (AForgottenKnightCharacter::MyCurrentHealth <= 0)
	{
		//AForgottenKnightCharacter::MyCurrentHealth = StartCurrentHealth;
		AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		MyPlayer->MyLife = false;
	}
	if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
	{
		AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
	}
}

bool UMyHealthBar::TurnEnd(int mobnum, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken)
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	damagehalf = mobnum;
	

	// 힘겨루기 모드 해결
	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;

		if (Enemy != nullptr && Enemy->ActorHasTag("Enemy1") && mobnum == 1 && Enemy->ActorHasTag("Durahan") && Enemy->DurahanParryingMode)
		{
			return GetDamaged(30);
		}
		else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy2") && mobnum == 2 && Enemy->ActorHasTag("Durahan") && Enemy->DurahanParryingMode)
		{
			return GetDamaged(30);
		}
		else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy3") && mobnum == 3 && Enemy->ActorHasTag("Durahan") && Enemy->DurahanParryingMode)
		{
			return GetDamaged(30);
		}
	}
	

	if (GetCorrosion && EnemyWeaken) // 나 부식, 상대 약화
	{
		Damage = round(float(Damage) * 1.25f * myweak);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나 부식, 상대 약화x
	{
		Damage = round(float(Damage) * 1.25f);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나 부식x, 상대 약화
	{
		Damage = round(float(Damage) * myweak);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나 부식x, 상대 약화x
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

	findamage += BonusDamageValue;

	if (Card == 0)
	{
		return GetDamaged(Damage);
	}
	else if (Card == 1 || Card == 101) // 피해를 6 줍니다.
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 2;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 2 || Card == 102) // 피해를 1 주고 출혈을 4 부여합니다.
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 1;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 3 || Card == 103) // 피해를 4 줍니다. 패링시 피해가 2배가 됩니다.
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += findamage * 2; // 패링 대미지 3배
			}
			else
			{
				findamage += findamage; // 패링 대미지 2배
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 4 || Card == 104) // 카운터 시에만 사용 가능. 피해를 8 줍니다
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 4;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			findamage += findamage; // 패링 대미지 2배

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 5 || Card == 105) // 선공:피해2, 패링:피해4,출혈2, 카운터:피해6
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
			findamage = 1;
		}

		if (BreatheOn) // 숨 고르기 사용 시
		{
			findamage += BreatheValue;
		}
		findamage += 2*(AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

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

		findamage += BonusDamageValue;

		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 6 || Card == 106) // 선공:피해4,약화2, 패링:피해6,부식2, 카운터:피해6,생흡
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
			findamage = 1;
		}

		if (BreatheOn) // 숨 고르기 사용 시
		{
			findamage += BreatheValue;
		}
		findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

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

		findamage += BonusDamageValue;

		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 7 || Card == 107) // 피해1 약화2
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 1;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 8 || Card == 108) // 패링 시에만 사용가능. 피해를 3 주고, 막지못한 피해 3 감소
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 1;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Card == 8)
			{
				if (Damage - (findamage + 3) > 0) // 피해 최대 3 막아줌
				{
					return GetDamaged(Damage - (findamage + 3));
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (Damage - (findamage + 4) > 0) // 피해 최대 4 막아줌
				{
					return GetDamaged(Damage - (findamage + 4));
				}
				else
				{
					return false;
				}
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 9 || Card == 109) // 피해 1 주기, 전투에서 피해 받을 때마다 2 증가
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card == 9)
			{
				findamage += 2 * (MyPlayer->GameHitNum);
			}
			else
			{
				findamage += 3 * (MyPlayer->GameHitNum);
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 10 || Card == 110) // 피해 6 주기, 부식약화출혈 2씩 부여
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 3;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 11 || Card == 111) // 서로 아무것도 안하기
	{
		FString NewWString = FString::FromInt(0);
		FText InWText = FText::FromString(NewWString);
		Text_Damage->SetText(InWText);

		return false;
	}
	else if (Card == 12 || Card == 112) // 피해 15
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 5;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 13 || Card == 113) // 삼중연쇄, 피해2
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 14 || Card == 114) // 삼중연쇄, 피해4
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 15 || Card == 115) // 삼중연쇄, 피해6 카운터 시에만 사용 가능
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 16 || Card == 116) // 잽잽훅, 피해1
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 1;
			}
			MyPlayer->ComboUsing_JapHook = true;

			if (MyPlayer->Combo_JapHook == 2)
			{
				findamage += 6;
			}
			else if (MyPlayer->Combo_JapHook == 12)
			{
				findamage += 8;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 17 || Card == 117) // 잽잽훅, 피해2
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			MyPlayer->ComboUsing_JapHook = true;

			if (MyPlayer->Combo_JapHook == 2)
			{
				findamage += 6;
			}
			else if (MyPlayer->Combo_JapHook == 12)
			{
				findamage += 8;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 18 || Card == 118) // 잽잽훅, 피해4
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 2;
			}
			MyPlayer->ComboUsing_JapHook = true;

			if (MyPlayer->Combo_JapHook == 2)
			{
				findamage += 6;
			}
			else if (MyPlayer->Combo_JapHook == 12)
			{
				findamage += 8;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 19 || Card == 119) // 피해를 2 주고, 출혈을 2 부여합니다. 대상이 출혈 보유 시 추가로 출혈을 2 부여합니다.
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 20 || Card == 120) // 선공:피해8, 패링:피해6, 카운터:피해4+체력회복3
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

	findamage += BonusDamageValue;

	findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 21 || Card == 121) // 피해를 4 주고 카드를 1장 뽑습니다
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			MyPlayer->BonusDrawNum += 2;
		}
		else
		{
			MyPlayer->BonusDrawNum += 1;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 22 || Card == 122) // 피해를 2 주고 버려진 카드더미 1장 가져오기
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{

		}

		MyPlayer->BonusDrawDumpDeck += 1;

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 24 || Card == 124) // 피해를 4 주고 부식을 2 부여
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 1;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 31 || Card == 131) // 오의절멸, 피해6
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 2;
			}
			MyPlayer->ComboUsing_Extinction = true;

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 32 || Card == 132) // 연계기
	{
		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			MyPlayer->ComboUsing_Triple = true;
			MyPlayer->ComboUsing_JapHook = true;
			MyPlayer->ComboUsing_Extinction = true;
			MyPlayer->ComboUsing_Madness = true;

			if (MyPlayer->Combo_Madness == 1 || MyPlayer->Combo_Madness == 3)
			{
				findamage += 2;
			}

			if (MyPlayer->Combo_Triple == 1)
			{
				findamage += 4;
			}
			else if (MyPlayer->Combo_Triple == 11)
			{
				findamage += 5;
			}

			if (MyPlayer->Combo_JapHook == 2)
			{
				findamage += 6;
			}
			else if (MyPlayer->Combo_JapHook == 12)
			{
				findamage += 8;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 34 || Card == 134) // 선공:피해1, 패링:동일 피해, 카운터:피해10
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
			findamage = 0; // 상대와 동일or+4
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

		findamage += BonusDamageValue;

		findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			return false;
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 35 || Card == 135) // 적의 잃은 체력만큼 피해
	{
		findamage = 0;

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;
			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1") && mobnum == 1)
				{
					findamage = Enemy->ShowLoseHP();
				}
				else if (Enemy->ActorHasTag("Enemy2") && mobnum == 2)
				{
					findamage = Enemy->ShowLoseHP();
				}
				else if (Enemy->ActorHasTag("Enemy3") && mobnum == 3)
				{
					findamage = Enemy->ShowLoseHP();
				}
			}
		}

		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 10;
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

		findamage += BonusDamageValue;

		findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
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

		if (Order == 1) // 내가 후로 맞음
		{
			return GetDamaged(Damage);
		}
		else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Damage - findamage > 0)
			{
				return GetDamaged(Damage - findamage);
			}
			else
			{
				return false;
			}
		}
		else if (Order == 3) // 내가 선으로 맞음
		{
			return GetDamaged(Damage);
		}
	}
	else if (Card == 36 || Card == 136) // 대상이 보유한 상태이상에 비례해 피해 배 증가
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

	if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
	{
		findamage = round(float(findamage) * 0.75f * mycor);
	}
	else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
	{
		findamage = round(float(findamage) * 0.75f);
	}
	else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
	{
		findamage = round(float(findamage) * mycor);
	}
	else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
	{
		findamage = round(float(findamage));
	}

	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3)
	{
		return GetDamaged(Damage);
	}

	}
	else if (Card == 41 || Card == 141) // 광란1 / 피해4 출혈2, 다음 콤보 피해 +2
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 42 || Card == 142) // 광란2 / 피해2 출혈4, 다음 콤보 출혈 +2
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 43 || Card == 143) // 광란3 / 피해1 출혈2, 다음 콤보 피해+2, 출혈 +2
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 45 || Card == 145) // 피의 축제 / 피해6, 선택한 적의 출혈만큼 모든 적에게 출혈 부여
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 3;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 46 || Card == 146) // 내려베기 / 피해12
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 4;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 47 || Card == 147) // 모아베기 / 피해16
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 8;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 48 || Card == 148) // 견제 / 피해3
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 0;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 49 || Card == 149) // 위기모면 / 피해8
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 2;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 50 || Card == 150) // 오의절멸2 / 피해2
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 2;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	else if (Card == 53 || Card == 153) // 상처폭발 / 적이 보유한 출혈량 만큼 피해
	{
	if (Order == 1) // 내가 후로 맞음
	{
		return GetDamaged(Damage);
	}
	else if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 0;
		}

		int x = 0;

		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				if (MyWindow->MobDone[1] == 2)
				{
					x = 1;
				}
				else if (MyWindow->MobDone[2] == 2)
				{
					x = 2;
				}
				else if (MyWindow->MobDone[3] == 2)
				{
					x = 3;
				}
			}
		}

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1") && x == 1)
				{
					findamage += Enemy->GetStackBlooding;
				}
				else if (Enemy->ActorHasTag("Enemy2") && x == 2)
				{
					findamage += Enemy->GetStackBlooding;
				}
				else if (Enemy->ActorHasTag("Enemy3") && x == 3)
				{
					findamage += Enemy->GetStackBlooding;
				}
			}
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return GetDamaged(Damage - findamage);
		}
		else
		{
			return false;
		}
	}
	else if (Order == 3) // 내가 선으로 맞음
	{
		return GetDamaged(Damage);
	}
	}
	


	return false;
}

void UMyHealthBar::GainBreathe()
{
	Image_State->SetVisibility(ESlateVisibility::Visible);
}

void UMyHealthBar::LoseBreathe()
{
	Image_State->SetVisibility(ESlateVisibility::Hidden);
}

void UMyHealthBar::UpdateNagativeEffect(int StackCorrosion, int StackWeaken, int StackBlooding)
{
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
			if (StackCorrosion > 0)
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
			if (StackWeaken > 0 && xArray[0] != 2)
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
			if (StackBlooding > 0 && xArray[0] != 3 && xArray[1] != 3)
			{
				Image_AddEffect2->SetVisibility(ESlateVisibility::Visible);
				Image_AddEffect2->SetBrush(Image_Blooding);
				FString NewString = FString::FromInt(StackBlooding);
				FText InText = FText::FromString(NewString);
				Text_AddEffect2->SetText(InText);
				xArray[2] = 3;
			}
		}
		else if (Image_AddEffect4->GetVisibility() == ESlateVisibility::Collapsed)
		{
			
		}
		else if (Image_AddEffect5->GetVisibility() == ESlateVisibility::Collapsed)
		{
			
		}
	}
}

void UMyHealthBar::CleanNagativeEffect()
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

void UMyHealthBar::UpdateHealthBar()
{
	if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
	{
		AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
	}
	else if (AForgottenKnightCharacter::MyCurrentHealth < 0)
	{
		AForgottenKnightCharacter::MyCurrentHealth = 0;
	}

	FString NewWString = FString::FromInt(AForgottenKnightCharacter::MyCurrentHealth);
	FText InWText = FText::FromString(NewWString);
	CurrentHealthText->SetText(InWText);

	NewWString = FString::FromInt(AForgottenKnightCharacter::MyMaxHealth);
	InWText = FText::FromString(NewWString);
	MaxHealthText->SetText(InWText);

	float PerW = float(AForgottenKnightCharacter::MyCurrentHealth) / float(AForgottenKnightCharacter::MyMaxHealth);
	HealthBar->SetPercent(PerW);
}

void UMyHealthBar::DamageEffect(int Turn, int MobPattern, bool Hit, int StackCorrosion, int StackWeaken, int StackBlooding)
{
	if (Turn > 0)
	{
		turn = Turn;
	}

	FString NewWString = FString::FromInt(totaldamage);
	FText InWText = FText::FromString(NewWString);
	Text_Damage->SetText(InWText);

	PlayAnimation(Anim_Attack); // 현재는 몹 패턴 상관없이 해당 애니 실행

	FString NewString = FString::FromInt(AForgottenKnightCharacter::MyCurrentHealth);
	FText InText = FText::FromString(NewString);
	CurrentHealthText->SetText(InText);
	float Per = float(AForgottenKnightCharacter::MyCurrentHealth) / float(AForgottenKnightCharacter::MyMaxHealth);
	HealthBar->SetPercent(Per);

	if (Hit)
	{
		UpdateNagativeEffect(StackCorrosion, StackWeaken, StackBlooding);
	}
}

int UMyHealthBar::ParryingWho(int mobnum, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken) // 0 - 내가 맞음, 1 - 동타, 2 - 적이 맞음
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (GetCorrosion && EnemyWeaken) // 나 부식, 상대 약화
	{
		Damage = round(float(Damage) * 1.25f * myweak);
	}
	else if (GetCorrosion && !EnemyWeaken) // 나 부식, 상대 약화x
	{
		Damage = round(float(Damage) * 1.25f);
	}
	else if (!GetCorrosion && EnemyWeaken) // 나 부식x, 상대 약화
	{
		Damage = round(float(Damage) * myweak);
	}
	else if (!GetCorrosion && !EnemyWeaken) // 나 부식x, 상대 약화x
	{
		Damage = round(float(Damage));
	}

	int findamage = 0;
	if (Card < 100) // 일반 카드
	{ // 카드 대미지 + 무기 레벨에 따른 추가 대미지
		findamage = CardDamage[Card] + 2 * (AForgottenKnightCharacter::MySwordLv - 1);
	}
	else // 강화 카드
	{
		findamage = CardDamage[Card - 100] + 2 * (AForgottenKnightCharacter::MySwordLv - 1);
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

	findamage += BonusDamageValue;

	if (Card == 0)
	{
		return 0;
	}
	else if (Card == 1 || Card == 101) // 피해를 6 줍니다.
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 2;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 2 || Card == 102) // 피해를 1 주고 출혈을 4 부여합니다.
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 1;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 3 || Card == 103) // 피해를 4 줍니다. 패링시 피해가 2배가 됩니다.
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card == 3)
			{
				findamage += findamage;
			}
			else
			{
				findamage += findamage * 2;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 4 || Card == 104) // 카운터 시에만 사용 가능. 피해를 8 줍니다
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 4;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 5 || Card == 105) // 선공:피해2, 패링:피해4,출혈2, 카운터:피해6
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card == 5)
			{
				findamage = 4;
			}
			else
			{
				findamage = 6;
			}

			if (MyPlayer->BeMadness) // 광기 발현 상태이면
			{
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
			findamage += BonusDamageValue;
			findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 6 || Card == 106) // 선공:피해4,약화2, 패링:피해6,부식2, 카운터:피해6,생흡
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card == 6)
			{
				findamage = 6;
			}
			else
			{
				findamage = 8;
			}

			if (MyPlayer->BeMadness) // 광기 발현 상태이면
			{
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
			findamage += BonusDamageValue;
			findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 7 || Card == 107) // 피해1 약화2
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 1;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 8 || Card == 108) // 패링 시에만 사용가능. 피해를 3 주고, 막지못한 피해 3 감소
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 1;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Card == 8)
			{
				if (Damage - (findamage + 3) > 0) // 피해 최대 3 막아줌
				{
					return 0;
				}
				else if (Damage == (findamage + 3))
				{
					return 1;
				}
				else
				{
					return 2;
				}
			}
			else
			{
				if (Damage - (findamage + 4) > 0) // 피해 최대 4 막아줌
				{
					return 0;
				}
				else if(Damage == (findamage + 4))
				{
					return 1;
				}
				else
				{
					return 2;
				}
			}
		}
	}
	else if (Card == 9 || Card == 109) // 피해를 1 줍니다. 피해 받을 때마다 카드 피해량 2 증가
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card == 9)
			{
				findamage += 2 * (MyPlayer->GameHitNum);
			}
			else
			{
				findamage += 3 * (MyPlayer->GameHitNum);
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 10 || Card == 110) // 피해를 6 줍니다. 부식 약화 출혈을 2 씩 부여
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 3;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 11 || Card == 111) // 서로 아무것도 안함.
	{
		return 1;
	}
	else if (Card == 12 || Card == 112) // 피해를 12 줌
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 5;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 13 || Card == 113) // 삼중연쇄, 피해2
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 14 || Card == 114) // 삼중연쇄, 피해4
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 15 || Card == 115) // 삼중연쇄, 피해6 카운터 시에만 사용 가능
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 16 || Card == 116) // 잽잽훅, 피해1
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 1;
			}
			MyPlayer->ComboUsing_JapHook = true;

			if (MyPlayer->Combo_JapHook == 2)
			{
				findamage += 6;
			}
			else if (MyPlayer->Combo_JapHook == 12)
			{
				findamage += 8;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 17 || Card == 117) // 잽잽훅, 피해2
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			MyPlayer->ComboUsing_JapHook = true;

			if (MyPlayer->Combo_JapHook == 2)
			{
				findamage += 6;
			}
			else if (MyPlayer->Combo_JapHook == 12)
			{
				findamage += 8;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 18 || Card == 118) // 잽잽훅, 피해4
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 2;
			}
			MyPlayer->ComboUsing_JapHook = true;

			if (MyPlayer->Combo_JapHook == 2)
			{
				findamage += 6;
			}
			else if (MyPlayer->Combo_JapHook == 12)
			{
				findamage += 8;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 19 || Card == 119) // 피해를 2 주고, 출혈을 2 부여합니다. 대상이 출혈 보유 시 추가로 출혈을 2 부여합니다.
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{

		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 20 || Card == 120) // 선공:피해8, 패링:피해6, 카운터:피해4+체력회복3
	{
	if (Order == 2)
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

	if (MyPlayer->BeMadness) // 광기 발현 상태이면
	{
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

	findamage += BonusDamageValue;

	findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 21 || Card == 121) // 피해를 4 주고 카드를 1장 뽑습니다
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{

		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				MyPlayer->BonusDrawNum += 2;
			}
			else
			{
				MyPlayer->BonusDrawNum += 1;
			}

			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 22 || Card == 122) // 피해를 2 주고 버려진 카드더미 1장 가져오기
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{

		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			MyPlayer->BonusDrawDumpDeck += 1;

			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 24 || Card == 124) // 피해를 4 주고 부식을 2 부여
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 1;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 31 || Card == 131) // 오의절멸, 피해6
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card > 100) // 강화 카드면 추가 효과
			{
				findamage += 2;
			}
			MyPlayer->ComboUsing_Extinction = true;

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 32 || Card == 132) // 연계기
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			MyPlayer->ComboUsing_Triple = true;
			MyPlayer->ComboUsing_JapHook = true;
			MyPlayer->ComboUsing_Extinction = true;
			MyPlayer->ComboUsing_Madness = true;

			if (MyPlayer->Combo_Madness == 1 || MyPlayer->Combo_Madness == 3)
			{
				findamage += 2;
			}

			if (MyPlayer->Combo_Triple == 1)
			{
				findamage += 4;
			}
			else if (MyPlayer->Combo_Triple == 11)
			{
				findamage += 5;
			}

			if (MyPlayer->Combo_JapHook == 2)
			{
				findamage += 6;
			}
			else if (MyPlayer->Combo_JapHook == 12)
			{
				findamage += 8;
			}

			if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
			{
				findamage = round(float(findamage) * 0.75f * mycor);
			}
			else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
			{
				findamage = round(float(findamage) * 0.75f);
			}
			else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
			{
				findamage = round(float(findamage) * mycor);
			}
			else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
			{
				findamage = round(float(findamage));
			}

			if (Damage - findamage > 0)
			{
				return 0;
			}
			else if (Damage == findamage)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	else if (Card == 34 || Card == 134) // 선공:피해1, 패링:동일 피해, 카운터:피해10
	{
		if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
		{
			if (Card == 34)
			{
				return 1; // 서로 패링
			}
			else
			{
				return 2; // 내가 4뎀 넣기
			}
		}
	}
	else if (Card == 35 || Card == 135) // 적의 잃은 체력만큼 피해
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		findamage = 0;

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;
			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1") && mobnum == 1)
				{
					findamage = Enemy->ShowLoseHP();
				}
				else if (Enemy->ActorHasTag("Enemy2") && mobnum == 2)
				{
					findamage = Enemy->ShowLoseHP();
				}
				else if (Enemy->ActorHasTag("Enemy3") && mobnum == 3)
				{
					findamage = Enemy->ShowLoseHP();
				}
			}
		}

		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 10;
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

		findamage += BonusDamageValue;

		findamage += 2 * (AForgottenKnightCharacter::MySwordLv - 1); // 무기 댐도 더하기

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
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

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 36 || Card == 136) // 대상이 보유한 상태이상에 비례해 피해 배 증가
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 41 || Card == 141) // 광란1 / 피해4 출혈2, 다음 콤보 피해 +2
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 42 || Card == 142) // 광란2 / 피해2 출혈4, 다음 콤보 출혈 +2
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 43 || Card == 143) // 광란3 / 피해1 출혈2, 다음 콤보 피해+2, 출혈 +2
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
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

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 45 || Card == 145) // 피의 축제 / 피해6, 선택한 적의 출혈만큼 모든 적에게 출혈 부여
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 3;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 46 || Card == 146) // 내려베기 / 피해12
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 4;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 47 || Card == 147) // 모아베기 / 피해16
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 8;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 48 || Card == 148) // 견제 / 피해3
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 0;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 49 || Card == 149) // 위기모면 / 피해8
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 2;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 50 || Card == 150) // 오의절멸2 / 피해2
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 2;
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
	else if (Card == 53 || Card == 153) // 상처폭발 / 적이 보유한 출혈량 만큼 피해
	{
	if (Order == 2) // 패링, 높은 쪽의 여분 대미지만 받음.
	{
		if (Card > 100) // 강화 카드면 추가 효과
		{
			findamage += 0;
		}

		int x = 0;

		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				if (MyWindow->MobDone[1] == 2)
				{
					x = 1;
				}
				else if (MyWindow->MobDone[2] == 2)
				{
					x = 2;
				}
				else if (MyWindow->MobDone[3] == 2)
				{
					x = 3;
				}
			}
		}

		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr)
			{
				if (Enemy->ActorHasTag("Enemy1") && x == 1)
				{
					findamage += Enemy->GetStackBlooding;
				}
				else if (Enemy->ActorHasTag("Enemy2") && x == 2)
				{
					findamage += Enemy->GetStackBlooding;
				}
				else if (Enemy->ActorHasTag("Enemy3") && x == 3)
				{
					findamage += Enemy->GetStackBlooding;
				}
			}
		}

		if (GetWeaken && EnemyCorrosion) // 나 약화, 상대 부식
		{
			findamage = round(float(findamage) * 0.75f * mycor);
		}
		else if (GetWeaken && !EnemyCorrosion) // 나 약화, 상대 부식x
		{
			findamage = round(float(findamage) * 0.75f);
		}
		else if (!GetWeaken && EnemyCorrosion) // 나 약화x, 상대 부식
		{
			findamage = round(float(findamage) * mycor);
		}
		else if (!GetWeaken && !EnemyCorrosion) // 나 약화x, 상대 부식x
		{
			findamage = round(float(findamage));
		}

		if (Damage - findamage > 0)
		{
			return 0;
		}
		else if (Damage == findamage)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}

	return 0;
}

void UMyHealthBar::ParryingEffect(int Turn)
{
	if (Turn > 0)
	{
		parturn = Turn;
	}

	PlayAnimation(Anim_Parrying);
}

void UMyHealthBar::BloodEffect()
{
	PlayAnimation(Anim_Blood);

	FString NewString = FString::FromInt(AForgottenKnightCharacter::MyCurrentHealth);
	FText InText = FText::FromString(NewString);
	CurrentHealthText->SetText(InText);
	float Per = float(AForgottenKnightCharacter::MyCurrentHealth) / float(AForgottenKnightCharacter::MyMaxHealth);
	HealthBar->SetPercent(Per);

}

void UMyHealthBar::OnBreatheEffect(int Turn)
{
	Breatheturn = Turn;

	if (TakeBreathSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, TakeBreathSound);
	}
	GainBreathe();
	PlayAnimation(Anim_Breathe);
}

void UMyHealthBar::PlayHealEffect(int value)
{
	FString NewString = FString("+" + FString::FromInt(value));
	FText InText = FText::FromString(NewString);
	Text_Heal->SetText(InText);
	PlayAnimation(Anim_Heal);
}

void UMyHealthBar::BloodEnded()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	MyPlayer->AllBloodEnd -= 1;
}

void UMyHealthBar::AttackEnded()
{
	totaldamage = 0;
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
	int x = turn;
	turn = 0;
	MyController->CallAttack(x);
}

void UMyHealthBar::ParryingEnded()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
	int x = parturn;
	parturn = 0;
	MyController->CallAttack(x);
}

void UMyHealthBar::BreatheEnded()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
	int x = Breatheturn;
	Breatheturn = 0;
	MyController->CallAttack(x);
}
