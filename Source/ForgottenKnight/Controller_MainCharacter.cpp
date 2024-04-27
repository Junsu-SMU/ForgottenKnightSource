// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller_MainCharacter.h"
#include "ForgottenKnightCharacter.h"
#include "EnemyCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Widget_Start.h"
#include "UI_Inventory.h"
#include "UI_Inventory_Cards.h"
#include "UI_OutsideUI.h"
#include "UI_PlayWindow.h"
#include "UI_StageDie.h"
#include "UI_StageClear.h"
#include "UI_Store.h"
#include "UI_Store2.h"
#include "UI_Store3.h"
#include "UI_Store4.h"
#include "UI_EnterStage.h"
#include "UI_Pause.h"
#include "UI_Store1_2.h"
#include "UI_Store1_3.h"
#include "UI_Store2_3.h"
#include "UI_Store4_3.h"
#include "UI_SaveSlot.h"
#include "UI_LoadSlot.h"
#include "UI_Event_Heal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "EventObject.h"
#include "EngineUtils.h"
#include "UI_Dialogue.h"
#include "UI_ClickDeck.h"
#include "UI_DrawDeck.h"
#include "UI_Event_Dungeon.h"
#include "UI_DungeonClear.h"
#include "UI_Event_3.h"
#include "UI_Equipment.h"
#include "UI_Tutorial.h"
#include "MyHealthBar.h"
#include "EnemyHealthBar.h"
#include "UI_HandPopup.h"
#include "UI_Event_4.h"
#include "UI_Event_4_Popup.h"
#include "UI_Event_Fountain.h"
#include "UI_ShowScript.h"

// 수량 고칠때, LoadNewGame()에 있는 수량도 같이 고칠것, 필수!
TArray<int32> AController_MainCharacter::MyDeck = { 1, 1, 1, 1, 1, 1, 1, 2, 3, 4 };
TArray<bool> AController_MainCharacter::MyProgress = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
FString AController_MainCharacter::InteractKeyword = { " " };
TArray<int32> AController_MainCharacter::Store1_1Count = { 3, 1, 1, 1, 1, 1, 2, 1, 1 };
TArray<int32> AController_MainCharacter::Store2_1Count = { 3, 1, 1, 1, 1, 1, 15, 5, 5, 1, 10, 1 };
TArray<int32> AController_MainCharacter::Store4_1Count = { 3, 1, 1, 1, 1, 1, 5, 5, 5, 1, 1, 1 };
TArray<bool> AController_MainCharacter::Store1_3Count = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true }; // 20품목
TArray<bool> AController_MainCharacter::Store2_3Count = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true }; // 17품목
TArray<bool> AController_MainCharacter::Store4_3Count = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                                          true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                                          true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true }; // 49품목 : 35, 12, 2
int AController_MainCharacter::GameSecond = 0;
int AController_MainCharacter::GameMinute = 0;
int AController_MainCharacter::GameHour = 0;
TArray<int32> AController_MainCharacter::EventHealData = { 3, 3, 3, 3, 3, 3 };
TArray<int32> AController_MainCharacter::EventFountainData = { 3, 3, 3, 3 };
TArray<int32> AController_MainCharacter::EventDungeonData = { 0, 0, 0, 0, 0, 0 };
TArray<int32> AController_MainCharacter::Event3Data = { 0, 1, 1, 1, 1, 1 };
TArray<bool> AController_MainCharacter::Event4Data = { false, true, true, true, true, true };
TArray<int32> AController_MainCharacter::StoreDeleteStack = { 0, 0, 0 }; // 상점 별 카드 제거횟수
bool AController_MainCharacter::Tutorial = false;

AController_MainCharacter::AController_MainCharacter()
{
    bShowMouseCursor = true;

}

void AController_MainCharacter::BeginPlay()
{
    Super::BeginPlay();

    AController_MainCharacter::InteractKeyword = 0;

    GetWorldTimerManager().SetTimer(GameTimerHandle,
        this,
        &AController_MainCharacter::GameTimeManage, // 사용할 함수
        1, // 쿨타임
        true); // 루프X

    // 흑마 다이얼로그
    int warlock = 0;
    for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
    {
        AEnemyCharacter* Enemy = *Iter;

        if (Enemy != nullptr)
        {
            if (Enemy->ActorHasTag("Warlock"))
            {
                warlock += 1;
            }
        }
    }
    if (warlock > 0) // 필드에 흑마 있으면
    {
        ShowDialogueWidget(0);
    }

    AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(GetCharacter());
    if (MyPlayer != nullptr)
    {
        if (MyPlayer->ActorHasTag("Stage2_10"))
        {
            ShowDialogueWidget(3);
        }
    }
}

void AController_MainCharacter::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if (bClickMouse)
    {
        MoveToMouseCursor();
    }

    AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(GetCharacter());
    if (MyPlayer != nullptr)
    {
        if ((GetCharacter()->GetVelocity() != FVector{ 0, 0, 0 }) && PlayerWalking == false)
        {
            MyPlayer->WalkSoundBase(true);
            PlayerWalking = true;
        }
        else if ((GetCharacter()->GetVelocity() == FVector{ 0, 0, 0 }) && PlayerWalking == true)
        {
            MyPlayer->WalkSoundBase(false);
            PlayerWalking = false;
        }
    }

}

void AController_MainCharacter::GameTimeManage()
{
    AController_MainCharacter::GameSecond += 1;
    if (AController_MainCharacter::GameSecond >= 60)
    {
        AController_MainCharacter::GameSecond = 0;
        AController_MainCharacter::GameMinute += 1;
        if (AController_MainCharacter::GameMinute >= 60)
        {
            AController_MainCharacter::GameMinute = 0;
            AController_MainCharacter::GameHour += 1;
        }
    }
}

