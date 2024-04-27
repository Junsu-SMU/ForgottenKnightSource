// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "DataTable_Dialogue.generated.h"

USTRUCT(BlueprintType)
struct FDialogueTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teller")
		FString StartTeller;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		FString StartDialogue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teller")
		FString Teller1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		FString Dialogue1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teller")
		FString Teller2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		FString Dialogue2;

};

