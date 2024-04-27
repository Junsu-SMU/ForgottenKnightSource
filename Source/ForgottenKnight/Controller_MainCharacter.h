// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Controller_MainCharacter.generated.h"

/**
 *
 */
UCLASS()
class FORGOTTENKNIGHT_API AController_MainCharacter : public APlayerController
{
	GENERATED_BODY()

public:
	AController_MainCharacter();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UWidget_Start> BP_Widget_Start;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_OutsideUI> BP_Widget_OutsideWindow;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Inventory> BP_Widget_Inventory;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Inventory_Cards> BP_Widget_Inventory_Cards;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_PlayWindow> BP_Widget_PlayWindow;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_StageClear> BP_Widget_StageClear;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_StageDie> BP_Widget_StageDie;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Store> BP_Widget_Store;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Store2> BP_Widget_Store2;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Store3> BP_Widget_Store3;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Store4> BP_Widget_Store4;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Store1_2> BP_Widget_Store1_2;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Store1_3> BP_Widget_Store1_3;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Store2_3> BP_Widget_Store2_3;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Store4_3> BP_Widget_Store4_3;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_EnterStage> BP_Widget_EnterStage;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Pause> BP_Widget_Pause;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_SaveSlot> BP_Widget_SaveSlot;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_LoadSlot> BP_Widget_LoadSlot;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Event_Heal> BP_Widget_Event_Heal;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Event_Fountain> BP_Widget_Event_Fountain;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Dialogue> BP_Widget_Dialogue;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_ClickDeck> BP_Widget_ClickDeck;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_DrawDeck> BP_Widget_DrawDeck;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Event_Dungeon> BP_Widget_Event_Dungeon;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_DungeonClear> BP_Widget_DungeonClear;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Event_3> BP_Widget_Event_3;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Equipment> BP_Widget_Equipment;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Tutorial> BP_Widget_Tutorial;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_HandPopup> BP_Widget_HandPopup;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Event_4> BP_Widget_Event_4;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_Event_4_Popup> BP_Widget_Event_4_Popup;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUI_ShowScript> BP_Widget_ShowScript;

	void ShowoutsideWidget();
	void ShowInventoryWidget();
	void HideInventoryWidget();
	void ShowInventoryCardsWidget();
	void HideInventoryCardsWidget();
	void ShowPlayWindowWidget();
	void HidePlayWindowWidget();
	void ShowStageClearWidget(int EXP, int Gold);
	void HideStageClearWidget();
	void ShowStageDieWidget(int EnterHP);
	void HideStageDieWidget();
	void ShowStoreWidget(int Order, int Page);
	void HideStoreWidget(int Order, int Page);
	void ShowEnterStageWidget(int Chapter, int Stage);
	void HideEnterStageWidget();
	void ShowPauseWidget();
	void HidePauseWidget();
	void ShowSaveSlotWidget();
	void HideSaveSlotWidget();
	void ShowLoadSlotWidget();
	void HideLoadSlotWidget();
	void ShowEventHealWidget(int Order);
	void HideEventHealWidget();
	void ShowEventFountainWidget(int Order);
	void HideEventFountainWidget();
	void ShowStartWidget();
	void HideStartWidget();
	void ShowDialogueWidget(int flowindex);
	void HideDialogueWidget();
	void ShowClickDeckWidget(int UseNum, int event);
	void HideClickDeckWidget();
	void ShowDrawDeckWidget(TArray<int32> Deck);
	void HideDrawDeckWidget();
	void ShowEventDungeonWidget(int Stage);
	void HideEventDungeonWidget();
	void ShowDungeonClearWidget(int EXP, int Gold, int clear);
	void HideDungeonClearWidget();
	void ShowEquipmentWidget();
	void HideEquipmentWidget();
	void FreezeStart();
	void FreezeEnd();
	void CallAttack(int Order);
	void InteractOn();
	void StoreChangeGold();
	int ConfirmEnemyNum();
	void DragStarting(int CardNum, int FixCost);
	void DetectEnterKey();
	void ShowCardClickPopup(int CardNum, int CardOrder);
	void OutsideChangeStat();
	bool SetLevelEXP(); // 레벨 업 했으면 true
	void ShowEvent3Widget(int Order);
	void HideEvent3Widget();
	void PlayUIMessage(int ui, int Order);
	void ShowTutorialWidget();
	void HideTutorialWidget();
	void ShowHandDeckWidget(TArray<int32> Deck);
	void HideHandDeckWidget(int CardNum);
	void ShowDumpDeckWidget(TArray<int32> Deck);
	void HideDumpDeckWidget(int CardNum);
	void ShowChronicityDeckWidget(TArray<int32> Deck);
	void HideChronicityDeckWidget(int CardNum);
	void ShowEvent4Widget(int Order);
	void HideEvent4Widget();
	void ShowEvent4PopupWidget(int Order);
	void HideEvent4PopupWidget();
	int ShowBonusAttack();
	void LoseKingHP(int value);
	void ShowScriptWidget(int purpose);
	void HideScriptWidget();
	void UpdateOutsideStat();

