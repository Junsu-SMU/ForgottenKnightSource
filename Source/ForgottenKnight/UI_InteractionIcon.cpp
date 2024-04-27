// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_InteractionIcon.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "EngineUtils.h"

void UUI_InteractionIcon::PlayOn()
{
	PlayAnimation(Anim_On);
}

void UUI_InteractionIcon::PlayOff()
{
	PlayAnimation(Anim_Off);
}