void AController_MainCharacter::ShowoutsideWidget()
{
    if (BP_Widget_OutsideWindow != nullptr)
    {
        SetPause(false);
        SetInputMode(FInputModeGameAndUI());
        bShowMouseCursor = true;
        OutsideWidget = CreateWidget<UUI_OutsideUI>(this, BP_Widget_OutsideWindow);
        OutsideWidget->AddToViewport();
    }
}

void AController_MainCharacter::ShowInventoryWidget()
{
    if (BP_Widget_Inventory != nullptr)
    {
        AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(GetCharacter());
        if (MyPlayer != nullptr)
        {
            MyPlayer->Movable = false;
        }
        bShowMouseCursor = true;
        UI_Inventory = CreateWidget<UUI_Inventory>(this, BP_Widget_Inventory);
        UI_Inventory->AddToViewport();
    }
}

void AController_MainCharacter::HideInventoryWidget()
{
    if (BP_Widget_Inventory != nullptr)
    {
        UI_Inventory->RemoveFromParent();
        UI_Inventory->Destruct();
        AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(GetCharacter());
        if (MyPlayer != nullptr)
        {
            MyPlayer->Movable = true;
        }
    }
}

void AController_MainCharacter::ShowInventoryCardsWidget()
{
    if (BP_Widget_Inventory_Cards != nullptr)
    {
        CardDragable = false;
        UI_Inventory_Cards = CreateWidget<UUI_Inventory_Cards>(this, BP_Widget_Inventory_Cards);
        UI_Inventory_Cards->AddToViewport();
    }
}

void AController_MainCharacter::HideInventoryCardsWidget()
{
    if (BP_Widget_Inventory_Cards != nullptr)
    {
        CardDragable = true;
        UI_Inventory_Cards->RemoveFromParent();
        UI_Inventory_Cards->Destruct();
    }
}

void AController_MainCharacter::ShowPlayWindowWidget()
{
    if (BP_Widget_PlayWindow != nullptr)
    {
        SetPause(false);
        SetInputMode(FInputModeGameAndUI());
        bShowMouseCursor = true;
        UI_PlayWindow = CreateWidget<UUI_PlayWindow>(this, BP_Widget_PlayWindow);
        UI_PlayWindow->AddToViewport();
    }
}

void AController_MainCharacter::HidePlayWindowWidget()
{
    if (BP_Widget_PlayWindow != nullptr)
    {
        UI_PlayWindow->RemoveFromParent();
        UI_PlayWindow->Destruct();
        SetPause(false);
    }
}

void AController_MainCharacter::ShowStageClearWidget(int EXP, int Gold)
{
    if (BP_Widget_StageClear != nullptr)
    {
        AForgottenKnightCharacter::MyCurrentHealth += 3;
        if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
        {
            AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
        }

        bool x = SetLevelEXP();

        SetPause(true);
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
        UI_StageClear = CreateWidget<UUI_StageClear>(this, BP_Widget_StageClear);
        UI_StageClear->AddToViewport();
        UI_StageClear->ChangeStat(EXP, Gold);
        if (x)
        {
            UI_StageClear->ShowLvUp();
        }
    }
}

void AController_MainCharacter::HideStageClearWidget()
{
    if (BP_Widget_StageClear != nullptr)
    {
        UI_StageClear->RemoveFromParent();
        UI_StageClear->Destruct();
        SetPause(false);
    }
}

void AController_MainCharacter::ShowStageDieWidget(int EnterHP)
{
    if (BP_Widget_StageDie != nullptr)
    {
        SetPause(true);
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
        UI_StageDie = CreateWidget<UUI_StageDie>(this, BP_Widget_StageDie);
        UI_StageDie->AddToViewport();
        UI_StageDie->NewHP = EnterHP;
    }
}

void AController_MainCharacter::HideStageDieWidget()
{
    if (BP_Widget_StageDie != nullptr)
    {
        UI_StageDie->RemoveFromParent();
        UI_StageDie->Destruct();
        SetPause(false);
    }
}

void AController_MainCharacter::ShowStoreWidget(int Order, int Page)
{
    CardDragable = false;
    if (Order == 1)
    {
        if (Page == 1)
        {
            bShowMouseCursor = true;
            StoreOrderNum = 1;
            StorePageNum = 1;
            UI_Store = CreateWidget<UUI_Store>(this, BP_Widget_Store);
            UI_Store->AddToViewport();
        }
        else if (Page == 2)
        {
            bShowMouseCursor = true;
            StoreOrderNum = 1;
            StorePageNum = 2;
            UI_Store1_2 = CreateWidget<UUI_Store1_2>(this, BP_Widget_Store1_2);
            UI_Store1_2->AddToViewport();
        }
        else if (Page == 3)
        {
            bShowMouseCursor = true;
            StoreOrderNum = 1;
            StorePageNum = 3;
            UI_Store1_3 = CreateWidget<UUI_Store1_3>(this, BP_Widget_Store1_3);
            UI_Store1_3->AddToViewport();
        }
    }
    else if (Order == 2)
    {
        if (Page == 1)
        {
            bShowMouseCursor = true;
            StoreOrderNum = 2;
            StorePageNum = 1;
            UI_Store2 = CreateWidget<UUI_Store2>(this, BP_Widget_Store2);
            UI_Store2->AddToViewport();
        }
        else if (Page == 2)
        {
            bShowMouseCursor = true;
            StoreOrderNum = 2;
            StorePageNum = 2;
            UI_Store1_2 = CreateWidget<UUI_Store1_2>(this, BP_Widget_Store1_2);
            UI_Store1_2->AddToViewport();
        }
        else if (Page == 3)
        {
            bShowMouseCursor = true;
            StoreOrderNum = 2;
            StorePageNum = 3;
            UI_Store2_3 = CreateWidget<UUI_Store2_3>(this, BP_Widget_Store2_3);
            UI_Store2_3->AddToViewport();
        }
    }
    else if (Order == 3)
    {
        bShowMouseCursor = true;
        StoreOrderNum = 3;
        StorePageNum = 1;
        UI_Store3 = CreateWidget<UUI_Store3>(this, BP_Widget_Store3);
        UI_Store3->AddToViewport();
    }
    else if (Order == 4)
    {
        if (Page == 1)
        {
            bShowMouseCursor = true;
            StoreOrderNum = 4;
            StorePageNum = 1;
            UI_Store4 = CreateWidget<UUI_Store4>(this, BP_Widget_Store4);
            UI_Store4->AddToViewport();
        }
        else if (Page == 2)
        {
            bShowMouseCursor = true;
            StoreOrderNum = 4;
            StorePageNum = 2;
            UI_Store1_2 = CreateWidget<UUI_Store1_2>(this, BP_Widget_Store1_2);
            UI_Store1_2->AddToViewport();
        }
        else if (Page == 3)
        {
            bShowMouseCursor = true;
            StoreOrderNum = 4;
            StorePageNum = 3;
            UI_Store4_3 = CreateWidget<UUI_Store4_3>(this, BP_Widget_Store4_3);
            UI_Store4_3->AddToViewport();
        }
    }
}

