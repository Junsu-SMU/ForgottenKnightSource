// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Inventory.h"
#include "Components/Button.h"
#include "Controller_MainCharacter.h"
#include "ForgottenKnightCharacter.h"

void UUI_Inventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ButtonCards != nullptr)
	{
		ButtonCards->OnClicked.AddDynamic(this, &UUI_Inventory::OnButtonCardsClicked);
	}

	if (ButtonEquipment != nullptr)
	{
		ButtonEquipment->OnClicked.AddDynamic(this, &UUI_Inventory::OnButtonEquipmentClicked);
	}

	if (ButtonBack != nullptr)
	{
		ButtonBack->OnClicked.AddDynamic(this, &UUI_Inventory::OnButtonBackClicked);
	}

}

void UUI_Inventory::OnButtonCardsClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowInventoryCardsWidget();
	}
}

void UUI_Inventory::OnButtonEquipmentClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowEquipmentWidget();
	}
}

void UUI_Inventory::OnButtonBackClicked()
{
	AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideInventoryWidget();
	}
}
