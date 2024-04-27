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
	void DamageEffect(int Turn, int CardNum, bool Hit); // ī�� ������ ���� ����Ʈ �ٸ��� �ϱ�
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
	int AttackStackCorrosion = 0; // ��Ʈ ���� ����� ����
	int AttackStackWeakening = 0;
	int AttackStackBlooding = 0;
	int GetStackCorrosion = 0; // ���� ����� ����
	int GetStackWeakening = 0;
	int GetStackBlooding = 0;
	int ConfirmedStackCorrosion = 0; // Ȯ�� ���� ����� ����
	int ConfirmedStackWeakening = 0;
	int ConfirmedStackBlooding = 0;

	bool ConfirmedBreathe = false; // �̹� �� ������ ����
	bool InBreathe = false; // ���� ������ ������ ����

	int ConfirmedDebuffType = 0; // 1:�ν�, 2:��ȭ, 3:����
	int ConfirmedDebuffValue = 0;

	int DamageValue = 0;
	bool GolemDelay = false;
	int AsuraOrder = 1;
	FString AttackType = "";
	int AttackOrder = 0; // �̹� �� ���������� ����
	int TurnEndHeal = 0; // �� ������ �� ���Ͽ� ���� ü�� ��ȭ
	int GriffonCondition = 0;
	bool IamScent = false; // ��� �������� Ȯ��
	bool ConfirmedScent = false; // �̹� ���Ͽ� ��� ��ȭ ����
	bool ShadowAxePattern = false; // ������ ���� ���� ����
	bool AsuraRevivemode = false; // �Ƽ��� ��Ȱ ����Ͻ�
	bool WarlockAttackmode = false; // �渶 2������ ����Ͻ�
	bool WarlockAttackmodeFirst = true; // �渶 2������ ��� ù �����Ͻ�
	bool WarlockAttackCool = false; // �渶 2������ 4���� ���� ��Ÿ��
	bool IamWarlock = false;
	int IamSkeleton = 0; // ���̷��� ��Ȱ �нú� ����
	bool IamLich = false; // ��ġ �� �̿� ����
	bool Skullmode = false; // ���� �ذ�Ӹ� ��������
	bool SkulldogSpecial = false; // �ذ���� Ư����� ����
	bool LastTurnParrying = false; // ���� �Ͽ� �и� �ƴ��� ����
	int LichReviveSkill = 0;
	int AttackTryNum = 1; // ���� Ƚ��, �Ϲ����̸� 1
	int firstturnbeholder = 0;
	int FairyTurn = 0; // �� ��
	int FairyLoseHPValue = 0; // ���Ƽ 3��° �� ���� ü��
	int BaphometValue = 0; // �� ���� �����ϴ� ���ط�
	int DurahanHead = 5; // ����� �Ӹ� �߰� �����
	bool DurahanTryUp = false; // ����� �Ӹ� ����� ���� ���� ���� ����
	bool DurahanBuffUp = false; // ����� �Ӹ� ����� ���� ����
	bool DurahanParryingMode = false; // ����� ���ܷ�� �ߵ� ����
	bool TakeDurahanHead = false; // ����� �Ӹ� ���� ���� �ִ� ����
	bool GetPowerHit = false; // ��Ÿ ���� ���� ����
	bool SoonPowerHit = false; // ��Ÿ ���� ���� �غ�
	bool AllStrongBuff = false; // ��ü ���� ���� �غ�
	bool SoonLosePower = false;
	bool FlagOnBlue = false; // �ذ���δ����� Ǫ�� ����� �� ��

	int BonusDamageValue = 0; // �߰� �����

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