void AController_MainCharacter::HideStoreWidget(int Order, int Page)
{
    CardDragable = true;
    OutsideChangeStat();
    if (Order == 1)
    {
        if (Page == 1)
        {
            if (BP_Widget_OutsideWindow != nullptr && UI_Store)
            {
                UI_Store->RemoveFromParent();
                UI_Store->Destruct();
            }
        }
        else if (Page == 2)
        {
            if (BP_Widget_OutsideWindow != nullptr && UI_Store1_2)
            {
                UI_Store1_2->RemoveFromParent();
                UI_Store1_2->Destruct();
            }
        }
        else if (Page == 3)
        {
            if (BP_Widget_OutsideWindow != nullptr && UI_Store1_3)
            {
                UI_Store1_3->RemoveFromParent();
                UI_Store1_3->Destruct();
            }
        }
    }
    else if (Order == 2)
    {
        if (Page == 1)
        {
            if (BP_Widget_OutsideWindow != nullptr && UI_Store2)
            {
                UI_Store2->RemoveFromParent();
                UI_Store2->Destruct();
            }
        }
        else if (Page == 2)
        {
            if (BP_Widget_OutsideWindow != nullptr && UI_Store1_2)
            {
                UI_Store1_2->RemoveFromParent();
                UI_Store1_2->Destruct();
            }
        }
        else if (Page == 3)
        {
            if (BP_Widget_OutsideWindow != nullptr && UI_Store2_3)
            {
                UI_Store2_3->RemoveFromParent();
                UI_Store2_3->Destruct();
            }
        }
    }
    else if (Order == 3)
    {
        if (BP_Widget_OutsideWindow != nullptr && UI_Store3)
        {
            UI_Store3->RemoveFromParent();
            UI_Store3->Destruct();
        }
    }
    else if (Order == 4)
    {
        if (Page == 1)
        {
            if (BP_Widget_OutsideWindow != nullptr && UI_Store4)
            {
                UI_Store4->RemoveFromParent();
                UI_Store4->Destruct();
            }
        }
        else if (Page == 2)
        {
            if (BP_Widget_OutsideWindow != nullptr && UI_Store1_2)
            {
                UI_Store1_2->RemoveFromParent();
                UI_Store1_2->Destruct();
            }
        }
        else if (Page == 3)
        {
            if (BP_Widget_OutsideWindow != nullptr && UI_Store4_3)
            {
                UI_Store4_3->RemoveFromParent();
                UI_Store4_3->Destruct();
            }
        }
    }
}

void AController_MainCharacter::ShowEnterStageWidget(int Chapter, int Stage)
{
    if (BP_Widget_EnterStage != nullptr)
    {
        bShowMouseCursor = true;
        UI_EnterStage = CreateWidget<UUI_EnterStage>(this, BP_Widget_EnterStage);
        UI_EnterStage->AddToViewport();
        UI_EnterStage->ChangeStat(Chapter, Stage);
    }
}

void AController_MainCharacter::HideEnterStageWidget()
{
    if (BP_Widget_EnterStage != nullptr && UI_EnterStage)
    {
        UI_EnterStage->RemoveFromParent();
        UI_EnterStage->Destruct();
    }
}

void AController_MainCharacter::ShowPauseWidget()
{
    if (BP_Widget_Pause != nullptr)
    {
        AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(GetCharacter());
        if (MyPlayer != nullptr)
        {
            MyPlayer->Movable = false;
        }
        bShowMouseCursor = true;
        UI_Pause = CreateWidget<UUI_Pause>(this, BP_Widget_Pause);
        UI_Pause->AddToViewport();
    }
}

void AController_MainCharacter::HidePauseWidget()
{
    if (BP_Widget_Pause != nullptr)
    {
        UI_Pause->RemoveFromParent();
        UI_Pause->Destruct();
        AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(GetCharacter());
        if (MyPlayer != nullptr)
        {
            MyPlayer->Movable = true;
        }
    }
}

void AController_MainCharacter::ShowSaveSlotWidget()
{
    if (BP_Widget_SaveSlot != nullptr)
    {
        SetPause(true);
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
        UI_SaveSlot = CreateWidget<UUI_SaveSlot>(this, BP_Widget_SaveSlot);
        UI_SaveSlot->AddToViewport();
    }
}

