// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MobPopup.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "UI_PlayWindow.h"
#include "EnemyHealthBar.h"
#include "MyHealthBar.h"

void UUI_MobPopup::SetMessage(FString MobName)
{

	if (MobName == "Slime")
	{
		FString sStr = FString::Printf(TEXT("슬라임"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 2 - 4\n*적중 시 부식 부여\n*적중 시 약화 부여\n\n\n[ 보상 ]\nEXP : 2 / Gold : 5"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Goblin")
	{
		FString sStr = FString::Printf(TEXT("고블린"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 3 - 6\n*적중 시 출혈 부여\n\n\n\n[ 보상 ]\nEXP : 4 / Gold : 7"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Troll")
	{
		FString sStr = FString::Printf(TEXT("트롤"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 6\n\n\n\n\n[ 보상 ]\nEXP : 6 / Gold : 12"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Spider")
	{
		FString sStr = FString::Printf(TEXT("거미 무리"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 3 - 9\n*약화 부여\n*남은 체력에 따라\n위력 감소\n\n[ 보상 ]\nEXP : 6 / Gold : 10"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Mantis")
	{
		FString sStr = FString::Printf(TEXT("대형 사마귀"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 1 - 15\n*출혈 부여\n*내 출혈량에 따라\n특수 공격 시전\n\n[ 보상 ]\nEXP : 8 / Gold : 14"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Golem")
	{
		FString sStr = FString::Printf(TEXT("골렘"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 0 - 10\n*공격 후 1턴 휴식\n\n\n\n[ 보상 ]\nEXP : 15 / Gold : 10"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Asura")
	{
		FString sStr = FString::Printf(TEXT("아수라"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 0 - 14\n*모든 디버프 부여\n*숨 고르기 사용\n*무언가 숨기고 있다\n\n[ 보상 ]\nEXP : 30 / Gold : 50"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "ShadowSpear")
	{
		FString sStr = FString::Printf(TEXT("그림자 창병"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 10 - 12\n*적중 시 출혈 부여\n*적중 시 부식 부여\n\n\n[ 보상 ]\nEXP : 10 / Gold : 15"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "ShadowSheild")
	{
		FString sStr = FString::Printf(TEXT("그림자 방패병"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 10\n*공격 후 체력3 회복\n*피해를 받은 후\n다른 아군 힘1 증가\n\n[ 보상 ]\nEXP : 10 / Gold : 15"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "ShadowAxe")
	{
		FString sStr = FString::Printf(TEXT("그림자 도끼병"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 0 - 15\n*숨 고르기 사용\n*혼자 남으면\n위력 증가\n\n[ 보상 ]\nEXP : 10 / Gold : 15"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Griffon")
	{
		FString sStr = FString::Printf(TEXT("그리폰"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 8 - 15\n*출혈 부여\n*약화 부여\n\n\n[ 보상 ]\nEXP : 15 / Gold : 30"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Rafflesia")
	{
		FString sStr = FString::Printf(TEXT("라플레시아"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 5\n*부식 부여\n*약화 부여\n*향기상태 가능\n\n[ 보상 ]\nEXP : 12 / Gold : 25"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Warlock")
	{
		FString sStr = FString::Printf(TEXT("흑마법사"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n하수인 소환\n*피해 무시\n*무언가 숨기고 있다\n\n\n[ 보상 ]\nEXP : 50 / Gold : 150"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "SkeletonSoldier")
	{
		FString sStr = FString::Printf(TEXT("해골 병사"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 8 - 15\n*부식 부여\n*약화 부여\n\n\n[ 보상 ]\nEXP : 25 / Gold : 40"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "SkeletonArcher")
	{
		FString sStr = FString::Printf(TEXT("해골 궁수"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 6 - 10\n*출혈 부여\n*이중 타격 가능\n\n\n[ 보상 ]\nEXP : 25 / Gold : 40"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "SkeletonGuardDog")
	{
		FString sStr = FString::Printf(TEXT("해골 경비견"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 15 - 30\n*모든 디버프 부여\n*특수 행동 존재\n\n\n[ 보상 ]\nEXP : 25 / Gold : 40"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Lich")
	{
		FString sStr = FString::Printf(TEXT("리치"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 10\n*모든 디버프 부여\n*해골 몬스터 부활\n\n\n[ 보상 ]\nEXP : 50 / Gold : 100"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Beholder")
	{
		FString sStr = FString::Printf(TEXT("비홀더"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 10 - 20\n*매혹 부여\n*특수 상태를 지님\n\n\n[ 보상 ]\nEXP : 70 / Gold : 130"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "FallenFairy")
	{
		FString sStr = FString::Printf(TEXT("타락한 요정"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n*모든 디버프 부여\n*고정된 패턴\n*모든 디버프 면역\n*받는 피해 70%% 감소\n\n[ 보상 ]\nEXP : 50 / Gold : 150"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Baphomet")
	{
		FString sStr = FString::Printf(TEXT("바포메트"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 0 - 20\n*적중 시 출혈 부여\n*위력이 점점 강해짐\n*특수 행동 존재\n\n[ 보상 ]\nEXP : 80 / Gold : 250"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "Durahan")
	{
		FString sStr = FString::Printf(TEXT("듀라한"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n공격력 : 5 - 30\n*힘 겨루기 상태 존재\n*다른 적 위력 상승\n\n\n[ 보상 ]\nEXP : 100 / Gold : 300"));
		Text_Message->SetText(FText::FromString(sStr));
	}
	else if (MobName == "King")
	{
		FString sStr = FString::Printf(TEXT("왕"));
		Text_Name->SetText(FText::FromString(sStr));
		sStr = FString::Printf(TEXT("[ 공격 패턴 ]\n다양한 패턴 존재\n\n\n\n\n[ 보상 ]\nEXP : 500 / Gold : 1000"));
		Text_Message->SetText(FText::FromString(sStr));
	}

}
