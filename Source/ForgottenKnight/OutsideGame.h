// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OutsideGame.generated.h"

UCLASS()
class FORGOTTENKNIGHT_API AOutsideGame : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOutsideGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