void AController_MainCharacter::HideSaveSlotWidget()
{
    if (BP_Widget_SaveSlot != nullptr)
    {
        UI_SaveSlot->RemoveFromParent();
        UI_SaveSlot->Destruct();
        SetPause(false);
        SetInputMode(FInputModeGameAndUI());
    }
}

void AController_MainCharacter::ShowLoadSlotWidget()
{
    if (BP_Widget_LoadSlot != nullptr)
    {
        SetPause(true);
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
        UI_LoadSlot = CreateWidget<UUI_LoadSlot>(this, BP_Widget_LoadSlot);
        UI_LoadSlot->AddToViewport();
    }
}

void AController_MainCharacter::HideLoadSlotWidget()
{
    if (BP_Widget_LoadSlot != nullptr)
    {
        UI_LoadSlot->RemoveFromParent();
        UI_LoadSlot->Destruct();
        SetPause(false);
        SetInputMode(FInputModeGameAndUI());
    }
}

void AController_MainCharacter::ShowStartWidget()
{
    if (BP_Widget_Start != nullptr)
    {
        SetPause(true);
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
        StartWidget = CreateWidget<UWidget_Start>(this, BP_Widget_Start);
        StartWidget->AddToViewport();
    }
}

void AController_MainCharacter::HideStartWidget()
{
    if (BP_Widget_Start != nullptr)
    {
        StartWidget->RemoveFromParent();
        StartWidget->Destruct();
        SetPause(false);
        SetInputMode(FInputModeGameAndUI());
    }
}

void AController_MainCharacter::ShowEventHealWidget(int Order)
{
    if (BP_Widget_Event_Heal != nullptr)
    {
        bShowMouseCursor = true;
        UI_Event_Heal = CreateWidget<UUI_Event_Heal>(this, BP_Widget_Event_Heal);
        UI_Event_Heal->EventOrder = Order;
        UI_Event_Heal->SetTextTake();
        UI_Event_Heal->AddToViewport();
    }
}

void AController_MainCharacter::HideEventHealWidget()
{
    if (BP_Widget_Event_Heal != nullptr && UI_Event_Heal)
    {
        UI_Event_Heal->RemoveFromParent();
        UI_Event_Heal->Destruct();
        EventCode = { "" };
        OutsideChangeStat();
    }
}

void AController_MainCharacter::ShowEventFountainWidget(int Order)
{
    if (BP_Widget_Event_Fountain != nullptr)
    {
        bShowMouseCursor = true;
        UI_Event_Fountain = CreateWidget<UUI_Event_Fountain>(this, BP_Widget_Event_Fountain);
        UI_Event_Fountain->EventOrder = Order;
        UI_Event_Fountain->SetTextTake();
        UI_Event_Fountain->AddToViewport();
    }
}

void AController_MainCharacter::HideEventFountainWidget()
{
    if (BP_Widget_Event_Fountain != nullptr && UI_Event_Fountain)
    {
        UI_Event_Fountain->RemoveFromParent();
        UI_Event_Fountain->Destruct();
        EventCode = { "" };
        OutsideChangeStat();
    }
}

void AController_MainCharacter::ShowDialogueWidget(int flowindex)
{
    if (BP_Widget_Dialogue != nullptr)
    {
        SetPause(false);
        SetInputMode(FInputModeGameAndUI());
        bShowMouseCursor = true;
        UI_Dialogue = CreateWidget<UUI_Dialogue>(this, BP_Widget_Dialogue);
        UI_Dialogue->FlowIndex = flowindex;
        UI_Dialogue->AddToViewport();
    }
}

void AController_MainCharacter::HideDialogueWidget()
{
    if (BP_Widget_Dialogue != nullptr)
    {
        int xexe = UI_Dialogue->FlowIndex;

        UI_Dialogue->RemoveFromParent();
        UI_Dialogue->Destruct();
        SetPause(false);
        SetInputMode(FInputModeGameAndUI());

        if (xexe == 2) // 1_10클리어
        {
            UI_PlayWindow->Dial1st = true;
            UI_PlayWindow->StageClear();
        }
        else if (xexe == 6) // 최종클리어
        {
            ShowScriptWidget(2);
        }
        else if (xexe == 7) // 영웅의일격 발동
        {
            UI_PlayWindow->SpecialBoom();
        }
    }
}

void AController_MainCharacter::ShowClickDeckWidget(int UseNum, int event)
{
    if (BP_Widget_ClickDeck != nullptr)
    {
        ClickUseNum = UseNum;
        CardClickable = true;
        bShowMouseCursor = true;
        UI_ClickDeck = CreateWidget<UUI_ClickDeck>(this, BP_Widget_ClickDeck);
        UI_ClickDeck->AddToViewport();
        UI_ClickDeck->EventOrder = event;
        UI_ClickDeck->SetMessage(UseNum, StoreOrderNum);
    }
}

void AController_MainCharacter::HideClickDeckWidget()
{
    if (BP_Widget_ClickDeck != nullptr)
    {
        UI_ClickDeck->RemoveFromParent();
        UI_ClickDeck->Destruct();
        ClickUseNum = 0;
        CardClickable = false;

        if (StoreOrderNum == 1)
        {
            UI_Store1_2->ChangeGold();
        }

        if (EventCode == FString{ "3" })
        {
            if (BP_Widget_Event_3 != nullptr)
            {
                UI_Event_3->UpdateStat();
            }
        }
    }
}

void AController_MainCharacter::ShowDrawDeckWidget(TArray<int32> Deck) // 0이면 뽑을 카드, 1이면 버린 카드
{
    if (BP_Widget_DrawDeck != nullptr)
    {
        CardDragable = false;
        bShowMouseCursor = true;
        UI_DrawDeck = CreateWidget<UUI_DrawDeck>(this, BP_Widget_DrawDeck);
        UI_DrawDeck->AddToViewport();
        UI_DrawDeck->SetDeck(Deck);
    }
}

