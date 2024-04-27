// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "EnemyCharacter.h"
#include "MyHealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "cmath"
#include "MySaveGame.h"
#include "EngineUtils.h"
#include "UI_PlayWindow.h"

//////////////////////////////////////////////////////////////////////////
// AForgottenKnightCharacter

int AForgottenKnightCharacter::MyLevel = 1;
int AForgottenKnightCharacter::MyCurrentEXP = 0;
int AForgottenKnightCharacter::MyMaxEXP = 10;
int AForgottenKnightCharacter::MyMaxHealth = 60;
int AForgottenKnightCharacter::MyCurrentHealth = 60;
int AForgottenKnightCharacter::MyGold = 0;
int AForgottenKnightCharacter::MyMapLive = 1;
int AForgottenKnightCharacter::MySwordLv = 1;
int AForgottenKnightCharacter::MySheildLv = 1;
TArray<int32> AForgottenKnightCharacter::MyAccessoryData = { 0, 0, 0 }; // 목걸이, 팔찌, 반지 순
FTransform AForgottenKnightCharacter::MyStartTransform = FTransform(FQuat(0, 0, 0, 0), FVector(0, 0, 0));
FTransform AForgottenKnightCharacter::MyStartTransform2 = FTransform(FQuat(0, 0, 0, 0), FVector(0, 0, 0));

AForgottenKnightCharacter::AForgottenKnightCharacter()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f); // ...at this rotation rate
	//GetCharacterMovement()->JumpZVelocity = 600.f;
	//GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1000.0f; // The camera follows at this distance behind the character
	//CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bDoCollisionTest = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	//HealthWidgetComp->SetupAttachment(RootComponent);

	MyAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("MySound"));
	MyAudioComp->SetupAttachment(RootComponent);
	MyAudioComp->bAutoActivate = false;

	Movable = true;
	MyLife = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

bool AForgottenKnightCharacter::CalculateTurnEnd(int mobnum, int DebuffType, int DebuffValue, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken)
{
	AController_MainCharacter* MyController = Cast<AController_MainCharacter>(Controller);
	if (MyController)
	{
		UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
		if (MyWindow)
		{
			UMyHealthBar* healthbar = Cast<UMyHealthBar>(MyWindow->MyHealthbar0);
			if (healthbar)
			{
				if (DebuffType == 1) // 부식
				{
					ConfirmedStackCorrosion += DebuffValue;
				}
				else if (DebuffType == 2) // 약화
				{
					ConfirmedStackWeakening += DebuffValue;
				}
				else if (DebuffType == 3) // 출혈
				{
					ConfirmedStackBlooding += DebuffValue;
				}

				return healthbar->TurnEnd(mobnum, Damage, Card, Order, BreatheOn, GetCorrosion, EnemyCorrosion, GetWeaken, EnemyWeaken);
			}
		}
	}

	return false;
}

void AForgottenKnightCharacter::InGameCamera()
{
	CameraBoom->SetRelativeRotation(FRotator(-45.f, 180.f, 0.f));
	CameraBoom->SetRelativeLocation(FVector(150.f, 0.f, 0.f));
	CameraBoom->TargetArmLength = 1000.0f;
}

void AForgottenKnightCharacter::OutGameCamera()
{
	CameraBoom->SetRelativeRotation(FRotator(-75.f, 90.f, 0.f));
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	CameraBoom->TargetArmLength = 700.0f;
}

void AForgottenKnightCharacter::UpdateHealth()
{
	AController_MainCharacter* MyController = Cast<AController_MainCharacter>(Controller);
	if (MyController)
	{
		UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
		if (MyWindow)
		{
			UMyHealthBar* healthbar = Cast<UMyHealthBar>(MyWindow->MyHealthbar0);
			if (healthbar)
			{
				healthbar->UpdateHealthBar();
			}
		}
	}
}

void AForgottenKnightCharacter::UpdateNagativeEffect()
{
	AController_MainCharacter* MyController = Cast<AController_MainCharacter>(Controller);
	if (MyController)
	{
		UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
		if (MyWindow)
		{
			UMyHealthBar* healthbar = Cast<UMyHealthBar>(MyWindow->MyHealthbar0);
			if (healthbar)
			{
				healthbar->UpdateNagativeEffect(GetStackCorrosion, GetStackWeakening, GetStackBlooding);
			}
		}
	}
}

