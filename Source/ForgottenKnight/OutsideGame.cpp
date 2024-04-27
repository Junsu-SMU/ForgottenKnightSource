// Fill out your copyright notice in the Description page of Project Settings.


#include "OutsideGame.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Controller_MainCharacter.h"
#include "ForgottenKnightCharacter.h"

// Sets default values
AOutsideGame::AOutsideGame()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOutsideGame::BeginPlay()
{
	Super::BeginPlay();

	if (ActorHasTag("Map1"))
	{
		AForgottenKnightCharacter::MyMapLive = 1;
	}
	else if (ActorHasTag("Map2"))
	{
		AForgottenKnightCharacter::MyMapLive = 2;
	}

	AForgottenKnightCharacter* Player = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	{
		if (Player != nullptr)
		{
			AController_MainCharacter* Controller = Cast<AController_MainCharacter>(Player->GetController());
			Controller->ShowoutsideWidget();
			Player->RemoveHealthBar();
			Player->OutGameCamera();

			Player->Savable = true;

			//Player->AddActorWorldTransform(AForgottenKnightCharacter::MyStartTransform);

			if (ActorHasTag("Map1"))
			{
				if (!AController_MainCharacter::MyProgress[0])// 처음이면
				{
					AController_MainCharacter::MyProgress[0] = true;
				}
				else // 처음 아니면
				{
					//Player->AddActorWorldOffset(FVector(-1420, 350, -598.25f)); // 맵1기준 1420, -350, 598.25
					Player->SetActorTransform(AForgottenKnightCharacter::MyStartTransform, false, nullptr, ETeleportType::TeleportPhysics);
					float x = AForgottenKnightCharacter::MyStartTransform.GetLocation().X;
					float y = AForgottenKnightCharacter::MyStartTransform.GetLocation().Y;
					float z = AForgottenKnightCharacter::MyStartTransform.GetLocation().Z;

					UE_LOG(LogTemp, Log, TEXT("%f %f %f"), x, y, z);
				}
			}
			else if (ActorHasTag("Map2"))
			{
				if (!AController_MainCharacter::MyProgress[11] || AController_MainCharacter::MyProgress[21])// 2_1 클리어 못했을 때
				{
					AController_MainCharacter::MyProgress[21] = false;
				}
				else // 처음 아니면
				{
					//Player->AddActorWorldOffset(FVector(-1420, 350, -598.25f)); // 맵1기준 1420, -350, 598.25
					Player->SetActorTransform(AForgottenKnightCharacter::MyStartTransform2, false, nullptr, ETeleportType::TeleportPhysics);
					float x = AForgottenKnightCharacter::MyStartTransform2.GetLocation().X;
					float y = AForgottenKnightCharacter::MyStartTransform2.GetLocation().Y;
					float z = AForgottenKnightCharacter::MyStartTransform2.GetLocation().Z;

					UE_LOG(LogTemp, Log, TEXT("%f %f %f"), x, y, z);
				}
			}

			if (!AController_MainCharacter::MyProgress[1]) // 1스테이지 안 깼으면
			{
				Controller->ShowScriptWidget(1);
			}
		}
	}
}