void AController_MainCharacter::HideDrawDeckWidget()
{
    if (BP_Widget_ClickDeck != nullptr)
    {
        CardDragable = true;
        UI_DrawDeck->RemoveFromParent();
        UI_DrawDeck->Destruct();
    }
}

void AController_MainCharacter::ShowHandDeckWidget(TArray<int32> Deck)
{
    if (BP_Widget_HandPopup != nullptr)
    {
        CardDragable = false;
        CardHandPopupClickable = true;
        bShowMouseCursor = true;
        UI_HandPopup = CreateWidget<UUI_HandPopup>(this, BP_Widget_HandPopup);
        UI_HandPopup->AddToViewport();
        UI_HandPopup->SetDeck(Deck);
    }
}

void AController_MainCharacter::HideHandDeckWidget(int CardNum)
{
    if (BP_Widget_HandPopup != nullptr)
    {
        CardDragable = true;
        CardHandPopupClickable = false;
        UI_HandPopup->RemoveFromParent();
        UI_HandPopup->Destruct();
        UI_PlayWindow->FixHandCost(CardNum);
    }
}

void AController_MainCharacter::ShowDumpDeckWidget(TArray<int32> Deck)
{
    if (BP_Widget_HandPopup != nullptr)
    {
        CardDragable = false;
        CardDumpPopupClickable = true;
        bShowMouseCursor = true;
        UI_HandPopup = CreateWidget<UUI_HandPopup>(this, BP_Widget_HandPopup);
        UI_HandPopup->AddToViewport();
        UI_HandPopup->SetDeck(Deck);
    }
}

void AController_MainCharacter::HideDumpDeckWidget(int CardNum)
{
    if (BP_Widget_HandPopup != nullptr)
    {
        CardDragable = true;
        CardDumpPopupClickable = false;
        UI_HandPopup->RemoveFromParent();
        UI_HandPopup->Destruct();
        UI_PlayWindow->TakeDumpCard(CardNum);
    }
}

void AController_MainCharacter::ShowChronicityDeckWidget(TArray<int32> Deck)
{
    if (BP_Widget_HandPopup != nullptr)
    {
        CardDragable = false;
        CardChronicityDeckPopupClickable = true;
        bShowMouseCursor = true;
        UI_HandPopup = CreateWidget<UUI_HandPopup>(this, BP_Widget_HandPopup);
        UI_HandPopup->AddToViewport();
        UI_HandPopup->SetDeck(Deck);
    }
}

void AController_MainCharacter::HideChronicityDeckWidget(int CardNum)
{
    if (BP_Widget_HandPopup != nullptr)
    {
        CardDragable = true;
        CardChronicityDeckPopupClickable = false;
        UI_HandPopup->RemoveFromParent();
        UI_HandPopup->Destruct();
        UI_PlayWindow->TakeChronicityCard(CardNum);
    }
}

void AController_MainCharacter::ShowEventDungeonWidget(int Stage)
{
    if (BP_Widget_Event_Dungeon != nullptr)
    {
        bShowMouseCursor = true;
        UI_Event_Dungeon = CreateWidget<UUI_Event_Dungeon>(this, BP_Widget_Event_Dungeon);
        UI_Event_Dungeon->AddToViewport();
        UI_Event_Dungeon->ChangeStat(Stage);
    }
}

void AController_MainCharacter::HideEventDungeonWidget()
{
    if (BP_Widget_Event_Dungeon != nullptr && UI_Event_Dungeon)
    {
        UI_Event_Dungeon->RemoveFromParent();
        UI_Event_Dungeon->Destruct();
    }
}

void AController_MainCharacter::ShowDungeonClearWidget(int EXP, int Gold, int clear)
{ // clear가 0이면 던전 탈출x, -1이면 탈출인데 이미 깨서 보상x, 각 던전 인덱스별로 clear값에 따라 보상받고 탈출
    if (BP_Widget_DungeonClear != nullptr)
    {
        if (clear == 1) // exp50, 골드100, 체력30퍼 회복
        {
            AForgottenKnightCharacter::MyCurrentEXP += 50;
            AForgottenKnightCharacter::MyGold += 100;
            AForgottenKnightCharacter::MyCurrentHealth += round(float(AForgottenKnightCharacter::MyMaxHealth) * 0.3);
            if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
            {
                AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
            }
        }
        else if (clear == 2) // exp70, 골드150, 체력30퍼 회복
        {
            AForgottenKnightCharacter::MyCurrentEXP += 70;
            AForgottenKnightCharacter::MyGold += 150;
            AForgottenKnightCharacter::MyCurrentHealth += round(float(AForgottenKnightCharacter::MyMaxHealth) * 0.3);
            if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
            {
                AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
            }
        }
        else if (clear == 3) // exp100, 골드500, 체력50퍼 회복
        {
            AForgottenKnightCharacter::MyCurrentEXP += 100; // 미완성
            AForgottenKnightCharacter::MyGold += 500;
            AForgottenKnightCharacter::MyCurrentHealth += round(float(AForgottenKnightCharacter::MyMaxHealth) * 0.5);
            if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
            {
                AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
            }
        }
        else if (clear == 4) // exp200, 골드750, 체력100퍼 회복
        {
            AForgottenKnightCharacter::MyCurrentEXP += 200; // 미완성
            AForgottenKnightCharacter::MyGold += 750;
            AForgottenKnightCharacter::MyCurrentHealth += AForgottenKnightCharacter::MyMaxHealth;
        }

        bool x = SetLevelEXP();
        SetPause(true);
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
        UI_DungeonClear = CreateWidget<UUI_DungeonClear>(this, BP_Widget_DungeonClear);
        UI_DungeonClear->AddToViewport();
        UI_DungeonClear->ChangeStat(EXP, Gold, clear);
        if (x)
        {
            UI_DungeonClear->ShowLvUp();
        }
    }
}

