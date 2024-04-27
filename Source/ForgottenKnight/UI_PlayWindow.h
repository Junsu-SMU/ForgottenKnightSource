// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_PlayCard.h"
#include "UI_PlayWindow.generated.h"

/**
 *
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_PlayWindow : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
		void OnButtonEndClicked();
	UFUNCTION()
		void OnButtonFirstClicked();
	UFUNCTION()
		void OnButtonSecondClicked();
	UFUNCTION()
		void OnButtonThirdClicked();
	UFUNCTION()
		void OnButtonBreathClicked();
	UFUNCTION()
		void OnButtonDeckClicked();
	UFUNCTION()
		void TurnStartEnded();
	UFUNCTION()
		void TurnEndEnded();
	UFUNCTION()
		void OnButtonPauseClicked();
	UFUNCTION()
		void OnButtonDrawDeckClicked();
	UFUNCTION()
		void OnButtonDumpDeckClicked();
	UFUNCTION()
		void OnButtonPopupYesClicked();
	UFUNCTION()
		void OnButtonPopupNoClicked();
	UFUNCTION()
		void OnButtonBehindHandClicked();
	UFUNCTION()
		void OnButtonFrontHandClicked();
	UFUNCTION()
		void BloodSetEnded();

	FWidgetAnimationDynamicEvent TurnStartDelegate;
	FWidgetAnimationDynamicEvent TurnEndDelegate;
	FWidgetAnimationDynamicEvent BloodSetDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardWidget")
		TSubclassOf<UUserWidget> CardWidget;

	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* TakeBreathSound;

public:

	virtual void NativeOnInitialized() override;

	void Mix();
	void TurnStart();
	void UpdateCost();
	void UpdateMaxCost();
	void TurnClear();
	void StageClear();
	void StageDie();
	void ChangeAttackImage(int AttackNum, int CardNum, int EnemyNum);
	bool CardDragControl(int CardNum);
	void Attack1Ended();
	void Attack2Ended();
	void Attack3Ended();
	void RecoverDragingCard();
	void ChangeHandCost();
	void ChangeHandUpgrade();
	void DrawCard(int DrawNum);
	bool Attacking(int MobNum, int turn); // 공격
	bool Attacked(int MobNum, int turn); // 적 공격
	bool BonusAttacked(int MobNum, int turn); // 적 공격
	int ParryingWho(int MobNum, int turn); // 패링
	void ChangeTurnText();
	void FixHandCost(int CardNum);
	void FastAttacking(int MobNum, int AttackValue);
	void CheckLife();
	void WarlockPhase1Revive();
	void Cleanze(int MobNum);
	void SkullRevive();
	void LichReviveSkill(int mobnum);
	bool FieldOnlyLich();
	void SkeletonFlag();
	int ShowBonusAttack();
	void TakeDumpCard(int CardNum);
	void TakeDrawCard(int CardNum);
	void TakeChronicityCard(int CardNum);
	void DurahanGiveHead();
	bool ChronicitySelect();
	void LastStagePhase(int Phase);
	void UpdateKingHealth();
	void MawangRevive();
	void LoseKingHP(int value);
	void SpecialBoom();
	bool CheckTurnClear();

	int DragingCardNum = 0;
	int DragingCardFixCost = 0;
	int DrawCardIndex = 0; // 뽑을 카드 인덱스
	int CardCircle = 0; // 뽑을 카드 덱의 총 개수
	int TurnCount = 0; // 턴 카운트
	int CurrentCost = 3;
	int MaxCost = 3;
	int EnemyCount = 3;
	bool FieldinAsura = false;
	bool FieldinWarlock = false;
	int WarlockMobCount = 0;
	bool BeDelirium = false; // 착란 사용 여부
	bool BeGrinding = false; // 기본기 연마 사용 여부
	bool BeMadness = false; // 광기발현 사용 여부
	bool BeCombination = false; // 연격의 대가 사용 여부
	int SkullReviveCooltime = 2; // 평소 쿨타임 2턴, 해골 백인대장 보유시 1턴
	int FlagOn = 0; // 0 = x, 1 = 빨, 2 = 파, 3 = 초
	TArray<bool> FlagEffect = { false, false, false, false }; // 영향 받은 적 식별
	int CombinationBonusDraw = 2; // 연격의 대가 추가 드로우 상한 2
	bool BeChronicity = false; // 대기만성 발동 여부
	int TakeChronicityNum = 0;
	bool SpecialBoomOn = false; // 마지막 스테이지 영웅의 일격 기믹
	int realthisturn = 0;
	bool DeckFront = true;
	bool IronHeal = false;

	int StartCurrentHealth = 0;

	TArray<int32> MobDone = { 0, 0, 0, 0 };
	TArray<int32> MobTargetCard = { 0, 0, 0, 0 };
	TArray<int32> MobTargetCost = { 0, 0, 0, 0 };
	TArray<int32> MobTargetFixCost = { 0, 0, 0, 0 };
	TArray<int32> EnemyAttackOrder = { 0, 2, 2, 2 }; // 적의 공격순서, 통상2
	TArray<bool> Attack = { false, false, false, false };
	TArray<bool> EnemyLife = { false, false, false, false };
	TArray<bool> Breathe = { false, false, false, false }; // 인덱스0은 게임 시작 시 여부

	TArray<int32> SoonSkullHead = { 0, 0, 0, 0 }; // 다음 턴에 해골머리로 바뀔 예정

	bool WaitBlooding = false; // 출혈 이펙트 끝날때까지 대기

	bool OnCombo_Extinction_CostDown1 = false;
	bool OnCombo_Triangle_CostDown1 = false;
	TArray<bool> ComboOnByAttackOrder_Extinction = { 0, 0, 0, 0 }; // 공격 순서 직전에 따른 절멸 콤보on여부, 인덱스0은 턴 시작시 여부
	TArray<bool> ComboOnByAttackOrder_Triangle = { 0, 0, 0, 0 }; // 공격 순서 직전에 따른 삼위일체 콤보on여부, 인덱스0은 턴 시작시 여부
	TArray<int32> CardNumByAttackOrder = { 0, 0, 0, 0 }; // 공격 순서에 따른 카드인덱스값, 인덱스0은 사용x

	TArray<int32> MixDeck;
	TArray<int32> DrawDeck;
	TArray<int32> DumpDeck;
	TArray<int32> HandCard = { 0, 0, 0, 0, 0 };
	TArray<int32> ChronicityDeck;
	int CardDeckOrder = 0;

	int EnterHP = 0;
	float mycor = 1.25f;
	float myweak = 0.75f;

	int KingHP = 100;
	bool LastStage = false;
	bool LastClear = false;
	int LastOrder = 0; // 마지막 스테이지 패턴 순서 (0 = 1-1페이즈, 1 = 1-2페이즈, 2 = 1-3페이즈, 3 = 2페이즈, 4 = 3페이즈)

	bool Dial1st = false; // 대화문 먼저 보고 보상 받기 변수

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* CardBox;
	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* CardBox2;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_End;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_First;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Second;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Third;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Breath;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Deck;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Pause;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_DrawDeck;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_DumpDeck;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_FrontHand;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_BehindHand;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_CurrentCost;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_MaxCost;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Notify;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_1st;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_2nd;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_3rd;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Background;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_AttackCard1;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_AttackCard2;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_AttackCard3;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_AttackMob1;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_AttackMob2;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_AttackMob3;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Buffmob;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Cerberus;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_demonhead;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_demonleft;
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_demonright;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Background_Stage1;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Background_Stage2;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Background_Dungeon;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBreathe; // 숨고르기 이미지

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card1; // 카드1 이미지
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card2;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card3;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card4;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card5;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card6;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card7;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card8;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card9;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card10;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card11;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card12;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card13;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card14;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card15;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card16;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card17;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card18;

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card19; // 상처 벌리기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card20; // 유연한 강함
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card21; // 준비만전
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card22; // 땅 파기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card23; // 대기만성
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card24; // 갑옷 부수기
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card25; // 삼위일체1
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card26; // 삼위일체2
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card27; // 삼위일체3

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card31;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card32;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card33;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card34;

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card35; // 오의절멸4
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card36; // 더티 파이트
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card37; // 무소유

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card41;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card42;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card43;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card44;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card45;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card46;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card47;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card48;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card49;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card50;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card51;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card52;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card53;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card54;

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card81;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card82;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card83; // 광기 발현
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush Card84; // 연격의 대가


	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSlime; // 슬라임 이미지
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgGoblin;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgTroll;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSpider;
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
		struct FSlateBrush ImgSkeletonCenturion; // 해골 백인대장
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSkeletonCenturionR;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSkeletonCenturionB;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgSkeletonCenturionG;

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBeholder;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgFallenFairy;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBaphomet;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgDurahan;

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgKing;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgKing2;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgDemonLordHead;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgDemonLordLeft;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgDemonLordRight;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgDemonLordBody;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgBall;

	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgCerABC;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgCerAC;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgCerAB;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgCerBC;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgCerA;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgCerB;
	UPROPERTY(EditDefaultsOnly)
		struct FSlateBrush ImgCerC;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_TurnStart;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_TurnEnd;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_TurnEnd2;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Message;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_TurnBox1;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_TurnBox2;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_Cerberus;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_BloodSet;

	UPROPERTY(meta = (BindWidget))
		class UImage* Image_Popup;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Popup;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_PopupYes;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_PopupNo;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* KingHealthText;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* KingHealthBar;

	FVector2D Mob1;
	FVector2D Mob2;
	FVector2D Mob3;
	FVector2D ErrorGap = FVector2D(140, 210);
	FVector2D PreMob1;
	FVector2D PreMob2;
	FVector2D PreMob3;

	UPROPERTY(meta = (BindWidget))
		class UEnemyHealthBar* EnemyHealthbar1;
	UPROPERTY(meta = (BindWidget))
		class UEnemyHealthBar* EnemyHealthbar2;
	UPROPERTY(meta = (BindWidget))
		class UEnemyHealthBar* EnemyHealthbar3;
	UPROPERTY(meta = (BindWidget))
		class UMyHealthBar* MyHealthbar0;

};
