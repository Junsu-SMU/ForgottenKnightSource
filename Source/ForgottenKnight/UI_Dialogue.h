// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Dialogue.generated.h"

/**
 * 
 */
UCLASS()
class FORGOTTENKNIGHT_API UUI_Dialogue : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	virtual void NativeOnInitialized() override;
	
	void ShowText();
	void ShowEntireText();
	void ShowAllDial();
	void TakeDialogueData(int DataNum);

	float TypeSpeed = 0.05; // 타이핑 속도
	float delayrating = 0;
	FString InputString = {""}; // 전체 문장
	FString DisPlayedString = {""}; // 보이는 문장
	FString Teller = {""}; // 화자 이름
	int DialLength = 0; // 전체 다이얼 길이
	int TextIndex = 0; // 문자 인덱스
	int DialIndex = 0; // 문장 인덱스
	int StartIndex = 0; // 스타트 인덱스
	bool DetectingEnter = true; // 엔터 키를 쳤는지 감지
	bool DialEnd = false;
	bool FirstDataLoad = true; // 처음만 변수 받기

	int FlowIndex = 0; // 흐름 인덱스

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Name;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Talk;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Enter;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Anim_ShowEnd;
	
};