void AController_MainCharacter::HideDungeonClearWidget()
{
    if (BP_Widget_DungeonClear != nullptr)
    {
        UI_DungeonClear->RemoveFromParent();
        UI_DungeonClear->Destruct();
        SetPause(false);
    }
}

void AController_MainCharacter::ShowEquipmentWidget()
{
    if (BP_Widget_Equipment != nullptr)
    {
        UI_Equipment = CreateWidget<UUI_Equipment>(this, BP_Widget_Equipment);
        UI_Equipment->AddToViewport();
    }
}

void AController_MainCharacter::HideEquipmentWidget()
{
    if (BP_Widget_Equipment != nullptr)
    {
        UI_Equipment->RemoveFromParent();
        UI_Equipment->Destruct();
    }
}

void AController_MainCharacter::ShowEvent3Widget(int Order)
{
    if (BP_Widget_Event_3 != nullptr)
    {
        CardDragable = false;
        bShowMouseCursor = true;
        UI_Event_3 = CreateWidget<UUI_Event_3>(this, BP_Widget_Event_3);
        UI_Event_3->EventOrder = Order;
        UI_Event_3->UpdateStat();
        UI_Event_3->AddToViewport();
    }
}

void AController_MainCharacter::HideEvent3Widget()
{
    if (BP_Widget_Event_3 != nullptr && UI_Event_3)
    {
        CardDragable = true;
        UI_Event_3->RemoveFromParent();
        UI_Event_3->Destruct();
        EventCode = { "" };
        OutsideChangeStat();
    }
}

void AController_MainCharacter::ShowEvent4Widget(int Order)
{
    if (BP_Widget_Event_4 != nullptr)
    {
        bShowMouseCursor = true;
        UI_Event_4 = CreateWidget<UUI_Event_4>(this, BP_Widget_Event_4);
        UI_Event_4->EventOrder = Order;
        UI_Event_4->UpdateStat();
        UI_Event_4->AddToViewport();
    }
}

void AController_MainCharacter::HideEvent4Widget()
{
    if (BP_Widget_Event_4 != nullptr && UI_Event_4)
    {
        UI_Event_4->RemoveFromParent();
        UI_Event_4->Destruct();
        EventCode = { "" };
        OutsideChangeStat();
    }
}

void AController_MainCharacter::ShowEvent4PopupWidget(int Order)
{
    if (BP_Widget_Event_4_Popup != nullptr)
    {
        bShowMouseCursor = true;
        UI_Event_4_Popup = CreateWidget<UUI_Event_4_Popup>(this, BP_Widget_Event_4_Popup);
        UI_Event_4_Popup->EventOrder = Order;
        UI_Event_4_Popup->AddToViewport();
    }
}

void AController_MainCharacter::HideEvent4PopupWidget()
{
    if (BP_Widget_Event_4_Popup != nullptr && UI_Event_4_Popup)
    {
        UI_Event_4_Popup->RemoveFromParent();
        UI_Event_4_Popup->Destruct();
        EventCode = { "" };
        OutsideChangeStat();
    }
}

int AController_MainCharacter::ShowBonusAttack()
{
    return UI_PlayWindow->ShowBonusAttack();
}

void AController_MainCharacter::LoseKingHP(int value)
{
    UI_PlayWindow->LoseKingHP(value);
}

void AController_MainCharacter::ShowScriptWidget(int purpose)
{
    if (BP_Widget_ShowScript != nullptr)
    {
        UI_ShowScript = CreateWidget<UUI_ShowScript>(this, BP_Widget_ShowScript);
        UI_ShowScript->whatpurpose = purpose;
        UI_ShowScript->AddToViewport();
    }
}

void AController_MainCharacter::HideScriptWidget()
{
    if (BP_Widget_ShowScript != nullptr)
    {
        int ksks = UI_ShowScript->whatpurpose;
        UI_ShowScript->RemoveFromParent();
        UI_ShowScript->Destruct();

        if (ksks == 2)
        {
            UI_PlayWindow->LastClear = true;
            UI_PlayWindow->StageClear();
        }
    }
}

void AController_MainCharacter::UpdateOutsideStat()
{
    if (BP_Widget_OutsideWindow != nullptr)
    {
        OutsideWidget->ChangeStat();
    }
}

void AController_MainCharacter::PlayUIMessage(int ui, int Order)
{
    if (ui == 13 && BP_Widget_Store1_3 != nullptr)
    {
        UI_Store1_3->PlayMessage(Order);
    }
    else if (ui == 23 && BP_Widget_Store2_3 != nullptr)
    {
        UI_Store2_3->PlayMessage(Order);
    }
    else if (ui == 43 && BP_Widget_Store4_3 != nullptr)
    {
        UI_Store4_3->PlayMessage(Order);
    }
}

void AController_MainCharacter::ShowTutorialWidget()
{
    if (BP_Widget_Tutorial != nullptr)
    {
        UI_Tutorial = CreateWidget<UUI_Tutorial>(this, BP_Widget_Tutorial);
        UI_Tutorial->AddToViewport();
    }
}

void AController_MainCharacter::HideTutorialWidget()
{
    if (BP_Widget_Tutorial != nullptr)
    {
        UI_Tutorial->RemoveFromParent();
        UI_Tutorial->Destruct();
    }
}