void AForgottenKnightCharacter::SaveData(int SlotIndex)
{
	UMySaveGame* NewPlayerData = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	if (SlotIndex == 1)
	{
		NewPlayerData->D_SaveSlotName = TEXT("1");
	}
	else if (SlotIndex == 2)
	{
		NewPlayerData->D_SaveSlotName = TEXT("2");
	}
	else if (SlotIndex == 3)
	{
		NewPlayerData->D_SaveSlotName = TEXT("3");
	}
	//NewPlayerData->D_PlayerName = GetPlayerName(); // 이름 입력 구현 하면 추가 예정
	NewPlayerData->D_Level = AForgottenKnightCharacter::MyLevel;
	NewPlayerData->D_CurrentEXP = AForgottenKnightCharacter::MyCurrentEXP;
	NewPlayerData->D_MaxEXP = AForgottenKnightCharacter::MyMaxEXP;
	NewPlayerData->D_CurrentHealth = AForgottenKnightCharacter::MyCurrentHealth;
	NewPlayerData->D_MaxHealth = AForgottenKnightCharacter::MyMaxHealth;
	NewPlayerData->D_Gold = AForgottenKnightCharacter::MyGold;
	NewPlayerData->D_Deck = AController_MainCharacter::MyDeck;
	NewPlayerData->D_Progress = AController_MainCharacter::MyProgress;
	NewPlayerData->D_SaveMap = AForgottenKnightCharacter::MyMapLive;
	
	if (AForgottenKnightCharacter::MyMapLive == 1)
	{
		NewPlayerData->D_SaveTransform = GetActorTransform();
		NewPlayerData->D_SaveTransform2 = AForgottenKnightCharacter::MyStartTransform2;
	}
	else
	{
		NewPlayerData->D_SaveTransform2 = GetActorTransform();
		NewPlayerData->D_SaveTransform = AForgottenKnightCharacter::MyStartTransform;
	}
	NewPlayerData->D_MyStore1_1Count = AController_MainCharacter::Store1_1Count;
	NewPlayerData->D_MyStore2_1Count = AController_MainCharacter::Store2_1Count;
	NewPlayerData->D_MyStore1_3Count = AController_MainCharacter::Store1_3Count;
	NewPlayerData->D_MyStore2_3Count = AController_MainCharacter::Store2_3Count;
	NewPlayerData->D_MyStore4_1Count = AController_MainCharacter::Store4_1Count;
	NewPlayerData->D_MyStore4_3Count = AController_MainCharacter::Store4_3Count;
	NewPlayerData->D_Second = AController_MainCharacter::GameSecond;
	NewPlayerData->D_Minute = AController_MainCharacter::GameMinute;
	NewPlayerData->D_Hour = AController_MainCharacter::GameHour;
	NewPlayerData->D_EventHealData = AController_MainCharacter::EventHealData;
	NewPlayerData->D_EventDungeonData = AController_MainCharacter::EventDungeonData;
	NewPlayerData->D_Event3Data = AController_MainCharacter::Event3Data;
	NewPlayerData->D_Event4Data = AController_MainCharacter::Event4Data;
	NewPlayerData->D_StoreDeleteStack = AController_MainCharacter::StoreDeleteStack;
	NewPlayerData->D_SwordLv = AForgottenKnightCharacter::MySwordLv;
	NewPlayerData->D_SheildLv = AForgottenKnightCharacter::MySheildLv;
	NewPlayerData->D_Tutorial = AController_MainCharacter::Tutorial;
	NewPlayerData->D_MyAccessoryData = AForgottenKnightCharacter::MyAccessoryData;
	NewPlayerData->D_EventFountainData = AController_MainCharacter::EventFountainData;

	UGameplayStatics::SaveGameToSlot(NewPlayerData, NewPlayerData->D_SaveSlotName, 0);
}

