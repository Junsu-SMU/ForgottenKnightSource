// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class FORGOTTENKNIGHT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	void ChangeWidgetImage(int CardNum, int Order);
	bool CalculateTurnEnd(int mobnum, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken);
	void ChangeDamage();
	void ChangeBreatheEffect(bool TurnOn);
	bool LifeCheck();
	void SetStat();
	void BloodingDamage();
	void DamageEffect(int Turn, int CardNum, bool Hit); // 카드 종류에 따라 이펙트 다르게 하기
	void ParryingEffect();
	void BloodEffect();
	void UpdateNagativeEffect();
	void OnBreatheEffect(int Turn);
	void GoAttack();
	void UpdateHealth(int heal);
	void Revive();
	void PlayHealEffect(int value);
	int ShowLoseHP();
	int ShowCurrentHP();
	void CleanzeDebuff();
	void LoseHP(int value);
	void SetBall(int idx); //
	void ReviveMawang();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool Life = true;
	int MyIndex = 0;
	bool GetAttackNag = false;
	int AttackStackCorrosion = 0; // 히트 관련 디버프 스택
	int AttackStackWeakening = 0;
	int AttackStackBlooding = 0;
	int GetStackCorrosion = 0; // 보유 디버프 스택
	int GetStackWeakening = 0;
	int GetStackBlooding = 0;
	int ConfirmedStackCorrosion = 0; // 확정 관련 디버프 스택
	int ConfirmedStackWeakening = 0;
	int ConfirmedStackBlooding = 0;

	bool ConfirmedBreathe = false; // 이번 턴 숨고르기 여부
	bool InBreathe = false; // 현재 숨고르지 중인지 여부

	int ConfirmedDebuffType = 0; // 1:부식, 2:약화, 3:출혈
	int ConfirmedDebuffValue = 0;

	int DamageValue = 0;
	bool GolemDelay = false;
	int AsuraOrder = 1;
	FString AttackType = "";
	int AttackOrder = 0; // 이번 턴 공격패턴의 순서
	int TurnEndHeal = 0; // 턴 끝나고 내 패턴에 의한 체력 변화
	int GriffonCondition = 0;
	bool IamScent = false; // 향기 상태인지 확인
	bool ConfirmedScent = false; // 이번 패턴에 향기 변화 여부
	bool ShadowAxePattern = false; // 숨고르기 연속 방지 변수
	bool AsuraRevivemode = false; // 아수라 부활 모드일시
	bool WarlockAttackmode = false; // 흑마 2페이즈 모드일시
	bool WarlockAttackmodeFirst = true; // 흑마 2페이즈 모드 첫 공격일시
	bool WarlockAttackCool = false; // 흑마 2페이즈 4패턴 공격 쿨타임
	bool IamWarlock = false;
	int IamSkeleton = 0; // 스켈레톤 부활 패시브 여부
	bool IamLich = false; // 리치 고립 이외 무적
	bool Skullmode = false; // 내가 해골머리 상태인지
	bool SkulldogSpecial = false; // 해골경비견 특수기믹 변수
	bool LastTurnParrying = false; // 지난 턴에 패링 됐는지 여부
	int LichReviveSkill = 0;
	int AttackTryNum = 1; // 공격 횟수, 일반적이면 1
	int firstturnbeholder = 0;
	int FairyTurn = 0; // 페어리 턴
	int FairyLoseHPValue = 0; // 페어티 3번째 턴 깎일 체력
	int BaphometValue = 0; // 턴 마다 증가하는 피해량
	int DurahanHead = 5; // 듀라한 머리 추가 대미지
	bool DurahanTryUp = false; // 듀라한 머리 대미지 증가 공격 시행 여부
	bool DurahanBuffUp = false; // 듀라한 머리 대미지 증가 여부
	bool DurahanParryingMode = false; // 듀라한 힘겨루기 발동 여부
	bool TakeDurahanHead = false; // 듀라한 머리 버프 갖고 있는 여부
	bool GetPowerHit = false; // 강타 버프 보유 여부
	bool SoonPowerHit = false; // 강타 버프 얻을 준비
	bool AllStrongBuff = false; // 전체 공업 버프 준비
	bool SoonLosePower = false;
	bool FlagOnBlue = false; // 해골백인대장이 푸른 깃발을 들 때

	int BonusDamageValue = 0; // 추가 대미지

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(VisibleAnyWhere)
	//	class UWidgetComponent* HealthWidgetComp;

	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* CounterAttackSound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* FirstStrikeSound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* ParryingSound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* SpecialAttackSound;

private:

};