void AController_MainCharacter::FreezeStart()
{
    SetInputMode(FInputModeGameOnly()); // UI 입력 막기
    bShowMouseCursor = false;
}

void AController_MainCharacter::FreezeEnd()
{
    SetInputMode(FInputModeGameAndUI());
    bShowMouseCursor = true;
}

void AController_MainCharacter::CallAttack(int Order)
{
    if (BP_Widget_PlayWindow != nullptr)
    {
        AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(GetCharacter());
        if (MyPlayer != nullptr && MyPlayer->MyLife)
        {
            if (Order == 1)
            {
                if (UI_PlayWindow->EnemyCount == 0 && ((!UI_PlayWindow->LastStage) || (UI_PlayWindow->LastOrder == 4)))
                {
                    UI_PlayWindow->StageClear();
                }
                else
                {
                    UI_PlayWindow->Attack1Ended();
                }
            }
            else if (Order == 2)
            {
                if (UI_PlayWindow->EnemyCount == 0 && ((!UI_PlayWindow->LastStage) || (UI_PlayWindow->LastOrder == 4)))
                {
                    UI_PlayWindow->StageClear();
                }
                else
                {
                    UI_PlayWindow->Attack2Ended();
                }
            }
            else if (Order == 3)
            {
                if (UI_PlayWindow->EnemyCount == 0 && ((!UI_PlayWindow->LastStage) || (UI_PlayWindow->LastOrder == 4)))
                {
                    UI_PlayWindow->StageClear();
                }
                else
                {
                    UI_PlayWindow->Attack3Ended();
                }
            }
            else if (Order == -10)
            {
                if (UI_PlayWindow->EnemyCount == 0 && ((!UI_PlayWindow->LastStage) || (UI_PlayWindow->LastOrder == 4)))
                {
                    UI_PlayWindow->StageClear();
                }
                else
                {
                    UI_PlayWindow->TurnClear();
                }
            }
            else if (Order == -1)
            {
                if (UI_PlayWindow->CheckTurnClear())
                {
                    UI_PlayWindow->TurnClear();
                }
            }
        }
    }
}

