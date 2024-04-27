// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_PlayCard.generated.h"

/**
 *
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_PlayCard : public UUserWidget
{
	GENERATED_BODY()

protected:

public:

	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	void InsertImage(int num);
	void InsertImageUpgrade(int num);
	void MarkChangeCost(int value, bool beGrinding);
	void ChangeTurnText(int turn);

	int ImageNum = 0;
	int MyCardNum = 0;
	int InCardCost = 0;
	int MyOrder = 0; // 나의 순서, 카드 뷰 순서나, 핸드 순서를 나타냄.
	int FixCost = -1; // 고정된 코스트
	bool ComboCard = false; // 이 카드가 콤보카드인지

	bool KeyWordin = false;

	UPROPERTY(meta = (BindWidget))
		class UImage* CardImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CardName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CardCost;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CardSpecial;

	UPROPERTY(meta = (BindWidget))
		class URichTextBlock* CardText;

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

};
