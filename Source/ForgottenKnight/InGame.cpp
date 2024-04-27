// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Controller_MainCharacter.h"
#include "ForgottenKnightCharacter.h"

// Sets default values
AInGame::AInGame()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInGame::BeginPlay()
{
	Super::BeginPlay();

	AForgottenKnightCharacter* Player = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AController_MainCharacter* Controller = Cast<AController_MainCharacter>(Player->GetController());

		if (Player != nullptr && Controller != nullptr)
		{
			Controller->ShowPlayWindowWidget();
			Controller->ClickMovable = false;
			Player->Movable = false;
			Player->Savable = false;
			Player->InGameCamera();
			Player->GameHitNum = 0; // 피해 받은 횟수 초기화

			if (!AController_MainCharacter::Tutorial && Player->ActorHasTag("Stage1_1"))
			{
				Controller->ShowTutorialWidget();
			}
		}

}

// Called every frame
void AInGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