void AController_MainCharacter::InteractOn()
{
    if (AController_MainCharacter::InteractKeyword == "Stage1_1")
    {
        ShowEnterStageWidget(1, 1);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage1_2")
    {
        ShowEnterStageWidget(1, 2);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage1_3")
    {
        ShowEnterStageWidget(1, 3);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage1_4")
    {
        ShowEnterStageWidget(1, 4);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage1_5")
    {
        ShowEnterStageWidget(1, 5);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage1_6")
    {
        ShowEnterStageWidget(1, 6);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage1_7")
    {
        ShowEnterStageWidget(1, 7);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage1_8")
    {
        ShowEnterStageWidget(1, 8);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage1_9")
    {
        ShowEnterStageWidget(1, 9);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage1_10")
    {
        ShowEnterStageWidget(1, 10);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage2_1")
    {
        ShowEnterStageWidget(2, 1);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage2_2")
    {
        ShowEnterStageWidget(2, 2);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage2_3")
    {
        ShowEnterStageWidget(2, 3);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage2_4")
    {
        ShowEnterStageWidget(2, 4);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage2_5")
    {
        ShowEnterStageWidget(2, 5);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage2_6")
    {
        ShowEnterStageWidget(2, 6);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage2_7")
    {
        ShowEnterStageWidget(2, 7);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage2_8")
    {
        ShowEnterStageWidget(2, 8);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage2_9")
    {
        ShowEnterStageWidget(2, 9);
    }
    else if (AController_MainCharacter::InteractKeyword == "Stage2_10")
    {
        ShowEnterStageWidget(2, 10);
    }
    else if (AController_MainCharacter::InteractKeyword == "Store1")
    {
        ShowStoreWidget(1, 1);
    }
    else if (AController_MainCharacter::InteractKeyword == "Store2")
    {
        ShowStoreWidget(2, 1);
    }
    else if (AController_MainCharacter::InteractKeyword == "Store3")
    {
        ShowStoreWidget(3, 1);
    }
    else if (AController_MainCharacter::InteractKeyword == "Store4")
    {
        ShowStoreWidget(4, 1);
    }
    else if (AController_MainCharacter::InteractKeyword == "EventHeal1")
    {
        ShowEventHealWidget(1);
        EventCode = { "Heal" };
    }
    else if (AController_MainCharacter::InteractKeyword == "EventHeal2")
    {
        ShowEventHealWidget(2);
        EventCode = { "Heal" };
    }
    else if (AController_MainCharacter::InteractKeyword == "EventHeal3")
    {
        ShowEventHealWidget(3);
        EventCode = { "Heal" };
    }
    else if (AController_MainCharacter::InteractKeyword == "EventHeal4")
    {
        ShowEventHealWidget(4);
        EventCode = { "Heal" };
    }
    else if (AController_MainCharacter::InteractKeyword == "EventHeal5")
    {
        ShowEventHealWidget(5);
        EventCode = { "Heal" };
    }
    else if (AController_MainCharacter::InteractKeyword == "EventFountain1")
    {
        ShowEventFountainWidget(1);
        EventCode = { "Fountain" };
    }
    else if (AController_MainCharacter::InteractKeyword == "EventFountain2")
    {
        ShowEventFountainWidget(2);
        EventCode = { "Fountain" };
    }
    else if (AController_MainCharacter::InteractKeyword == "EventFountain3")
    {
        ShowEventFountainWidget(3);
        EventCode = { "Fountain" };
    }
    else if (AController_MainCharacter::InteractKeyword == "EventDungeon1")
    {
        ShowEventDungeonWidget(1);
        EventCode = { "Dungeon" };
    }
    else if (AController_MainCharacter::InteractKeyword == "EventDungeon2")
    {
        ShowEventDungeonWidget(2);
        EventCode = { "Dungeon" };
    }
    else if (AController_MainCharacter::InteractKeyword == "EventDungeon3")
    {
        ShowEventDungeonWidget(3);
        EventCode = { "Dungeon" };
    }
    else if (AController_MainCharacter::InteractKeyword == "EventDungeon4")
    {
        ShowEventDungeonWidget(4);
        EventCode = { "Dungeon" };
    }
    else if (AController_MainCharacter::InteractKeyword == "Event3_1")
    {
        ShowEvent3Widget(1);
        EventCode = { "3" };
    }
    else if (AController_MainCharacter::InteractKeyword == "Event3_2")
    {
        ShowEvent3Widget(2);
        EventCode = { "3" };
    }
    else if (AController_MainCharacter::InteractKeyword == "Event4_1")
    {
        ShowEvent4Widget(1);
        EventCode = { "4" };
    }
    else if (AController_MainCharacter::InteractKeyword == "Event4_2")
    {
        ShowEvent4Widget(2);
        EventCode = { "4" };
    }
    else if (AController_MainCharacter::InteractKeyword == "GoChap2")
    {
        AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(GetCharacter());
        AForgottenKnightCharacter::MyStartTransform = MyPlayer->GetTransform();
        UGameplayStatics::OpenLevel(this, FName("OutsideMap2"));
    }
    else if (AController_MainCharacter::InteractKeyword == "GoChap1")
    {
        AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(GetCharacter());
        AForgottenKnightCharacter::MyStartTransform2 = MyPlayer->GetTransform();
        UGameplayStatics::OpenLevel(this, FName("OutsideMap"));
    }

}

void AController_MainCharacter::StoreChangeGold()
{
    if (StoreOrderNum == 1)
    {
        UI_Store1_3->ChangeGold();
    }
    else if (StoreOrderNum == 2)
    {
        UI_Store2_3->ChangeGold();
    }
    else if (StoreOrderNum == 4)
    {
        UI_Store4_3->ChangeGold();
    }
}

int AController_MainCharacter::ConfirmEnemyNum()
{
    int x = UI_PlayWindow->EnemyCount;

    return x;
}

void AController_MainCharacter::DragStarting(int CardNum, int FixCost)
{
    UI_PlayWindow->DragingCardNum = CardNum;
    UI_PlayWindow->DragingCardFixCost = FixCost;
}

void AController_MainCharacter::DetectEnterKey()
{
    if (UI_Dialogue != nullptr)
    {
        if (UI_Dialogue->DetectingEnter == true)
        {
            UI_Dialogue->ShowEntireText();
        }
        else
        {
            UI_Dialogue->DetectingEnter = true;
        }
    }

    if (UI_ShowScript != nullptr)
    {
        if (UI_ShowScript->DetectingEnter == true)
        {
            UI_ShowScript->ShowEntireText();
        }
        else
        {
            UI_ShowScript->DetectingEnter = true;
        }
    }
}

void AController_MainCharacter::ShowCardClickPopup(int CardNum, int CardOrder)
{
    if (BP_Widget_ClickDeck != nullptr)
    {
        UI_ClickDeck->ShowPopup(CardNum, CardOrder);
    }
}

void AController_MainCharacter::OutsideChangeStat()
{
    if (BP_Widget_OutsideWindow != nullptr)
    {
        OutsideWidget->ChangeStat();
    }
}

bool AController_MainCharacter::SetLevelEXP()
{
    int x = 0;
    while (AForgottenKnightCharacter::MyCurrentEXP >= AForgottenKnightCharacter::MyMaxEXP)
    {
        AForgottenKnightCharacter::MyCurrentEXP -= AForgottenKnightCharacter::MyMaxEXP;
        AForgottenKnightCharacter::MyMaxEXP += 20 * AForgottenKnightCharacter::MyLevel;
        AForgottenKnightCharacter::MyLevel += 1;
        AForgottenKnightCharacter::MyMaxHealth += 5;
        AForgottenKnightCharacter::MyCurrentHealth += 10;
        if (AForgottenKnightCharacter::MyCurrentHealth > AForgottenKnightCharacter::MyMaxHealth)
        {
            AForgottenKnightCharacter::MyCurrentHealth = AForgottenKnightCharacter::MyMaxHealth;
        }
        x += 1;
    }

    if (x == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void AController_MainCharacter::InputClickPressed()
{
    if (ClickMovable == true)
    {
        bClickMouse = true;
    }
}

void AController_MainCharacter::InputClickReleased()
{
    if (ClickMovable == true)
    {
        bClickMouse = false;
    }
}

void AController_MainCharacter::SetupInputComponent()
{
    Super::SetupInputComponent();

    //InputComponent->BindAction("Click_Left", IE_Pressed, this, &AController_MainCharacter::InputClickPressed);
    //InputComponent->BindAction("Click_Left", IE_Released, this, &AController_MainCharacter::InputClickReleased);
    InputComponent->BindAction("Inventory", IE_Pressed, this, &AController_MainCharacter::ShowInventoryWidget);
    InputComponent->BindAction("Interaction", IE_Pressed, this, &AController_MainCharacter::InteractOn);
    InputComponent->BindAction("EnterKey", IE_Pressed, this, &AController_MainCharacter::DetectEnterKey);
}

void AController_MainCharacter::SetNewDestination(const FVector DestLocation)
{
    APawn* const MyPawn = GetPawn();
    if (MyPawn)
    {
        float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

        if (Distance > 120.0f)
        {
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
        }
    }
}

void AController_MainCharacter::MoveToMouseCursor()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        SetNewDestination(Hit.ImpactPoint);
    }
}