	static TArray<int32> MyDeck;
	static TArray<bool> MyProgress;
	bool ClickMovable = true;
	static FString InteractKeyword;
	static TArray<int32> Store1_1Count;
	static TArray<int32> Store2_1Count;
	static TArray<int32> Store4_1Count;
	static TArray<bool> Store1_3Count;
	static TArray<bool> Store2_3Count;
	static TArray<bool> Store4_3Count;
	static int GameSecond;
	static int GameMinute;
	static int GameHour;
	static TArray<int32> EventHealData;
	static TArray<int32> EventFountainData;
	static TArray<int32> EventDungeonData;
	static TArray<int32> Event3Data;
	static TArray<bool> Event4Data;
	static TArray<int32> StoreDeleteStack;
	static bool Tutorial;

	bool CardDragable = true;
	bool CardClickable = false;
	bool CardHandPopupClickable = false;
	bool CardDumpPopupClickable = false;
	bool CardChronicityDeckPopupClickable = false;
	int StoreOrderNum = 0;
	int StorePageNum = 0;
	int ClickUseNum = 0;
	FString EventCode = { "" };

	bool PlayerWalking = false;

	UPROPERTY()
		class UUI_PlayWindow* UI_PlayWindow;

protected:
	bool bClickMouse;

	FTimerHandle GameTimerHandle;

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	void InputClickPressed();
	void InputClickReleased();
	void SetNewDestination(const FVector DestLocation);
	void MoveToMouseCursor();
	void GameTimeManage();

private:

	UPROPERTY()
		class UWidget_Start* StartWidget;
	UPROPERTY()
		class UUI_OutsideUI* OutsideWidget;
	UPROPERTY()
		class UUI_Inventory* UI_Inventory;
	UPROPERTY()
		class UUI_Inventory_Cards* UI_Inventory_Cards;
	UPROPERTY()
		class UUI_StageClear* UI_StageClear;
	UPROPERTY()
		class UUI_StageDie* UI_StageDie;
	UPROPERTY()
		class UUI_Store* UI_Store;
	UPROPERTY()
		class UUI_Store2* UI_Store2;
	UPROPERTY()
		class UUI_Store3* UI_Store3;
	UPROPERTY()
		class UUI_Store4* UI_Store4;
	UPROPERTY()
		class UUI_Store1_2* UI_Store1_2;
	UPROPERTY()
		class UUI_Store1_3* UI_Store1_3;
	UPROPERTY()
		class UUI_Store2_3* UI_Store2_3;
	UPROPERTY()
		class UUI_Store4_3* UI_Store4_3;
	UPROPERTY()
		class UUI_EnterStage* UI_EnterStage;
	UPROPERTY()
		class UUI_Pause* UI_Pause;
	UPROPERTY()
		class UUI_SaveSlot* UI_SaveSlot;
	UPROPERTY()
		class UUI_LoadSlot* UI_LoadSlot;
	UPROPERTY()
		class UUI_Event_Heal* UI_Event_Heal;
	UPROPERTY()
		class UUI_Event_Fountain* UI_Event_Fountain;
	UPROPERTY()
		class UUI_Dialogue* UI_Dialogue;
	UPROPERTY()
		class UUI_ClickDeck* UI_ClickDeck;
	UPROPERTY()
		class UUI_DrawDeck* UI_DrawDeck;
	UPROPERTY()
		class UUI_Event_Dungeon* UI_Event_Dungeon;
	UPROPERTY()
		class UUI_DungeonClear* UI_DungeonClear;
	UPROPERTY()
		class UUI_Event_3* UI_Event_3;
	UPROPERTY()
		class UUI_Equipment* UI_Equipment;
	UPROPERTY()
		class UUI_Tutorial* UI_Tutorial;
	UPROPERTY()
		class UUI_HandPopup* UI_HandPopup;
	UPROPERTY()
		class UUI_Event_4* UI_Event_4;
	UPROPERTY()
		class UUI_Event_4_Popup* UI_Event_4_Popup;
	UPROPERTY()
		class UUI_ShowScript* UI_ShowScript;
};
