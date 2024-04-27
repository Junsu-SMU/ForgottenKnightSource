// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGame.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Controller_MainCharacter.h"
#include "ForgottenKnightCharacter.h"

// Sets default values
AStartGame::AStartGame()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStartGame::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AForgottenKnightCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AForgottenKnightCharacter* Player = *Iter; // Player 변수에 DodgeballCharacter 담기

		if (Player != nullptr)
		{
			Player->Savable = true;
			AController_MainCharacter* Controller = Cast<AController_MainCharacter>(Player->GetController());
			Controller->ShowStartWidget();
		}
	}
}
