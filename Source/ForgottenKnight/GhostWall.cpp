// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostWall.h"
#include "Controller_MainCharacter.h"

// Sets default values
AGhostWall::AGhostWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGhostWall::BeginPlay()
{
	Super::BeginPlay();

	if ((ActorHasTag("Phase1") && AController_MainCharacter::MyProgress[1])
		|| (ActorHasTag("Phase2") && AController_MainCharacter::MyProgress[2])
		|| (ActorHasTag("Phase3") && AController_MainCharacter::MyProgress[3])
		|| (ActorHasTag("Phase4") && AController_MainCharacter::MyProgress[4])
		|| (ActorHasTag("Phase5") && AController_MainCharacter::MyProgress[5])
		|| (ActorHasTag("Phase6") && AController_MainCharacter::MyProgress[6])
		|| (ActorHasTag("Phase7") && AController_MainCharacter::MyProgress[7])
		|| (ActorHasTag("Phase8") && AController_MainCharacter::MyProgress[8])
		|| (ActorHasTag("Phase9") && AController_MainCharacter::MyProgress[9])
		|| (ActorHasTag("Phase10") && AController_MainCharacter::MyProgress[10])
		|| (ActorHasTag("Phase11") && AController_MainCharacter::MyProgress[11])
		|| (ActorHasTag("Phase12") && AController_MainCharacter::MyProgress[12])
		|| (ActorHasTag("Phase13") && AController_MainCharacter::MyProgress[13])
		|| (ActorHasTag("Phase14") && AController_MainCharacter::MyProgress[14])
		|| (ActorHasTag("Phase15") && AController_MainCharacter::MyProgress[15])
		|| (ActorHasTag("Phase16") && AController_MainCharacter::MyProgress[16])
		|| (ActorHasTag("Phase17") && AController_MainCharacter::MyProgress[17])
		|| (ActorHasTag("Phase18") && AController_MainCharacter::MyProgress[18])
		|| (ActorHasTag("Phase19") && AController_MainCharacter::MyProgress[19])
		|| (ActorHasTag("Phase20") && AController_MainCharacter::MyProgress[20])
		)
	{
		Destroy();
	}
	
}

// Called every frame
void AGhostWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

