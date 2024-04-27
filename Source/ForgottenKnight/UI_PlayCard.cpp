// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayCard.h"
#include "DragWidget.h"
#include "Input/Events.h"
#include "InputCoreTypes.h"
#include "Styling/SlateBrush.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "Kismet/GameplayStatics.h"

void UUI_PlayCard::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UUI_PlayCard::InsertImage(int Num)
{
	MyCardNum = Num;

	if (Num > 100)
	{
		InsertImageUpgrade(Num);
		return;
	}
	if (CardImage != nullptr)
	{
		if (Num == 0)
		{
			CardImage->SetVisibility(ESlateVisibility::Collapsed);
			InCardCost = 0;
		}
		else if (Num == 1) // 공격 : 피해를 6 줍니다.
		{
			CardImage->SetBrush(Image1); // 해당 카드 이미지 삽입
			ImageNum = 1; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("공격"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 6 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 2) // 상처내기 : 피해를 1 주고 출혈을 4 부여합니다.
		{
			CardImage->SetBrush(Image2); // 해당 카드 이미지 삽입
			ImageNum = 2; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("상처내기"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 1 주고 <stat>출혈</>을 4 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 3) // 받아치기 : 피해를 4 줍니다. 패링 시 피해가 두 배가 됩니다
		{
			CardImage->SetBrush(Image3); // 해당 카드 이미지 삽입
			ImageNum = 3; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("받아치기"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 줍니다\n패링 시 피해가 두 배가 됩니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 4) // 되돌려주기 : 피해를 8 줍니다. 카운터 시에만 사용 가능
		{
			CardImage->SetBrush(Image4); // 해당 카드 이미지 삽입
			ImageNum = 4; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("되돌려주기"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("카운터 시에만 사용 가능\n피해를 8 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 5) // 천변의 검 : 선공 : 피해를 2 줍니다. 패링 : 피해를 4 주고, 적중 시 출혈을 2 부여합니다. 카운터 : 피해를 6 줍니다
		{
			CardImage->SetBrush(Image5); // 해당 카드 이미지 삽입
			ImageNum = 5; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("천변의 검"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("변화"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("선제공격:피해를 2 줍니다\n패링:피해를 4 주고, 적중 시 <stat>출혈</>을 2 부여합니다\n카운터:피해를 6 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 6) // 다재다능 : 선제공격:피해를 4 주고 약화를 2 부여합니다 패링:피해를 6 주고 부식을 2 부여합니다 카운터:피해를 6 주고 가한 피해의 25%만큼 회복합니다
		{
			CardImage->SetBrush(Image6); // 해당 카드 이미지 삽입
			ImageNum = 6; // 카드의 고유 인덱스
			InCardCost = 2; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("2");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("다재다능"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("변화"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("선제공격:피해를 4 주고 <stat>약화</>를 2 부여합니다\n패링:피해를 6 주고 <stat>부식</>을 2 부여합니다\n카운터:<stat>생명력흡수</>\n피해를 6 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 7) // 힘줄끊기 : 피해를 1 주고 약화를 2 부여합니다
		{
			CardImage->SetBrush(Image7); // 해당 카드 이미지 삽입
			ImageNum = 7; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("힘줄끊기"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 1 주고 <stat>약화</>를 2 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 8) // 흘리기 : 패링 시에만 사용가능 피해를 3 주고 막지 못한 피해를 최대 3 만큼 감소시킵니다
		{
			CardImage->SetBrush(Image8); // 해당 카드 이미지 삽입
			ImageNum = 8; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("흘리기"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("패링 시에만 사용가능\n피해를 3 주고 막지 못한 피해를 최대 3 만큼 감소시킵니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 9) // 감내 : 피해를 1 줍니다. 이번 전투에서 피해를 받을 때마다 이 카드의 피해량이 2 증가합니다
		{
			CardImage->SetBrush(Image9); // 해당 카드 이미지 삽입
			ImageNum = 9; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("감내"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 1 줍니다\n이번 전투에서 피해를 받을 때마다 이 카드의 피해량이 2 증가합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 10) // 진흙탕싸움 : 피해를 6 줍니다. 부식,약화,출혈을 2 씩 부여합니다.
		{
			CardImage->SetBrush(Image10); // 해당 카드 이미지 삽입
			ImageNum = 10; // 카드의 고유 인덱스
			InCardCost = 2; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("2");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("진흙탕싸움"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 6 줍니다\n<stat>부식 약화 출혈</>을 2 씩 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 11) // 철괴 : 이번 턴에 공격할 수 없습니다. 이번 턴에 피해를 받지 않습니다.
		{
			CardImage->SetBrush(Image11); // 해당 카드 이미지 삽입
			ImageNum = 11; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("철괴"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("이번 턴에 공격할 수 없습니다\n이번 턴에 피해를 받지 않습니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 12) // 피의 맛 : 피해를 15 줍니다 , 생흡
		{
			CardImage->SetBrush(Image12); // 해당 카드 이미지 삽입
			ImageNum = 12; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("피의 맛"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("생명력 흡수"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 15 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 13) // 삼중연쇄1 : 피해2, 다음 연쇄 피해 4 증가
		{
			CardImage->SetBrush(Image13); // 해당 카드 이미지 삽입
			ImageNum = 13; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("삼중연쇄1"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 2 줍니다\n선제공격 시 다음 <stat>연쇄</>의 피해가 4 증가합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 14) // 삼중연쇄2
		{
			CardImage->SetBrush(Image14); // 해당 카드 이미지 삽입
			ImageNum = 14; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("삼중연쇄2"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 줍니다\n패링 시 다음 <stat>연쇄</>에 <stat>생명력 흡수</>가 적용됩니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 15) // 삼중연쇄3
		{
			CardImage->SetBrush(Image15); // 해당 카드 이미지 삽입
			ImageNum = 15; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("삼중연쇄3"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("카운터시에만 사용가능\n피해를 6 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 16) // 잽잽훅1
		{
			CardImage->SetBrush(Image16); // 해당 카드 이미지 삽입
			ImageNum = 16; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("잽잽훅1"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 1 줍니다\n다음 <stat>연쇄</>에 모든 적에게 <stat>부식</>을 1 부여 합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 17) // 잽잽훅2
		{
			CardImage->SetBrush(Image17); // 해당 카드 이미지 삽입
			ImageNum = 17; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("잽잽훅2"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 2 줍니다\n다음 <stat>연쇄</>의 피해가 6 증가합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 18) // 잽잽훅3
		{
			CardImage->SetBrush(Image18); // 해당 카드 이미지 삽입
			ImageNum = 18; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("잽잽훅3"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 19) // 상처 벌리기
		{
			CardImage->SetBrush(Image19); // 해당 카드 이미지 삽입
			ImageNum = 19; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("상처 벌리기"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 2 주고 <stat>출혈</>을 2 부여합니다\n대상이 <stat>출혈</> 보유 시 추가로 <stat>출혈</>을 2 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 20) // 상처 벌리기
		{
			CardImage->SetBrush(Image20); // 해당 카드 이미지 삽입
			ImageNum = 20; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("유연한 강함"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("변화"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("선제공격:피해를 8 줍니다\n패링:피해를 6 줍니다\n카운터:피해를 4 주고 체력을 3 회복합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 21) // 준비만전
		{
			CardImage->SetBrush(Image21); // 해당 카드 이미지 삽입
			ImageNum = 21; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("준비만전"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 줍니다\n카드를 1장 뽑습니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 22) // 땅 파기
		{
			CardImage->SetBrush(Image22); // 해당 카드 이미지 삽입
			ImageNum = 22; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("땅 파기"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 2 줍니다\n버려진 카드더미에서 카드를 1장 가져옵니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 23) // 대기만성
		{
			CardImage->SetBrush(Image23); // 해당 카드 이미지 삽입
			ImageNum = 23; // 카드의 고유 인덱스
			InCardCost = 2; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("2");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("대기만성"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("다음 턴에 선택한 <stat>연쇄</>카드의 종류를 모두 손으로 가져옵니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 24) // 갑옷 부수기
		{
			CardImage->SetBrush(Image24); // 해당 카드 이미지 삽입
			ImageNum = 24; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("갑옷 부수기"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 줍니다\n<stat>부식</>을 2 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 25) // 삼위일체1
		{
			CardImage->SetBrush(Image25); // 해당 카드 이미지 삽입
			ImageNum = 25; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("삼위일체1"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속 연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 3 줍니다\n모든 적에게 <stat>출혈</>을 3 부여합니다\n다음 <stat>연쇄</>의 비용이 3 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 26) // 삼위일체2
		{
			CardImage->SetBrush(Image26); // 해당 카드 이미지 삽입
			ImageNum = 26; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("삼위일체2"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속 연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 3 줍니다\n모든 적에게 <stat>부식</>을 3 부여합니다\n다음 <stat>연쇄</>의 비용이 3 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 27) // 삼위일체3
		{
			CardImage->SetBrush(Image27); // 해당 카드 이미지 삽입
			ImageNum = 27; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("삼위일체3"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속 연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 3 줍니다\n모든 적에게 <stat>약화</>를 3 부여합니다\n다음 <stat>연쇄</>의 비용이 3 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 31) // 오의절멸1
		{
			CardImage->SetBrush(Image31); // 해당 카드 이미지 삽입
			ImageNum = 31; // 카드의 고유 인덱스
			InCardCost = 2; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("2");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("오의:절멸1"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속 연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 6 줍니다\n다음 <stat>연쇄</>의 비용이 1 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 32) // 연계기
		{
			CardImage->SetBrush(Image32); // 해당 카드 이미지 삽입
			ImageNum = 32; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("연계기"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 6 줍니다\n이 카드는 <stat>연쇄</>로 취급됩니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 33) // 명상
		{
			CardImage->SetBrush(Image33); // 해당 카드 이미지 삽입
			ImageNum = 33; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("명상"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("가지고 있는 모든 부정적인 효과를 제거합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 34) // 적절한 분배
		{
			CardImage->SetBrush(Image34); // 해당 카드 이미지 삽입
			ImageNum = 34; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("적절한 분배"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("변화"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("선제공격:피해를 1주고 <stat>출혈 약화 부식</>을 2씩 부여합니다\n패링:동일한 피해를 줍니다\n카운터:피해를 10 주고 자신에게 <stat>약화</>를 2 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 35) // 오의절멸4
		{
			CardImage->SetBrush(Image35); // 해당 카드 이미지 삽입
			ImageNum = 35; // 카드의 고유 인덱스
			InCardCost = 5; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("5");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("오의:절멸4"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("카운터에 사용할 수 없습니다\n선택한 적의 잃은 체력만큼 피해를 줍니다\n(최대30)"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 36) // 더티 파이트
		{
			CardImage->SetBrush(Image36); // 해당 카드 이미지 삽입
			ImageNum = 36; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("더티 파이트"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 1 줍니다\n대상이 보유한 상태이상마다 피해량이 2배 증가합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 37) // 무소유
		{
			CardImage->SetBrush(Image37); // 해당 카드 이미지 삽입
			ImageNum = 37; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("무소유"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("패의 모든 카드를 버리고 버린 수 만큼 카드를 뽑습니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 41) // 광란1
		{
			CardImage->SetBrush(Image41); // 해당 카드 이미지 삽입
			ImageNum = 41; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("광란1"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 줍니다. <stat>출혈</>을 2 부여합니다. 다음 <stat>연쇄</>의 피해가 2 증가합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 42) // 광란2
		{
			CardImage->SetBrush(Image42); // 해당 카드 이미지 삽입
			ImageNum = 42; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("광란2"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 2 줍니다. <stat>출혈</>을 4 부여합니다. 다음 <stat>연쇄</>에 <stat>출혈</>을 2 부여가 적용됩니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 43) // 광란2
		{
			CardImage->SetBrush(Image43); // 해당 카드 이미지 삽입
			ImageNum = 43; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("광란3"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 1 줍니다. <stat>출혈</>을 2 부여합니다. 다음 <stat>연쇄</>의 피해량이 2 증가하고 <stat>출혈</>을 2 부여가 적용됩니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 44) // 날 세우기
		{
			CardImage->SetBrush(Image44); // 해당 카드 이미지 삽입
			ImageNum = 44; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("날 세우기"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("다음 공격의 데미지가 2 증가합니다."));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 45) // 피의 축제
		{
			CardImage->SetBrush(Image45); // 해당 카드 이미지 삽입
			ImageNum = 45; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("피의 축제"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 6 줍니다. 선택한 적이 보유한 <stat>출혈</>만큼 모든 적에게 <stat>출혈</>을 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 46) // 내려베기
		{
			CardImage->SetBrush(Image46); // 해당 카드 이미지 삽입
			ImageNum = 46; // 카드의 고유 인덱스
			InCardCost = 2; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("2");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("내려베기"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 12 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 47) // 모아베기
		{
			CardImage->SetBrush(Image47); // 해당 카드 이미지 삽입
			ImageNum = 47; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("모아베기"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 16 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 48) // 견제
		{
			CardImage->SetBrush(Image48); // 해당 카드 이미지 삽입
			ImageNum = 48; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("견제"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 3 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 49) // 위기 모면
		{
			CardImage->SetBrush(Image49); // 해당 카드 이미지 삽입
			ImageNum = 49; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("위기 모면"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 8 줍니다. 적의 수만큼 비용이 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 50) // 오의 절멸2
		{
			CardImage->SetBrush(Image50); // 해당 카드 이미지 삽입
			ImageNum = 50; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("오의 : 절멸2"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속 연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 2 주고 손에 있는 카드 한 장의 비용을 이번 턴에 1로 만듭니다. 다음 <stat>연쇄</>의 비용이 1 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 51) // 오의 절멸3
		{
			CardImage->SetBrush(Image51); // 해당 카드 이미지 삽입
			ImageNum = 51; // 카드의 고유 인덱스
			InCardCost = 2; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("2");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("오의 : 절멸3"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속 연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("다음 <stat>연쇄</>공격의 피해가 모든 적에게 적용됩니다. 다음 <stat>연쇄</>의 비용이 1 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 52) // 전투 휴식
		{
			CardImage->SetBrush(Image52); // 해당 카드 이미지 삽입
			ImageNum = 52; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("전투 휴식"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("비용을 1 회복합니다. 카드를 1장 뽑습니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 53) // 상처 폭발
		{
			CardImage->SetBrush(Image53); // 해당 카드 이미지 삽입
			ImageNum = 53; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("상처 폭발"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("적이 보유한 <stat>출혈</>만큼 피해를 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 54) // 각성
		{
			CardImage->SetBrush(Image54); // 해당 카드 이미지 삽입
			ImageNum = 54; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("각성"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("손의 모든 카드를 강화합니다. 이번 전투동안 지속됩니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		// 특수 카드
		else if (Num == 81) // 착란
		{
			CardImage->SetBrush(Image81); // 해당 카드 이미지 삽입
			ImageNum = 81; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("착란"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("이번 전투동안 내가 보유한 모든 카드의 비용을 0~3사이의 무작위숫자로 변경합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 82) // 기본기 연마
		{
			CardImage->SetBrush(Image82); // 해당 카드 이미지 삽입
			ImageNum = 82; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("기본기 연마"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("이번 전투동안 카드 설명에 키워드가 포함돼있지 않은 모든 카드의 비용을 1 감소시킵니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 83) // 광기 발현
		{
			CardImage->SetBrush(Image83); // 해당 카드 이미지 삽입
			ImageNum = 83; // 카드의 고유 인덱스
			InCardCost = 2; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("2");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("광기 발현"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("이번 전투동안 모든 카드의 대미지를 1로 낮추고 줄어든 대미지의 절반만큼 <stat>출혈</>을 부여하는 효과를 카드에 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 84) // 연격의 대가
		{
			CardImage->SetBrush(Image84); // 해당 카드 이미지 삽입
			ImageNum = 84; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("연격의 대가"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("이번 전투동안 <stat>연쇄</>카드를 제외한 카드들의 비용을 1 증가시키고 뽑을 시 카드를 한장 드로우(턴당 최대 2회)하는 효과를 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}

	}
}

void UUI_PlayCard::InsertImageUpgrade(int Num)
{
	MyCardNum = Num;

	if (CardImage != nullptr)
	{
		if (Num == 101) // 공격 : 피해를 6 줍니다.
		{
			CardImage->SetBrush(Image1); // 해당 카드 이미지 삽입
			ImageNum = 101; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("공격+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 8 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 102) // 상처내기 : 피해를 1 주고 출혈을 4 부여합니다.
		{
			CardImage->SetBrush(Image2); // 해당 카드 이미지 삽입
			ImageNum = 102; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("상처내기+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 2 주고 <stat>출혈</>을 6 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 103) // 받아치기 : 피해를 4 줍니다. 패링 시 피해가 두 배가 됩니다
		{
			CardImage->SetBrush(Image3); // 해당 카드 이미지 삽입
			ImageNum = 103; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("받아치기+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 줍니다\n패링 시 피해가 3배가 됩니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 104) // 되돌려주기 : 피해를 8 줍니다. 카운터 시에만 사용 가능
		{
			CardImage->SetBrush(Image4); // 해당 카드 이미지 삽입
			ImageNum = 104; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("되돌려주기+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("카운터 시에만 사용 가능\n피해를 12 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 105) // 천변의 검 : 선공 : 피해를 2 줍니다. 패링 : 피해를 4 주고, 적중 시 출혈을 2 부여합니다. 카운터 : 피해를 6 줍니다
		{
			CardImage->SetBrush(Image5); // 해당 카드 이미지 삽입
			ImageNum = 105; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("천변의 검+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("변화"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("선제공격:피해를 3 줍니다\n패링:피해를 6 주고, 적중 시 <stat>출혈</>을 2 부여합니다\n카운터:피해를 8 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 106) // 다재다능 : 선제공격:피해를 4 주고 약화를 2 부여합니다 패링:피해를 6 주고 부식을 2 부여합니다 카운터:피해를 6 주고 가한 피해의 25%만큼 회복합니다
		{
			CardImage->SetBrush(Image6); // 해당 카드 이미지 삽입
			ImageNum = 106; // 카드의 고유 인덱스
			InCardCost = 2; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("2");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("다재다능+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("변화"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("선제공격:피해를 5 주고 <stat>약화</>를 3 부여합니다\n패링:피해를 8 주고 <stat>부식</>을 3 부여합니다\n카운터:<stat>생명력흡수</>\n피해를 8 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 107) // 힘줄끊기 : 피해를 1 주고 약화를 2 부여합니다
		{
			CardImage->SetBrush(Image7); // 해당 카드 이미지 삽입
			ImageNum = 107; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("힘줄끊기+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 2 주고 <stat>약화</>를 2 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 108) // 흘리기 : 패링 시에만 사용가능 피해를 3 주고 막지 못한 피해를 최대 3 만큼 감소시킵니다
		{
			CardImage->SetBrush(Image8); // 해당 카드 이미지 삽입
			ImageNum = 108; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("흘리기+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("패링 시에만 사용가능\n피해를 4 주고 막지 못한 피해를 최대 4 만큼 감소시킵니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 109) // 감내 : 피해를 1 줍니다. 이번 전투에서 피해를 받을 때마다 이 카드의 피해량이 2 증가합니다
		{
			CardImage->SetBrush(Image9); // 해당 카드 이미지 삽입
			ImageNum = 109; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("감내+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 1 줍니다\n이번 전투에서 피해를 받을 때마다 이 카드의 피해량이 3 증가합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 110) // 진흙탕싸움 : 피해를 6 줍니다. 부식,약화,출혈을 2 씩 부여합니다.
		{
			CardImage->SetBrush(Image10); // 해당 카드 이미지 삽입
			ImageNum = 110; // 카드의 고유 인덱스
			InCardCost = 2; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("2");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("진흙탕싸움+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 9 줍니다\n<stat>부식 약화 출혈</>을 2 씩 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 111) // 철괴 : 이번 턴에 공격할 수 없습니다. 이번 턴에 피해를 받지 않습니다.
		{
			CardImage->SetBrush(Image11); // 해당 카드 이미지 삽입
			ImageNum = 111; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("철괴+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("이번 턴에 공격할 수 없습니다\n이번 턴에 피해를 받지 않습니다\n체력을 10 회복합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 112) // 피의 맛 : 피해를 15 줍니다 , 생흡
		{
			CardImage->SetBrush(Image12); // 해당 카드 이미지 삽입
			ImageNum = 112; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("피의 맛+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("생명력 흡수"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 20 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 113) // 삼중연쇄1 : 피해2, 다음 연쇄 피해 4 증가
		{
			CardImage->SetBrush(Image13); // 해당 카드 이미지 삽입
			ImageNum = 113; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("삼중연쇄1+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 3 줍니다\n선제공격 시 다음 <stat>연쇄</>의 피해가 5 증가합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 114) // 삼중연쇄2
		{
			CardImage->SetBrush(Image14); // 해당 카드 이미지 삽입
			ImageNum = 114; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("삼중연쇄2+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 6 줍니다\n패링 시 다음 <stat>연쇄</>에 <stat>생명력 흡수</>가 적용됩니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 115) // 삼중연쇄3
		{
			CardImage->SetBrush(Image15); // 해당 카드 이미지 삽입
			ImageNum = 115; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("삼중연쇄3+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("카운터시에만 사용가능\n피해를 8 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 116) // 잽잽훅1
		{
			CardImage->SetBrush(Image16); // 해당 카드 이미지 삽입
			ImageNum = 116; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("잽잽훅1+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 2 줍니다\n다음 <stat>연쇄</>에 모든 적에게 <stat>부식</>을 1 부여 합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 117) // 잽잽훅2
		{
			CardImage->SetBrush(Image17); // 해당 카드 이미지 삽입
			ImageNum = 117; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("잽잽훅2+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 2 줍니다\n다음 <stat>연쇄</>의 피해가 8 증가합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 118) // 잽잽훅3
		{
			CardImage->SetBrush(Image18); // 해당 카드 이미지 삽입
			ImageNum = 118; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("잽잽훅3+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 6 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 119) // 상처 벌리기
		{
			CardImage->SetBrush(Image19); // 해당 카드 이미지 삽입
			ImageNum = 119; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("상처 벌리기+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 3 주고 <stat>출혈</>을 3 부여합니다\n대상이 <stat>출혈</> 보유 시 추가로 <stat>출혈</>을 3 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 120) // 상처 벌리기
		{
			CardImage->SetBrush(Image20); // 해당 카드 이미지 삽입
			ImageNum = 120; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("유연한 강함+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("변화"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("선제공격:피해를 10 줍니다\n패링:피해를 8 줍니다\n카운터:피해를 6 주고 체력을 3 회복합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 121) // 준비만전
		{
			CardImage->SetBrush(Image21); // 해당 카드 이미지 삽입
			ImageNum = 121; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("준비만전+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 줍니다\n카드를 2장 뽑습니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 122) // 땅 파기
		{
			CardImage->SetBrush(Image22); // 해당 카드 이미지 삽입
			ImageNum = 122; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("땅 파기+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 2 줍니다\n버려진 카드더미에서 카드를 1장 가져옵니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 123) // 대기만성
		{
			CardImage->SetBrush(Image23); // 해당 카드 이미지 삽입
			ImageNum = 123; // 카드의 고유 인덱스
			InCardCost = 2; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("2");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("대기만성+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("다음 턴에 선택한 <stat>연쇄</>카드의 종류를 모두 손으로 가져옵니다\n비용을 1 회복합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 124) // 갑옷 부수기
		{
			CardImage->SetBrush(Image24); // 해당 카드 이미지 삽입
			ImageNum = 124; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("갑옷 부수기+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 5 줍니다\n<stat>부식</>을 3 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 125) // 삼위일체1
		{
			CardImage->SetBrush(Image25); // 해당 카드 이미지 삽입
			ImageNum = 125; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("삼위일체1+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속 연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 줍니다\n모든 적에게 <stat>출혈</>을 4 부여합니다\n다음 <stat>연쇄</>의 비용이 3 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 126) // 삼위일체2
		{
			CardImage->SetBrush(Image26); // 해당 카드 이미지 삽입
			ImageNum = 126; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("삼위일체2+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속 연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 줍니다\n모든 적에게 <stat>부식</>을 4 부여합니다\n다음 <stat>연쇄</>의 비용이 3 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 127) // 삼위일체3
		{
			CardImage->SetBrush(Image27); // 해당 카드 이미지 삽입
			ImageNum = 127; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("삼위일체3+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속 연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 줍니다\n모든 적에게 <stat>약화</>를 4 부여합니다\n다음 <stat>연쇄</>의 비용이 3 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 131) // 오의절멸1
		{
			CardImage->SetBrush(Image31); // 해당 카드 이미지 삽입
			ImageNum = 131; // 카드의 고유 인덱스
			InCardCost = 2; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("2");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("오의:절멸1+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 8 줍니다\n다음 <stat>연쇄</>의 비용이 1 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 132) // 연계기
		{
			CardImage->SetBrush(Image32); // 해당 카드 이미지 삽입
			ImageNum = 132; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("연계기+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 6 줍니다\n이 카드는 <stat>연쇄</>로 취급됩니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 133) // 명상
		{
			CardImage->SetBrush(Image33); // 해당 카드 이미지 삽입
			ImageNum = 133; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("명상+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("가지고 있는 모든 부정적인 효과를 제거합니다\n카드를 1장 뽑습니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 134) // 적절한 분배
		{
			CardImage->SetBrush(Image34); // 해당 카드 이미지 삽입
			ImageNum = 134; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("적절한 분배+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("변화"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("선제공격:피해를 2주고 <stat>출혈 약화 부식</>을 3씩 부여합니다\n패링:상대보다 피해를 4 더 줍니다\n카운터:피해를 12 주고 자신에게 <stat>약화</>를 1 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 135) // 오의절멸4
		{
			CardImage->SetBrush(Image35); // 해당 카드 이미지 삽입
			ImageNum = 135; // 카드의 고유 인덱스
			InCardCost = 5; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("5");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("오의:절멸4+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("카운터에 사용할 수 없습니다\n선택한 적의 잃은 체력+10만큼 피해를 줍니다\n(최대50)"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 136) // 더티 파이트
		{
			CardImage->SetBrush(Image36); // 해당 카드 이미지 삽입
			ImageNum = 136; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("더티 파이트+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 1 줍니다\n대상이 보유한 상태이상마다 피해량이 3배 증가합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 137) // 무소유
		{
			CardImage->SetBrush(Image37); // 해당 카드 이미지 삽입
			ImageNum = 137; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("무소유+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("패의 모든 카드를 버리고 버린 수 만큼 카드를 뽑습니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 141) // 광란1
		{
			CardImage->SetBrush(Image41); // 해당 카드 이미지 삽입
			ImageNum = 141; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("광란1+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 줍니다. <stat>출혈</>을 4 부여합니다. 다음 <stat>연쇄</>의 피해가 2 증가합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 142) // 광란2
		{
			CardImage->SetBrush(Image42); // 해당 카드 이미지 삽입
			ImageNum = 142; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("광란2+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 3 줍니다. <stat>출혈</>을 6 부여합니다. 다음 <stat>연쇄</>에 <stat>출혈</>을 2 부여가 적용됩니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 143) // 광란3
		{
			CardImage->SetBrush(Image43); // 해당 카드 이미지 삽입
			ImageNum = 143; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("광란3+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 2 줍니다. <stat>출혈</>을 4 부여합니다. 다음 <stat>연쇄</>의 피해량이 2 증가하고 <stat>출혈</>을 2 부여가 적용됩니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 144) // 날 세우기
		{
			CardImage->SetBrush(Image44); // 해당 카드 이미지 삽입
			ImageNum = 144; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("날 세우기+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("다음 공격의 데미지가 3 증가합니다."));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 145) // 피의 축제
		{
			CardImage->SetBrush(Image45); // 해당 카드 이미지 삽입
			ImageNum = 145; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("피의 축제+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 9 줍니다. 선택한 적이 보유한 <stat>출혈</>만큼 모든 적에게 <stat>출혈</>을 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 146) // 내려베기
		{
			CardImage->SetBrush(Image46); // 해당 카드 이미지 삽입
			ImageNum = 146; // 카드의 고유 인덱스
			InCardCost = 2; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("2");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("내려베기+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 16 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 147) // 모아베기
		{
			CardImage->SetBrush(Image47); // 해당 카드 이미지 삽입
			ImageNum = 147; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("모아베기+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 24 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 148) // 견제
		{
			CardImage->SetBrush(Image48); // 해당 카드 이미지 삽입
			ImageNum = 148; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("견제+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 3 줍니다. <stat>부식</>을 2 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 149) // 위기 모면
		{
			CardImage->SetBrush(Image49); // 해당 카드 이미지 삽입
			ImageNum = 149; // 카드의 고유 인덱스
			InCardCost = 3; // 카드의 코스트
			KeyWordin = false; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("3");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("위기 모면+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 10 줍니다. 적의 수만큼 비용이 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 150) // 오의 절멸2
		{
			CardImage->SetBrush(Image50); // 해당 카드 이미지 삽입
			ImageNum = 150; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("오의 : 절멸2+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("피해를 4 주고 손에 있는 카드 한 장의 비용을 이번 턴에 1로 만듭니다. 다음 <stat>연쇄</>의 비용이 1 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 151) // 오의 절멸3
		{
			CardImage->SetBrush(Image51); // 해당 카드 이미지 삽입
			ImageNum = 151; // 카드의 고유 인덱스
			InCardCost = 1; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부
			ComboCard = true;

			FText InWText = FText::FromString("1");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("오의 : 절멸3+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("연쇄 신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("다음 <stat>연쇄</>공격의 피해가 모든 적에게 적용됩니다. 다음 <stat>연쇄</>의 비용이 1 감소합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 152) // 전투 휴식
		{
			CardImage->SetBrush(Image52); // 해당 카드 이미지 삽입
			ImageNum = 152; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("전투 휴식+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("비용을 2 회복합니다. 카드를 1장 뽑습니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 153) // 상처 폭발
		{
			CardImage->SetBrush(Image53); // 해당 카드 이미지 삽입
			ImageNum = 153; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("상처 폭발+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT(""));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("적이 보유한 <stat>출혈</>만큼 피해를 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (Num == 154) // 각성
		{
			CardImage->SetBrush(Image54); // 해당 카드 이미지 삽입
			ImageNum = 154; // 카드의 고유 인덱스
			InCardCost = 0; // 카드의 코스트
			KeyWordin = true; // 카드 설명에 키워드 포함 여부

			FText InWText = FText::FromString("0");
			CardCost->SetText(InWText);
			FString sStr = FString::Printf(TEXT("각성+"));
			CardName->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("신속"));
			CardSpecial->SetText(FText::FromString(sStr));
			sStr = FString::Printf(TEXT("손의 모든 카드를 강화합니다. 이번 전투동안 지속됩니다"));
			CardText->SetText(FText::FromString(sStr));
		}

	}
}

void UUI_PlayCard::MarkChangeCost(int value, bool beGrinding)
{
	FString NewWString;

	if (beGrinding)
	{
		value -= 1;
	}

	if (InCardCost + value >= 0)
	{
		NewWString = FString::FromInt(InCardCost + value);
	}
	else
	{
		NewWString = FString::FromInt(0);
	}
	FText InWText = FText::FromString(NewWString);
	CardCost->SetText(InWText);
	if (value == 0)
	{
		CardCost->SetColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 1.f)); // 글색 검정
	}
	else if (value < 0)
	{
		CardCost->SetColorAndOpacity(FLinearColor(0.f, 0.f, 1.f, 1.f)); // 글색 파랗게
	}
	else
	{
		CardCost->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f)); // 글색 빨갛게
	}


	if (FixCost >= 0) // 코스트가 픽스된 상태면
	{
		int fixco = FixCost;
		if (beGrinding && fixco > 0)
		{
			fixco -= 1;
		}

		NewWString = FString::FromInt(fixco);
		InWText = FText::FromString(NewWString);
		CardCost->SetText(InWText);
		if (InCardCost == fixco)
		{
			CardCost->SetColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 1.f)); // 글색 검정
		}
		else if (InCardCost > fixco)
		{
			CardCost->SetColorAndOpacity(FLinearColor(0.f, 0.f, 1.f, 1.f)); // 글색 파랗게
		}
		else
		{
			CardCost->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f)); // 글색 빨갛게
		}
	}
}

void UUI_PlayCard::ChangeTurnText(int turn) // 턴에 따라 변화 카드 텍스트 바뀌는 매커니즘 함수
{
	if (ImageNum == 5) // 천변의 검 : 선공 : 피해를 2 줍니다. 패링 : 피해를 4 주고, 적중 시 출혈을 2 부여합니다. 카운터 : 피해를 6 줍니다
	{
		FString sStr;
		if (turn == 0)
		{
			sStr = FString::Printf(TEXT("선제공격:피해를 2 줍니다\n패링:피해를 4 주고, 적중 시 <stat>출혈</>을 2 부여합니다\n카운터:피해를 6 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 1)
		{
			sStr = FString::Printf(TEXT("선제공격:피해를 2 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 2)
		{
			sStr = FString::Printf(TEXT("패링:피해를 4 주고, 적중 시 <stat>출혈</>을 2 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 3)
		{
			sStr = FString::Printf(TEXT("카운터:피해를 6 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
	}
	else if (ImageNum == 6) // 다재다능 : 선제공격:피해를 4 주고 약화를 2 부여합니다 패링:피해를 6 주고 부식을 2 부여합니다 카운터:피해를 6 주고 가한 피해의 25%만큼 회복합니다
	{
		FString sStr;
		if (turn == 0)
		{
			sStr = FString::Printf(TEXT("선제공격:피해를 4 주고 <stat>약화</>를 2 부여합니다\n패링:피해를 6 주고 <stat>부식</>을 2 부여합니다\n카운터:<stat>생명력흡수</>\n피해를 6 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 1)
		{
			sStr = FString::Printf(TEXT("선제공격:피해를 4 주고 <stat>약화</>를 2 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 2)
		{
			sStr = FString::Printf(TEXT("패링:피해를 6 주고 <stat>부식</>을 2 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 3)
		{
			sStr = FString::Printf(TEXT("카운터:<stat>생명력흡수</>\n피해를 6 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
	}
	else if (ImageNum == 34) // 적절한 분배
	{
		FString sStr;
		if (turn == 0)
		{
			sStr = FString::Printf(TEXT("선제공격:피해를 1주고 <stat>출혈 약화 부식</>을 2씩 부여합니다\n패링:동일한 피해를 줍니다\n카운터:피해를 10 주고 자신에게 <stat>약화</>를 2 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 1)
		{
			sStr = FString::Printf(TEXT("선제공격:피해를 1주고 <stat>출혈 약화 부식</>을 2씩 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 2)
		{
			sStr = FString::Printf(TEXT("패링:상대의 공격과 동일한 피해를 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 3)
		{
			sStr = FString::Printf(TEXT("카운터:피해를 10 주고 자신에게 <stat>약화</>를 2 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
	}

	if (ImageNum == 105) // 천변의 검 : 선공 : 피해를 2 줍니다. 패링 : 피해를 4 주고, 적중 시 출혈을 2 부여합니다. 카운터 : 피해를 6 줍니다
	{
		FString sStr;
		if (turn == 0)
		{
			sStr = FString::Printf(TEXT("선제공격:피해를 3 줍니다\n패링:피해를 6 주고, 적중 시 <stat>출혈</>을 2 부여합니다\n카운터:피해를 8 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 1)
		{
			sStr = FString::Printf(TEXT("선제공격:피해를 3 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 2)
		{
			sStr = FString::Printf(TEXT("패링:피해를 6 주고, 적중 시 <stat>출혈</>을 2 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 3)
		{
			sStr = FString::Printf(TEXT("카운터:피해를 8 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
	}
	else if (ImageNum == 106) // 다재다능 : 선제공격:피해를 4 주고 약화를 2 부여합니다 패링:피해를 6 주고 부식을 2 부여합니다 카운터:피해를 6 주고 가한 피해의 25%만큼 회복합니다
	{
		FString sStr;
		if (turn == 0)
		{
			sStr = FString::Printf(TEXT("선제공격:피해를 5 주고 <stat>약화</>를 3 부여합니다\n패링:피해를 8 주고 <stat>부식</>을 3 부여합니다\n카운터:<stat>생명력흡수</>\n피해를 8 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 1)
		{
			sStr = FString::Printf(TEXT("선제공격:피해를 5 주고 <stat>약화</>를 3 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 2)
		{
			sStr = FString::Printf(TEXT("패링:피해를 8 주고 <stat>부식</>을 3 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 3)
		{
			sStr = FString::Printf(TEXT("카운터:<stat>생명력흡수</>\n피해를 8 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
	}
	else if (ImageNum == 134) // 적절한 분배
	{
		FString sStr;
		if (turn == 0)
		{
			sStr = FString::Printf(TEXT("선제공격:피해를 2주고 <stat>출혈 약화 부식</>을 3씩 부여합니다\n패링:상대보다 피해를 4 더 줍니다\n카운터:피해를 12 주고 자신에게 <stat>약화</>를 1 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 1)
		{
			sStr = FString::Printf(TEXT("선제공격:피해를 2주고 <stat>출혈 약화 부식</>을 3씩 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 2)
		{
			sStr = FString::Printf(TEXT("패링:상대보다 피해를 4 더 줍니다"));
			CardText->SetText(FText::FromString(sStr));
		}
		else if (turn == 3)
		{
			sStr = FString::Printf(TEXT("카운터:피해를 12 주고 자신에게 <stat>약화</>를 1 부여합니다"));
			CardText->SetText(FText::FromString(sStr));
		}
	}
}

FReply UUI_PlayCard::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (MyPlayer != nullptr && PlayerController != nullptr)
	{
		if (PlayerController->CardClickable)
		{
			PlayerController->ShowCardClickPopup(ImageNum, MyOrder);
		}
		else if (PlayerController->CardHandPopupClickable)
		{
			PlayerController->HideHandDeckWidget(MyCardNum);
		}
		else if (PlayerController->CardDumpPopupClickable)
		{
			PlayerController->HideDumpDeckWidget(MyCardNum);
		}
		else if (PlayerController->CardChronicityDeckPopupClickable)
		{
			PlayerController->HideChronicityDeckWidget(MyCardNum);
		}
		else if (PlayerController->CardDragable)
		{
			reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
	}

	return reply.NativeReply;
}

void UUI_PlayCard::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragWidget* DragDropOperation = NewObject<UDragWidget>();
	//this->SetVisibility(ESlateVisibility::HitTestInvisible);

	SetRenderScale(FVector2D(1.0f, 1.0f));
	SetRenderTranslation(FVector2D(0, 0));

	DragDropOperation->WidgetReference = this;
	DragDropOperation->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	DragDropOperation->DefaultDragVisual = this;
	DragDropOperation->Pivot = EDragPivot::CenterCenter;
	DragDropOperation->EnemyTag = FString::FromInt(ImageNum); // 태그에 카드 인덱스 정보 넣기
	DragDropOperation->CostTag = FString::FromInt(InCardCost); // 태그에 카드 코스트 정보 넣기
	DragDropOperation->FixCostTag = FString::FromInt(FixCost); // 고정된 비용 정보 넣기
	DragDropOperation->IsComboCard = ComboCard;

	int keywordin1 = 0;
	if (KeyWordin)
	{
		keywordin1 = 1;
	}

	DragDropOperation->KeyWordinTag = FString::FromInt(keywordin1); // 감소된 비용 정보 넣기

	OutOperation = DragDropOperation;

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
	MyController->DragStarting(ImageNum, FixCost);
	RemoveFromParent();
}

void UUI_PlayCard::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		if (PlayerController->CardDragable)
		{
			SetRenderScale(FVector2D(1.5f, 1.5f));
			SetRenderTranslation(FVector2D(0, -150.f));
		}
	}
}

void UUI_PlayCard::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		if (PlayerController->CardDragable)
		{
			SetRenderScale(FVector2D(1.0f, 1.0f));
			SetRenderTranslation(FVector2D(0, 0));
		}
	}
}
