// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHealthBar.generated.h"

/**
 *
 */
UCLASS()
class FORGOTTENKNIGHT_API UMyHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
		void AttackEnded();
	UFUNCTION()
		void BloodEnded();
	UFUNCTION()
		void BreatheEnded();
	UFUNCTION()
		void ParryingEnded();

	FWidgetAnimationDynamicEvent AttackDelegate;
	FWidgetAnimationDynamicEvent ParryingDelegate;
	FWidgetAnimationDynamicEvent BloodDelegate;
	FWidgetAnimationDynamicEvent BreatheDelegate;

	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* TakeBreathSound;

public:

	virtual void NativeOnInitialized() override;

	bool GetDamaged(int Damage);
	void GetBloodDamaged(int Damage);
	bool TurnEnd(int mobnum, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken);
	void GainBreathe();
	void LoseBreathe();
	void UpdateNagativeEffect(int StackCorrosion, int StackWeaken, int StackBlooding);
	void CleanNagativeEffect();
	void UpdateHealthBar();
	void DamageEffect(int Turn, int MobPattern, bool Hit, int StackCorrosion, int StackWeaken, int StackBlooding); // 몹 종류에 따라 이펙트 다르게 하기
	int ParryingWho(int mobnum, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken);
	void ParryingEffect(int Turn);
	void BloodEffect();
	void OnBreatheEffect(int Turn);
	void PlayHealEffect(int value);

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
	};

	//int StartCurrentHealth = 0;
	int turn = 0;
	int parturn = 0;
	int Breatheturn = 0;
	int totaldamage = 0;
	int BreatheValue = 2;
	bool BeDamaged = false; // 첫 대미지 체크 여부
	float mycor = 1.25f;
	float myweak = 0.75f;
	int BonusDamageValue = 0; // 추가 대미지
	int BonusDefenseValue = 0; // 피해 감소량
	int damagehalf = 0;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentHealthText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxHealthText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Damage;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_State;
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
		class UTextBlock* Text_AddEffect1;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_AddEffect2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_AddEffect3;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_AddEffect4;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_AddEffect5;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image_Corrosion;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image_Weaken;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Image_Blooding;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ParryingEffect1;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush DamageEffect1;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Attack;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Blood;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Breathe;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Parrying;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Heal;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Heal;

};
