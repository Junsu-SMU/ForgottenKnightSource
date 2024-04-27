// Fill out your copyright notice in the Description page of Project Settings.


#include "EventObject.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "Components/WidgetComponent.h"
#include "UI_InteractionIcon.h"

// Sets default values
AEventObject::AEventObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	//BoxComponent->SetCollisionProfileName(FName(" "));
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetNotifyRigidBodyCollision(true);
	RootComponent = BoxComponent;

	InteractWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction"));
	InteractWidgetComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEventObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEventObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AForgottenKnightCharacter* Player = Cast<AForgottenKnightCharacter>(OtherActor);
	if (Player != nullptr)
	{
		if (InteractSound != nullptr)
		{
			UGameplayStatics::PlaySound2D(this, InteractSound);
		}
		InteractWidgetComp->InitWidget();
		UUI_InteractionIcon* InteractWidget = Cast<UUI_InteractionIcon>(InteractWidgetComp->GetUserWidgetObject());
		if (InteractWidget)
		{
			InteractWidget->PlayOn();
		}
		AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(Player->Controller);
		if (PlayerController != nullptr)
		{
			FName Stage = Tags[0];
			FString StrStage = Stage.ToString();
			AController_MainCharacter::InteractKeyword = StrStage;
		}
	}
}

void AEventObject::NotifyActorEndOverlap(AActor* OtherActor)
{
	AForgottenKnightCharacter* Player = Cast<AForgottenKnightCharacter>(OtherActor);
	if (Player != nullptr)
	{
		InteractWidgetComp->InitWidget();
		UUI_InteractionIcon* InteractWidget = Cast<UUI_InteractionIcon>(InteractWidgetComp->GetUserWidgetObject());
		if (InteractWidget)
		{
			InteractWidget->PlayOff();
		}
		AController_MainCharacter::InteractKeyword = {" "};

		AController_MainCharacter* PlayerController = Cast<AController_MainCharacter>(Player->Controller);
		if (PlayerController != nullptr)
		{
			if (PlayerController->EventCode == "Heal")
			{
				PlayerController->HideEventHealWidget();
			}
			else if (PlayerController->EventCode == "Dungeon")
			{
				PlayerController->HideEventDungeonWidget();
			}
			else if (PlayerController->EventCode == "3")
			{
				PlayerController->HideEvent3Widget();
			}
			else if (PlayerController->EventCode == "4")
			{
				PlayerController->HideEvent4Widget();
			}
			else if (PlayerController->EventCode == "Fountain")
			{
				PlayerController->HideEventFountainWidget();
			}
		}

	}
}