void AForgottenKnightCharacter::LoadData(int SlotIndex)
{
	UMySaveGame* NewPlayerData = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	if (SlotIndex == 1)
	{
		NewPlayerData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("1"), 0));
	}
	else if (SlotIndex == 2)
	{
		NewPlayerData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("2"), 0));
	}
	else if (SlotIndex == 3)
	{
		NewPlayerData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("3"), 0));
	}

	if (NewPlayerData == nullptr)
	{
		return;
	}
	//NewPlayerData->D_PlayerName = GetPlayerName(); // 이름 입력 구현 하면 추가 예정
	AForgottenKnightCharacter::MyLevel = NewPlayerData->D_Level;
	AForgottenKnightCharacter::MyCurrentEXP = NewPlayerData->D_CurrentEXP;
	AForgottenKnightCharacter::MyMaxEXP = NewPlayerData->D_MaxEXP;
	AForgottenKnightCharacter::MyCurrentHealth = NewPlayerData->D_CurrentHealth;
	AForgottenKnightCharacter::MyMaxHealth = NewPlayerData->D_MaxHealth;
	AForgottenKnightCharacter::MyGold = NewPlayerData->D_Gold;
	AController_MainCharacter::MyDeck = NewPlayerData->D_Deck;
	AController_MainCharacter::MyProgress = NewPlayerData->D_Progress;
	AForgottenKnightCharacter::MyMapLive = NewPlayerData->D_SaveMap;
	AForgottenKnightCharacter::MyStartTransform = NewPlayerData->D_SaveTransform;
	AForgottenKnightCharacter::MyStartTransform2 = NewPlayerData->D_SaveTransform2;
	AController_MainCharacter::Store1_1Count = NewPlayerData->D_MyStore1_1Count;
	AController_MainCharacter::Store2_1Count = NewPlayerData->D_MyStore2_1Count;
	AController_MainCharacter::Store1_3Count = NewPlayerData->D_MyStore1_3Count;
	AController_MainCharacter::Store2_3Count = NewPlayerData->D_MyStore2_3Count;
	AController_MainCharacter::Store4_1Count = NewPlayerData->D_MyStore4_1Count;
	AController_MainCharacter::Store4_3Count = NewPlayerData->D_MyStore4_3Count;
	AController_MainCharacter::GameSecond = NewPlayerData->D_Second;
	AController_MainCharacter::GameMinute = NewPlayerData->D_Minute;
	AController_MainCharacter::GameHour = NewPlayerData->D_Hour;
	AController_MainCharacter::EventHealData = NewPlayerData->D_EventHealData;
	AController_MainCharacter::EventDungeonData = NewPlayerData->D_EventDungeonData;
	AController_MainCharacter::Event3Data = NewPlayerData->D_Event3Data;
	AController_MainCharacter::Event4Data = NewPlayerData->D_Event4Data;
	AController_MainCharacter::StoreDeleteStack = NewPlayerData->D_StoreDeleteStack;
	AForgottenKnightCharacter::MySwordLv = NewPlayerData->D_SwordLv;
	AForgottenKnightCharacter::MySheildLv = NewPlayerData->D_SheildLv;
	AController_MainCharacter::Tutorial = NewPlayerData->D_Tutorial;
	AForgottenKnightCharacter::MyAccessoryData = NewPlayerData->D_MyAccessoryData;
	AController_MainCharacter::EventFountainData = NewPlayerData->D_EventFountainData;

	if (AForgottenKnightCharacter::MyMapLive == 1)
	{
		UGameplayStatics::OpenLevel(this, FName("OutsideMap"));
	}
	else
	{
		UGameplayStatics::OpenLevel(this, FName("OutsideMap2"));
	}
}

