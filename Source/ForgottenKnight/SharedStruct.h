// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SharedStruct.generated.h"

USTRUCT(BlueprintType)
struct FBaseAttackKeyword : public FTableRowBase
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool Change;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool LifeSteal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool Fast;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool Combo;

};
