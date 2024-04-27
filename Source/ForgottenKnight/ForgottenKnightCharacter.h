// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ForgottenKnightCharacter.generated.h"

UCLASS(config = Game)
class AForgottenKnightCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

public:
	AForgottenKnightCharacter();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	bool CalculateTurnEnd(int mobnum, int DebuffType, int DebuffValue, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken);
	bool LifeCheck();
	void RemoveHealthBar();
	//void GainBreathe();
	//void LoseBreathe();
	void InGameCamera();
	void OutGameCamera();
	void UpdateHealth();
	//void BloodingDamage();
	void DamageEffect(int Turn, int MobPattern, bool Hit); // 몹 공격에 따라 이펙트 다르게 하기
	int ParryingWho(int mobnum, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken); // 패링 시 누구한테 대미지가 갈지 true = 적, false = 나
	void ParryingEffect(int Turn);
	//void BloodEffect();
	void UpdateNagativeEffect();
	void SaveData(int SlotIndex);
	void LoadData(int SlotIndex);
	void LoadNewGame();
	//void OnBreatheEffect(int Turn);
	void WalkSoundBase(bool Switching);
	void PlayHealEffect(int value);
	void ConfirmStackClean();
	int ShowBonusAttack();
	void DebuffCleanze();
	int FindFirstAttackMob();

	bool Savable = true;
	bool Movable = true;
	bool MyLife = true;

	int GetStackCorrosion = 0; // 보유 디버프 스택
	int GetStackWeakening = 0;
	int GetStackBlooding = 0;
	int AttackStackCorrosion = 0; // 히트 관련 디버프 스택
	int AttackStackWeakening = 0;
	int AttackStackBlooding = 0;
	TArray<bool> GetStackCharmed = { false, false, false, false }; // 다음 턴 매혹 예약
	TArray<bool> GetCharmed = { false, false, false, false }; // 매혹 여부

	int GameHitNum = 0; // 게임에서 히트 받은 횟수
	int AllBloodEnd = 0;
	bool Ironmode = false; // 철괴(무적) 상태일 시
	int AttackUp = 0; // 다음 공격 추가 공격력
	bool ExtinctionWholeAttack = false; // 절멸 광역딜 준비
	bool ExtinctionWholeAttackDone = false; // 절멸 광역딜 개시
	int BonusDrawNum = 0; // 다음 턴 추가 드로우 장 수
	int BonusDrawDumpDeck = 0; // 다음 턴 버려진 덱에서 드로우 할 카드 장 수

	bool BeMadness = false; // 광기발현 사용 여부

	int ConfirmedStackCorrosion = 0; // 확정 관련 디버프 스택
	int ConfirmedStackWeakening = 0;
	int ConfirmedStackBlooding = 0;

	int Combo_Triple = 0; //이번 턴에 사용할 콤보 이득
	int Combo_JapHook = 0;
	int Combo_Extinction = 0;
	int Combo_Madness = 0;
	int ComboConfirm_Triple = 0; // 다음 턴에 받을 콤보 이득
	int ComboConfirm_JapHook = 0;
	int ComboConfirm_Extinction = 0;
	int ComboConfirm_Madness = 0;
	bool ComboUsing_Triple = false; // 이번 턴에 해당 콤보 카드 사용 여부
	bool ComboUsing_JapHook = false;
	bool ComboUsing_Extinction = false;
	bool ComboUsing_Madness = false;

	static int MyLevel;
	static int MyCurrentEXP;
	static int MyMaxEXP;
	static int MyMaxHealth;
	static int MyCurrentHealth;
	static int MyGold;
	static int MyMapLive;
	static int MySwordLv;
	static int MySheildLv;
	static TArray<int32> MyAccessoryData;
	static FTransform MyStartTransform;
	static FTransform MyStartTransform2;

	FTimerHandle StopMoveHandle;

protected:

	//UPROPERTY(VisibleAnyWhere)
	//	class UWidgetComponent* HealthWidgetComp;
	UPROPERTY(VisibleAnyWhere)
		class UAudioComponent* MyAudioComp;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* TakeBreathSound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* WalkingSound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* ParryingSound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* AttackedBiteSound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* AttackedKnifeSound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* AttackedKnife2Sound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* AttackedNormalSound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* AttackedNormal2Sound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* AttackedStrongSound;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* AttackedLongDistSound;

	bool IsWalking = false;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:


};