void AForgottenKnightCharacter::LoadNewGame()
{
	AForgottenKnightCharacter::MyLevel = 1;
	AForgottenKnightCharacter::MyCurrentEXP = 0;
	AForgottenKnightCharacter::MyMaxEXP = 10;
	AForgottenKnightCharacter::MyCurrentHealth = 60;
	AForgottenKnightCharacter::MyMaxHealth = 60;
	AForgottenKnightCharacter::MyGold = 0; // 
	AController_MainCharacter::MyDeck = { 1, 1, 1, 1, 1, 1, 1, 2, 3, 4 };
	AController_MainCharacter::MyProgress = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
	AForgottenKnightCharacter::MyMapLive = 1;
	AForgottenKnightCharacter::MyStartTransform = FTransform(FQuat(0, 0, 0, 0), FVector(0, 0, 0));
	AForgottenKnightCharacter::MyStartTransform2 = FTransform(FQuat(0, 0, 0, 0), FVector(0, 0, 0));
	AForgottenKnightCharacter::MySwordLv = 1;
	AForgottenKnightCharacter::MySheildLv = 1;
	AController_MainCharacter::GameSecond = 0;
	AController_MainCharacter::GameMinute = 0;
	AController_MainCharacter::GameHour = 0;
	AController_MainCharacter::EventHealData = { 3, 3, 3, 3, 3, 3 };
	AController_MainCharacter::EventFountainData = { 3, 3, 3, 3 };
	AController_MainCharacter::EventDungeonData = { 0, 0, 0, 0, 0, 0 };
	AController_MainCharacter::Event3Data = { 0, 1, 1, 1, 1, 1 };
	AController_MainCharacter::Event4Data = { false, true, true, true, true, true };
	AController_MainCharacter::Store1_1Count = { 3, 1, 1, 1, 1, 1, 2, 1, 1 };
	AController_MainCharacter::Store2_1Count = { 3, 1, 1, 1, 1, 1, 15, 5, 5, 1, 10, 1 };
	AController_MainCharacter::Store1_3Count = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };
	AController_MainCharacter::Store2_3Count = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };
	AController_MainCharacter::Store4_1Count = { 3, 1, 1, 1, 1, 1, 5, 5, 5, 1, 1, 1 };
	AController_MainCharacter::Store4_3Count = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
												 true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
												 true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };
	AController_MainCharacter::StoreDeleteStack = { 0, 0, 0 };
	AController_MainCharacter::Tutorial = false;
	AForgottenKnightCharacter::MyAccessoryData = { 0, 0, 0 };

	if (AForgottenKnightCharacter::MyMapLive == 1)
	{
		UGameplayStatics::OpenLevel(this, FName("OutsideMap"));
	}
	else
	{
		UGameplayStatics::OpenLevel(this, FName("OutsideMap2"));
	}
}

void AForgottenKnightCharacter::WalkSoundBase(bool Switching)
{
	if (Switching)
	{
		MyAudioComp->Play();
	}
	else
	{
		MyAudioComp->Stop();
	}
}

void AForgottenKnightCharacter::PlayHealEffect(int value)
{
	AController_MainCharacter* MyController = Cast<AController_MainCharacter>(Controller);
	if (MyController)
	{
		UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
		if (MyWindow)
		{
			UMyHealthBar* healthbar = Cast<UMyHealthBar>(MyWindow->MyHealthbar0);
			if (healthbar)
			{
				healthbar->PlayHealEffect(value);
			}
		}
	}
}

void AForgottenKnightCharacter::ConfirmStackClean()
{
	ConfirmedStackCorrosion = 0;
	ConfirmedStackWeakening = 0;
	ConfirmedStackBlooding = 0;
}

int AForgottenKnightCharacter::ShowBonusAttack()
{
	AController_MainCharacter* MyController = Cast<AController_MainCharacter>(Controller);

	return MyController->ShowBonusAttack();
}

void AForgottenKnightCharacter::DebuffCleanze()
{
	GetStackCorrosion = 0; // 보유 디버프 스택
	GetStackWeakening = 0;
	GetStackBlooding = 0;
	ConfirmedStackCorrosion = 0;
	ConfirmedStackWeakening = 0;
	ConfirmedStackBlooding = 0;
}

int AForgottenKnightCharacter::FindFirstAttackMob()
{
	AController_MainCharacter* MyController = Cast<AController_MainCharacter>(Controller);
	if (MyController)
	{
		UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
		if (MyWindow)
		{
			if (MyWindow->MobDone[1] == 1)
			{
				return 1;
			}
			else if (MyWindow->MobDone[2] == 1)
			{
				return 2;
			}
			else if (MyWindow->MobDone[3] == 1)
			{
				return 3;
			}
			else
			{
				return 10;
			}
			
		}
	}

	return 10;
}

