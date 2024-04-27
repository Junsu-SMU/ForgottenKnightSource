// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBar.generated.h"

/**
 *
 */
UCLASS()
class FORGOTTENKNIGHT_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
		void AttackEnded();
	UFUNCTION()
		void BloodEnded();
	UFUNCTION()
		void BreatheEnded();

	FWidgetAnimationDynamicEvent AttackDelegate;
	FWidgetAnimationDynamicEvent BloodDelegate;
	FWidgetAnimationDynamicEvent BreatheDelegate;

	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* CounterAttackSound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* FirstStrikeSound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* ParryingSound;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	virtual void NativeOnInitialized() override;

	void ConnectImage(int CardNum, int Order);
	bool GetDamaged(int Damage);
	void GetBloodDamaged(int Damage);
	bool TurnEnd(int mobnum, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken);
	void ChangeBreatheEffect(bool TurnOn);
	void StatSet(int Health);
	void StathalfSet(int Health);
	void UpdateNagativeEffect(int StackCorrosion, int StackWeaken, int StackBlooding);
	void CleanNagativeEffect();
	void DamageEffect(int Turn, int CardNum, bool Hit, int StackCorrosion, int StackWeaken, int StackBlooding); // 카드 종류에 따라 이펙트 다르게 하기
	void CallMyIndex(int MyOrder);
	void ParryingEffect();
	void BloodEffect();
	void OnBreatheEffect(int Turn);
	void GoAttack();
	void UpdateHealthBar(int heal);
	void Revive();
	void PlayHealEffect(int value);
	int ShowLoseHP();
	int ShowCurrentHP();
	void CenturionFlagOn(int Flattype);
	void LoseHP(int value);
	void Playdieanim();
	void BeholderEye(bool turnon);

	void AttackSet(int Damage); // enemypattern위젯에 있던 부분
	void GainNagative();
	void LoseNagative();
	void GainPositive();
	void LosePositive();
	void SetMobImage(FString MobName); // enemyimage위젯에 있던 부분
	void MadnessChange(int MobNum, int Damage);
	//void SetAttackSpeed(int Speed);

	int MaxHealth = 1;
	int CurrentHealth = 1;
	int turn = 0;
	int	Breatheturn = 0;
	int MyIndex = 0; // 내 위치
	int BreatheValue = 2;
	float mycor = 1.25f;
	float myweak = 0.75f;
	bool GetAttakedWhole = false;
	int BonusDefenseValue = 0; // 피해 감소량
	int Attackorderthisturn = 0; // 당장 공격 턴

	bool IamSpider = false;
	bool IamShadowShadowSheild = false;
	bool IamAsura = false;
	bool IamAsuraup = false;
	bool IamWarlock = false;
	bool IamWarlock2Phase = false;

	int IamSkeleton = 0; // 스켈레톤 부활 패시브 여부
	bool IamLich = false; // 리치 고립 이외 무적
	bool Lichimmune = false; // 리치 무적 상태 여부
	bool Skullmode = false; // 내가 해골머리 상태인지
	int SkullReviveCool = 0;
	int MawangReviveCool = 0;
	int MawangReviveTime = 3;

	bool DebuffImmune = false; // 디버프 면역 여부
	bool NoScan = false; // 패턴 보이는 여부
	bool IamBall = false; // 왕 2페이즈일 경우
	int IamDemonLord = 0; // 왕 3페이즈
	bool CenturionBlueimmune = false; // 해골백인대장 블루 면역
	bool IamCenturion = false;

	TArray<int32> CardDamage =
	{ 0, 6, 1, 4, 8 ,0
		,0, 1, 3, 1, 6	//10
		,0 ,15, 2, 4, 6
		,1, 2, 4, 2, 0	//20
		,4, 2, 0, 4, 3
		,3, 3, 0, 0, 0	//30
		,6, 6, 0, 0, 0
		,1, 0, 0, 0, 0	//40
		,4, 2, 1, 0, 6
		,12, 16, 3, 8, 2	//50
		,0, 0, 0, 0, 0
		,0, 0, 0, 0, 0	//60
		,0, 0, 0, 0, 0
		,0, 0, 0, 0, 0	//70
		,0, 0, 0, 0, 0
		,0, 0, 0, 0, 0	//80
		,0, 0, 0, 0, 0
		,0, 0, 0, 0, 0	//90
		,0, 0, 0, 0, 0
		,0, 0, 0, 0, 0  //100
	};

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DamageText;
	UPROPERTY(meta = (BindWidget))
		class UCircularThrobber* NagativeEffect;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Mob;
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* Text_MobName;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSlime; // 슬라임
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgGoblin;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgTroll;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSpider;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSpider2;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSpider3;
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
		struct FSlateBrush ImgWarlock2Phase;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSkullHead;
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
		struct FSlateBrush ImgBeholder2;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgFallenFairy;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBaphomet;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgDurahan;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSkeletonCenturion;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSkeletonCenturionRed;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSkeletonCenturionBlue;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSkeletonCenturionGreen;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgDemonLordHead;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgDemonLordLeft;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgDemonLordRight;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBall1; // 화염구
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBall2; // 얼음구
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBall3; // 저주구
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBall4; // 산성구
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBall5; // 혈구
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBall6; // 강철구
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBall7; // 전기구


	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentHealthText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxHealthText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* OrderText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Damage;
	UPROPERTY(meta = (BindWidget))
		class UImage* InImage;
	UPROPERTY(meta = (BindWidget))
		class UImage* InImage_Background;
	UPROPERTY(meta = (BindWidget))
		class UCircularThrobber* BreatheEffect;
	UPROPERTY(meta = (BindWidget))
		class UCircularThrobber* PositiveEffect;
	UPROPERTY(meta = (BindWidget))
		class UCircularThrobber* LifeStealEffect;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Immune;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_AddEffect1;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_AddEffect2;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_AddEffect3;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_AddEffect4;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_AddEffect5;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Effect;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_CardAttack;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_CardEffect1;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_CardEffect2;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_CardEffect3;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_AddEffect1;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_AddEffect2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_AddEffect3;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_AddEffect4;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_AddEffect5;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_CardAttack;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_CardEffect1;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_CardEffect2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_CardEffect3;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_PopupPassive;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_PopupPassive;

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image1; // 공격
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image2; // 상처내기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image3; // 받아치기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image4; // 되돌려주기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image5; // 천변의검
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image6; // 다재다능
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image7; // 힘줄끊기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image8; // 흘리기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image9; // 감내
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image10; // 진흙탕싸움
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image11; // 철괴
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image12; // 피의맛
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image13; // 삼중연쇄1
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image14; // 삼중연쇄2
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image15; // 삼중연쇄3
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image16; // 잽잽훅1
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image17; // 잽잽훅2
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image18; // 잽잽훅3

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image19; // 상처 벌리기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image20; // 유연한 강함
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image21; // 준비만전
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image22; // 땅 파기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image23; // 대기만성
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image24; // 갑옷 부수기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image25; // 삼위일체1
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image26; // 삼위일체2
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image27; // 삼위일체3

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image31; // 오의절멸1
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image32; // 연계기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image33; // 명상
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image34; // 적절한 분배

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image35; // 오의절멸4
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image36; // 더티 파이트
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image37; // 무소유

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image41; // 광란1
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image42; // 광란2
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image43; // 광란3
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image44; // 날 세우기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image45; // 피의 축제
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image46; // 내려베기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image47; // 모아베기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image48; // 견제
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image49; // 위기 모면

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image50; // 오의 절멸2
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image51; // 오의 절멸3
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image52; // 전투 휴식
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image53; // 상처 폭발
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image54; // 각성

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image81; // 착란
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image82; // 기본기 연마
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image83; // 광기 발현
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image84; // 연격의 대가


	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image_Corrosion;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image_Weaken;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image_Blooding;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image_SpecialPassive;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ParryingEffect1;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush DamageEffect1;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Attack;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_GoAttack;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Die;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Revive;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Blood;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Breathe;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Resize_Goblin;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Resize_Mantis;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Resize_Troll;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Resize_Asura;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Resize_Golem;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Resize_NormalSize;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Heal;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Heal;
};
