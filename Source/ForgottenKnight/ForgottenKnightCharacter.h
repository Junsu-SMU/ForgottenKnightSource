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
	void DamageEffect(int Turn, int MobPattern, bool Hit); // �� ���ݿ� ���� ����Ʈ �ٸ��� �ϱ�
	int ParryingWho(int mobnum, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken); // �и� �� �������� ������� ���� true = ��, false = ��
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

	int GetStackCorrosion = 0; // ���� ����� ����
	int GetStackWeakening = 0;
	int GetStackBlooding = 0;
	int AttackStackCorrosion = 0; // ��Ʈ ���� ����� ����
	int AttackStackWeakening = 0;
	int AttackStackBlooding = 0;
	TArray<bool> GetStackCharmed = { false, false, false, false }; // ���� �� ��Ȥ ����
	TArray<bool> GetCharmed = { false, false, false, false }; // ��Ȥ ����

	int GameHitNum = 0; // ���ӿ��� ��Ʈ ���� Ƚ��
	int AllBloodEnd = 0;
	bool Ironmode = false; // ö��(����) ������ ��
	int AttackUp = 0; // ���� ���� �߰� ���ݷ�
	bool ExtinctionWholeAttack = false; // ���� ������ �غ�
	bool ExtinctionWholeAttackDone = false; // ���� ������ ����
	int BonusDrawNum = 0; // ���� �� �߰� ��ο� �� ��
	int BonusDrawDumpDeck = 0; // ���� �� ������ ������ ��ο� �� ī�� �� ��

	bool BeMadness = false; // ������� ��� ����

	int ConfirmedStackCorrosion = 0; // Ȯ�� ���� ����� ����
	int ConfirmedStackWeakening = 0;
	int ConfirmedStackBlooding = 0;

	int Combo_Triple = 0; //�̹� �Ͽ� ����� �޺� �̵�
	int Combo_JapHook = 0;
	int Combo_Extinction = 0;
	int Combo_Madness = 0;
	int ComboConfirm_Triple = 0; // ���� �Ͽ� ���� �޺� �̵�
	int ComboConfirm_JapHook = 0;
	int ComboConfirm_Extinction = 0;
	int ComboConfirm_Madness = 0;
	bool ComboUsing_Triple = false; // �̹� �Ͽ� �ش� �޺� ī�� ��� ����
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