void AForgottenKnightCharacter::DamageEffect(int Turn, int MobPattern, bool Hit)
{
	AController_MainCharacter* MyController = Cast<AController_MainCharacter>(Controller);
	if (MyController)
	{
		UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
		if (MyWindow)
		{
			UMyHealthBar* healthbar = Cast<UMyHealthBar>(MyWindow->MyHealthbar0);
			if (healthbar)
			{
				healthbar->DamageEffect(Turn, MobPattern, Hit, GetStackCorrosion, GetStackWeakening, GetStackBlooding);
			}
		}
	}

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AEnemyCharacter* Enemy = *Iter;
		if (Enemy != nullptr)
		{
			if (Enemy->AttackType == "Bite" && AttackedBiteSound != nullptr)
			{
				UGameplayStatics::PlaySound2D(this, AttackedBiteSound);
			}
			else if (Enemy->AttackType == "Knife2" && AttackedKnife2Sound != nullptr)
			{
				UGameplayStatics::PlaySound2D(this, AttackedKnife2Sound);
			}
			else if (Enemy->AttackType == "Bite" && AttackedBiteSound != nullptr)
			{
				UGameplayStatics::PlaySound2D(this, AttackedBiteSound);
			}
			else if (Enemy->AttackType == "Strong" && AttackedStrongSound != nullptr)
			{
				UGameplayStatics::PlaySound2D(this, AttackedStrongSound);
			}
			else if (Enemy->AttackType == "Knife" && AttackedKnifeSound != nullptr)
			{
				UGameplayStatics::PlaySound2D(this, AttackedKnifeSound);
			}
			else if (Enemy->AttackType == "Normal2" && AttackedNormal2Sound != nullptr)
			{
				UGameplayStatics::PlaySound2D(this, AttackedNormal2Sound);
			}
			else if (Enemy->AttackType == "Breath" && TakeBreathSound != nullptr)
			{
				UGameplayStatics::PlaySound2D(this, TakeBreathSound);
			}
			else if (Enemy->AttackType == "Normal2Breath" && AttackedBiteSound != nullptr && TakeBreathSound != nullptr)
			{
				UGameplayStatics::PlaySound2D(this, TakeBreathSound);
				UGameplayStatics::PlaySound2D(this, AttackedBiteSound);
			}
		}
	}
}

int AForgottenKnightCharacter::ParryingWho(int mobnum, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken)
{
	AController_MainCharacter* MyController = Cast<AController_MainCharacter>(Controller);
	if (MyController)
	{
		UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
		if (MyWindow)
		{
			UMyHealthBar* healthbar = Cast<UMyHealthBar>(MyWindow->MyHealthbar0);
			if (healthbar)
			{
				return healthbar->ParryingWho(mobnum, Damage, Card, Order, BreatheOn, GetCorrosion, EnemyCorrosion, GetWeaken, EnemyWeaken);
			}
		}
	}

	return 0;
}

void AForgottenKnightCharacter::ParryingEffect(int Turn)
{
	UGameplayStatics::PlaySound2D(this, ParryingSound);

	AController_MainCharacter* MyController = Cast<AController_MainCharacter>(Controller);
	if (MyController)
	{
		UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
		if (MyWindow)
		{
			UMyHealthBar* healthbar = Cast<UMyHealthBar>(MyWindow->MyHealthbar0);
			if (healthbar)
			{
				healthbar->ParryingEffect(Turn);
			}
		}
	}
}

bool AForgottenKnightCharacter::LifeCheck()
{
	if (AForgottenKnightCharacter::MyCurrentHealth > 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	return true;
}

void AForgottenKnightCharacter::RemoveHealthBar()
{
	AController_MainCharacter* MyController = Cast<AController_MainCharacter>(Controller);
	if (MyController)
	{
		UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
		if (MyWindow)
		{
			UMyHealthBar* healthbar = Cast<UMyHealthBar>(MyWindow->MyHealthbar0);
			if (healthbar)
			{
				healthbar->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void AForgottenKnightCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AForgottenKnightCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AForgottenKnightCharacter::MoveRight);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AForgottenKnightCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AForgottenKnightCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AForgottenKnightCharacter::OnResetVR);
}

void AForgottenKnightCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AForgottenKnightCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AForgottenKnightCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AForgottenKnightCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && (Movable == true))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AForgottenKnightCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && (Movable == true))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
