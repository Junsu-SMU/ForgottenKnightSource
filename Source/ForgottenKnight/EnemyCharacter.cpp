// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyHealthBar.h"
#include "MyHealthBar.h"
#include "EnemyPattern.h"
#include "EnemyImage.h"
#include "ForgottenKnightCharacter.h"
#include "Controller_MainCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "iostream"
#include "random"
#include "cmath"
#include "UI_PlayWindow.h"

std::random_device rng;
std::uniform_int_distribution<int> RandTwo(0, 1); // [0, 1] 범위
std::uniform_int_distribution<int> RandThree(0, 2); // [0, 2] 범위
std::uniform_int_distribution<int> RandFour(0, 3); // [0, 3] 범위
std::uniform_int_distribution<int> RandFive(0, 4); // [0, 4] 범위
std::uniform_int_distribution<int> RandSeven(0, 6); // [0, 6] 범위

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	//HealthWidgetComp->SetupAttachment(RootComponent);

	DamageValue = 0;
	GolemDelay = false;
	AsuraOrder = 1;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;

				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
					if (healthbar)
					{
						MyIndex = 1;
						healthbar->CallMyIndex(1);
						
						if (ActorHasTag("NoScan"))
						{
							healthbar->NoScan = true;
						}
					}
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
					if (healthbar)
					{
						MyIndex = 2;
						healthbar->CallMyIndex(2);

						if (ActorHasTag("NoScan"))
						{
							healthbar->NoScan = true;
						}
					}
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
					if (healthbar)
					{
						MyIndex = 3;
						healthbar->CallMyIndex(3);

						if (ActorHasTag("NoScan"))
						{
							healthbar->NoScan = true;
						}
					}
				}
			}
		}
	}

	SetStat();
	ChangeDamage();
	
}

void AEnemyCharacter::SetStat()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
					UE_LOG(LogTemp, Log, TEXT("Enemy Cast1"));
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
					UE_LOG(LogTemp, Log, TEXT("Enemy Cast2"));
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
					UE_LOG(LogTemp, Log, TEXT("Enemy Cast3"));
				}

				if (healthbar)
				{
					if (ActorHasTag("Slime"))
					{
						healthbar->StatSet(10);
						healthbar->SetMobImage("Slime");
					}
					else if (ActorHasTag("Goblin"))
					{
						healthbar->StatSet(7);
						healthbar->SetMobImage("Goblin");
					}
					else if (ActorHasTag("Troll"))
					{
						healthbar->StatSet(25);
						healthbar->SetMobImage("Troll");
					}
					else if (ActorHasTag("Spider"))
					{
						healthbar->StatSet(15);
						healthbar->SetMobImage("Spider");
					}
					else if (ActorHasTag("Mantis")) //사마귀
					{
						healthbar->StatSet(15);
						healthbar->SetMobImage("Mantis");
					}
					else if (ActorHasTag("Mantis_Up")) //사마귀
					{
						healthbar->StatSet(30);
						healthbar->SetMobImage("Mantis");
					}
					else if (ActorHasTag("Golem"))
					{
						healthbar->StatSet(40);
						healthbar->SetMobImage("Golem");
					}
					else if (ActorHasTag("Asura"))
					{
						healthbar->StatSet(20); // 각 20
						healthbar->SetMobImage("Asura");
					}
					else if (ActorHasTag("Asura_Up"))
					{
						healthbar->StatSet(27); // 각 20
						healthbar->SetMobImage("Asura_Up");
					}
					else if (ActorHasTag("ShadowSpear"))
					{
						healthbar->StatSet(30);
						healthbar->SetMobImage("ShadowSpear");
					}
					else if (ActorHasTag("ShadowSheild"))
					{
						healthbar->StatSet(50);
						healthbar->SetMobImage("ShadowSheild");
					}
					else if (ActorHasTag("ShadowAxe"))
					{
						healthbar->StatSet(40);
						healthbar->SetMobImage("ShadowAxe");
					}
					else if (ActorHasTag("Griffon"))
					{
						healthbar->StatSet(35);
						healthbar->SetMobImage("Griffon");
					}
					else if (ActorHasTag("Rafflesia"))
					{
						healthbar->StatSet(60);
						healthbar->SetMobImage("Rafflesia");
					}
					else if (ActorHasTag("Warlock2Phase"))
					{
						healthbar->StatSet(30);
						healthbar->SetMobImage("Warlock2Phase");
						IamWarlock = false;
					}
					else if (ActorHasTag("Warlock"))
					{
						healthbar->StatSet(100);
						healthbar->SetMobImage("Warlock");
						IamWarlock = true;
					}
					else if (ActorHasTag("SkullHead"))
					{
						if (IamSkeleton == 1)
						{
							healthbar->StatSet(40);
						}
						else if (IamSkeleton == 2)
						{
							healthbar->StatSet(25);
						}
						else if (IamSkeleton == 3)
						{
							healthbar->StatSet(15);
						}
						else if (IamSkeleton == 4)
						{
							healthbar->StatSet(40);
						}

						healthbar->SetMobImage("SkullHead");
						Skullmode = true;
					}
					else if (ActorHasTag("SkeletonSoldier"))
					{
						healthbar->StatSet(80);
						healthbar->SetMobImage("SkeletonSoldier");
						IamSkeleton = 1;
						Skullmode = false;
					}
					else if (ActorHasTag("SkeletonArcher"))
					{
						healthbar->StatSet(50);
						healthbar->SetMobImage("SkeletonArcher");
						IamSkeleton = 2;
						Skullmode = false;
					}
					else if (ActorHasTag("SkeletonGuardDog"))
					{
						healthbar->StatSet(30);
						healthbar->SetMobImage("SkeletonGuardDog");
						IamSkeleton = 3;
						Skullmode = false;
					}
					else if (ActorHasTag("Lich"))
					{
						healthbar->StatSet(80);
						healthbar->SetMobImage("Lich");
						IamSkeleton = 4;
						IamLich = true;
						Skullmode = false;
					}
					else if (ActorHasTag("Beholder"))
					{
						healthbar->StatSet(80);
						healthbar->SetMobImage("Beholder");
						firstturnbeholder = 0;
					}
					else if (ActorHasTag("FallenFairy"))
					{
						healthbar->StatSet(40);
						healthbar->SetMobImage("FallenFairy");
						healthbar->DebuffImmune = true;
						FairyTurn = 0;
					}
					else if (ActorHasTag("Baphomet"))
					{
						healthbar->StatSet(40);
						healthbar->SetMobImage("Baphomet");
					}
					else if (ActorHasTag("Durahan"))
					{
						healthbar->StatSet(100);
						healthbar->SetMobImage("Durahan");
					}
					else if (ActorHasTag("SkeletonCenturion"))
					{
						healthbar->StatSet(200);
						healthbar->SetMobImage("SkeletonCenturion");
					}
					else if (ActorHasTag("King"))
					{
						healthbar->StatSet(100);
						healthbar->SetMobImage("King");
					}
					else if (ActorHasTag("DemonLordHead"))
					{
						healthbar->StatSet(150);
						healthbar->SetMobImage("DemonLordHead");
					}
					else if (ActorHasTag("DemonLordLeft"))
					{
						healthbar->StatSet(100);
						healthbar->SetMobImage("DemonLordLeft");
					}
					else if (ActorHasTag("DemonLordRight"))
					{
						healthbar->StatSet(100);
						healthbar->SetMobImage("DemonLordRight");
					}
					else if (ActorHasTag("MagicBall"))
					{
						healthbar->StatSet(10);
						healthbar->SetMobImage("Ball1");
					}
					else if (ActorHasTag("Random")) // 랜덤 해골 소환(던전4-3패턴)
					{
						int x = RandThree(rng);

						if (x == 0)
						{
							Tags[1] = FName("SkeletonSoldier");
						}
						else if (x == 1)
						{
							Tags[1] = FName("SkeletonArcher");
						}
						else if (x == 2)
						{
							Tags[1] = FName("SkeletonGuardDog");
						}

						SetStat();
					}
				}

				if (!ActorHasTag("SkullHead"))
				{
					Skullmode = false;
				}
				if (!ActorHasTag("SkeletonSoldier") && !ActorHasTag("SkeletonArcher") && !ActorHasTag("SkeletonGuardDog") && !ActorHasTag("Lich"))
				{
					IamSkeleton = 0;
				}
				if (!ActorHasTag("Lich"))
				{
					IamLich = false;
				}
			}
		}
	}
}

void AEnemyCharacter::BloodingDamage()
{
	if (IamWarlock)
	{
		return;
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->GetBloodDamaged(GetStackBlooding);
					}
				}
			}
		}
	}
}

void AEnemyCharacter::BloodEffect()
{
	if (IamWarlock)
	{
		return;
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->BloodEffect();
					}
				}
			}
		}
	}
}

void AEnemyCharacter::GoAttack()
{
	if (IamWarlock)
	{
		return;
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->GoAttack();
					}
				}
			}
		}
	}
}

void AEnemyCharacter::UpdateHealth(int heal)
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->UpdateHealthBar(heal);
					}
				}
			}
		}
	}
}

void AEnemyCharacter::Revive()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->Revive();
					}
				}
			}
		}
	}
}

void AEnemyCharacter::UpdateNagativeEffect()
{
	if (IamWarlock)
	{
		return;
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->UpdateNagativeEffect(GetStackCorrosion, GetStackWeakening, GetStackBlooding);
					}
				}
			}
		}
	}
}

void AEnemyCharacter::DamageEffect(int Turn, int CardNum, bool Hit)
{
	if (IamWarlock)
	{
		return;
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->DamageEffect(Turn, CardNum, Hit, GetStackCorrosion, GetStackWeakening, GetStackBlooding);
					}
				}
			}
		}
	}
}

void AEnemyCharacter::ParryingEffect()
{
	if (IamWarlock)
	{
		return;
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->ParryingEffect();
					}
				}
			}
		}
	}
}

void AEnemyCharacter::ChangeDamage()
{
	AttackStackCorrosion = 0;
	AttackStackWeakening = 0;
	AttackStackBlooding = 0;
	ConfirmedDebuffType = 0;
	ConfirmedDebuffValue = 0;
	ConfirmedBreathe = false;
	TurnEndHeal = 0;

	AttackOrder = 2; // 빠른 공격 : 1, 표준 : 2, 느린 공격 : 3
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		UEnemyHealthBar* healthbar = nullptr;

		AController_MainCharacter* MyController = Cast< AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}
			}
		}
		int finaldamage = 0;

		if (ActorHasTag("Slime"))
		{
			//체력10, 공격패턴 : 피해2+적중 시 부식2 / 피해4 / 피해2+적중 시 약화2
			int x = RandThree(rng);

			AttackType = "Bite";

			if (healthbar)
			{
				if (x == 0) // 피해2, 부식2
				{
					finaldamage = 2 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackCorrosion = 2;
				}
				else if (x == 1) // 피해4
				{
					finaldamage = 4 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 2) // 피해2, 약화2
				{
					finaldamage = 2 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackWeakening = 2;
				}
			}
		}
		else if (ActorHasTag("Goblin"))
		{
			//체력7, 공격패턴 : 피해3 적중 시 출혈2 / 피해6
			int x = RandTwo(rng);

			AttackType = "Knife2";

			if (healthbar)
			{
				if (x == 0) // 피해3 적중 시 출혈2
				{
					finaldamage = 3 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackBlooding = 2;
				}
				else if (x == 1) // 피해6
				{
					finaldamage = 6 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
			}
		}
		else if (ActorHasTag("Troll"))
		{
			//체력25, 공격패턴 : 피해6

			AttackType = "Strong";

			if (healthbar)
			{
				finaldamage = 6 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 2;
					InBreathe = false;
				}

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = false;
			}
		}
		else if (ActorHasTag("Spider")) // 피 1/3 까일 때마다 공격 대미지 1/3씩 줆어들게
		{
			//체력15, 공격패턴 : 피해3 약화3 / 피해9
			int x = RandTwo(rng);
			float y = float(healthbar->CurrentHealth) / float(healthbar->MaxHealth);

			AttackType = "Bite";

			if (healthbar)
			{
				if (x == 0) // 피해3 약화3
				{
					if (y > 2 / 3)
					{
						finaldamage = 3 + BonusDamageValue;
						if (InBreathe)
						{
							finaldamage += 2;
							InBreathe = false;
						}

						healthbar->GainNagative();
						healthbar->LosePositive();
						healthbar->AttackSet(finaldamage);
						DamageValue = finaldamage;
						GetAttackNag = true;
						ConfirmedDebuffType = 2;
						ConfirmedDebuffValue = 3;
					}
					else if (y > 1 / 3)
					{
						finaldamage = 2 + BonusDamageValue;
						if (InBreathe)
						{
							finaldamage += 2;
							InBreathe = false;
						}

						healthbar->GainNagative();
						healthbar->LosePositive();
						healthbar->AttackSet(finaldamage);
						DamageValue = finaldamage;
						GetAttackNag = true;
						ConfirmedDebuffType = 2;
						ConfirmedDebuffValue = 2;
					}
					else
					{
						finaldamage = 1 + BonusDamageValue;
						if (InBreathe)
						{
							finaldamage += 2;
							InBreathe = false;
						}

						healthbar->GainNagative();
						healthbar->LosePositive();
						healthbar->AttackSet(finaldamage);
						DamageValue = finaldamage;
						GetAttackNag = true;
						ConfirmedDebuffType = 2;
						ConfirmedDebuffValue = 1;
					}
				}
				else if (x == 1) // 피해9
				{
					if (y > 2 / 3)
					{
						finaldamage = 9 + BonusDamageValue;
						if (InBreathe)
						{
							finaldamage += 2;
							InBreathe = false;
						}

						healthbar->LoseNagative();
						healthbar->LosePositive();
						healthbar->AttackSet(finaldamage);
						DamageValue = finaldamage;
						GetAttackNag = false;
					}
					else if (y > 1 / 3)
					{
						finaldamage = 6 + BonusDamageValue;
						if (InBreathe)
						{
							finaldamage += 2;
							InBreathe = false;
						}

						healthbar->LoseNagative();
						healthbar->LosePositive();
						healthbar->AttackSet(finaldamage);
						DamageValue = finaldamage;
						GetAttackNag = false;
					}
					else
					{
						finaldamage = 3 + BonusDamageValue;
						if (InBreathe)
						{
							finaldamage += 2;
							InBreathe = false;
						}

						healthbar->LoseNagative();
						healthbar->LosePositive();
						healthbar->AttackSet(finaldamage);
						DamageValue = finaldamage;
						GetAttackNag = false;
					}
				}
			}
		}
		else if (ActorHasTag("Mantis"))
		{
			//체력15, 공격패턴 : 피해4 출혈2 / 피해1 출혈6 (출혈스택10이상일 시 피해 15 줌)
			int x = RandTwo(rng);

			AttackType = "Knife";

			if (healthbar)
			{
				if (MyPlayer->GetStackBlooding >= 10) // 피해 15
				{
					finaldamage = 15 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 0) // 피해4 출혈2
				{
					finaldamage = 4 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 3;
					ConfirmedDebuffValue = 2;
				}
				else if (x == 1) // 피해1 출혈6
				{
					finaldamage = 1 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 3;
					ConfirmedDebuffValue = 6;
				}
			}
		}
		else if (ActorHasTag("Mantis_Up"))
		{
			//체력30, 공격패턴 : 피해8 출혈2 / 피해2 출혈6 (출혈스택10이상일 시 피해 30 줌)
			int x = RandTwo(rng);

			AttackType = "Knife";

			if (healthbar)
			{
				if (MyPlayer->GetStackBlooding >= 10) // 피해 30
				{
					finaldamage = 30 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 0) // 피해8 출혈2
				{
					finaldamage = 8 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 3;
					ConfirmedDebuffValue = 2;
				}
				else if (x == 1) // 피해2 출혈6
				{
					finaldamage = 2 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 3;
					ConfirmedDebuffValue = 6;
				}
			}
		}
		else if (ActorHasTag("Golem"))
		{
			//체력40, 공격패턴 : 피해10 / 다음 턴 행동x

			if (healthbar)
			{
				if (GolemDelay == false)
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Strong";

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
					GolemDelay = true;
				}
				else
				{
					// 행동 안하므로 숨고르기 소모 x

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(0);
					DamageValue = 0;
					GetAttackNag = false;
					GolemDelay = false;
				}
			}
		}
		else if (ActorHasTag("Asura"))
		{
			//체력20, 공격패턴 : 피해10 / 피해6 출혈2 / 피해8 약화2 / 피해8 부식2 / 피해 2 숨고르기
			//공격방향이 하나만 남으면 : 피해8 출혈4 -> 숨고르기 -> 피해14
			int x = RandFive(rng);
			//int NumEnemy = MyController->ConfirmEnemyNum();

			if (healthbar)
			{
				if (AsuraRevivemode && AsuraOrder == 1) // 피해8 출혈4
				{
					finaldamage = 8 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2";

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 3;
					ConfirmedDebuffValue = 4;

					AsuraOrder = 2;
				}
				else if (AsuraRevivemode && AsuraOrder == 2) // 숨고르기
				{
					// 공격이 아니므로 숨고르기 소모 x

					AttackType = "Breath";

					healthbar->LoseNagative();
					healthbar->GainPositive();
					healthbar->AttackSet(0);
					DamageValue = 0;
					GetAttackNag = false;

					AsuraOrder = 3;

					ConfirmedBreathe = true;
				}
				else if (AsuraRevivemode && AsuraOrder == 3) // 피해14
				{
					finaldamage = 14 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2";

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;

					AsuraOrder = 1;
				}
				else if (x == 0) // 피해6 출혈2
				{
					finaldamage = 6 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2";

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 3;
					ConfirmedDebuffValue = 2;
				}
				else if (x == 1) // 피해10
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2";

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 2) // 피해8 약화2
				{
					finaldamage = 8 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2";

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 2;
					ConfirmedDebuffValue = 2;
				}
				else if (x == 3) // 피해8 부식2
				{
					finaldamage = 8 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2";

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 1;
					ConfirmedDebuffValue = 2;
				}
				else if (x == 4) // 피해2 숨고르기
				{
					finaldamage = 2 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2Breath";

					healthbar->LoseNagative();
					healthbar->GainPositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;

					ConfirmedBreathe = true;
				}
			}
		}
		else if (ActorHasTag("Asura_Up"))
		{
			//기존보다 체력, 대미지 30퍼 증가
			//체력27, 공격패턴 : 피해13 / 피해8 출혈2 / 피해11 약화2 / 피해11 부식2 / 피해 3 숨고르기
			//공격방향이 하나만 남으면 : 피해11 출혈4 -> 숨고르기 -> 피해19
			int x = RandFive(rng);
			//int NumEnemy = MyController->ConfirmEnemyNum();

			if (healthbar)
			{
				if (AsuraRevivemode && AsuraOrder == 1) // 피해11 출혈4
				{
					finaldamage = 11 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2";

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 3;
					ConfirmedDebuffValue = 4;

					AsuraOrder = 2;
				}
				else if (AsuraRevivemode && AsuraOrder == 2) // 숨고르기
				{
					// 공격이 아니므로 숨고르기 소모 x

					AttackType = "Breath";

					healthbar->LoseNagative();
					healthbar->GainPositive();
					healthbar->AttackSet(0);
					DamageValue = 0;
					GetAttackNag = false;

					AsuraOrder = 3;

					ConfirmedBreathe = true;
				}
				else if (AsuraRevivemode && AsuraOrder == 3) // 피해19
				{
					finaldamage = 19 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2";

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;

					AsuraOrder = 1;
				}
				else if (x == 0) // 피해8 출혈2
				{
					finaldamage = 8 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2";

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 3;
					ConfirmedDebuffValue = 2;
				}
				else if (x == 1) // 피해13
				{
					finaldamage = 13 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2";

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 2) // 피해11 약화2
				{
					finaldamage = 11 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2";

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 2;
					ConfirmedDebuffValue = 2;
				}
				else if (x == 3) // 피해11 부식2
				{
					finaldamage = 11 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2";

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 1;
					ConfirmedDebuffValue = 2;
				}
				else if (x == 4) // 피해3 숨고르기
				{
					finaldamage = 3 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2Breath";

					healthbar->LoseNagative();
					healthbar->GainPositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;

					ConfirmedBreathe = true;
				}
			}
		}
		else if (ActorHasTag("ShadowSpear"))
		{
			//체력30, 공격패턴 : 피해12 적중시 부식2 / 피해10 적중시 출혈2
			int x = RandTwo(rng);

			AttackType = "Knife2";

			if (healthbar)
			{
				if (x == 0) // 피해12 적중시 부식2
				{
					finaldamage = 12 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					//AttackOrder = 1; // 빠공
					//healthbar->SetAttackSpeed(1); // 빠공

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackCorrosion = 2;
				}
				else if (x == 1) // 피해10 적중시 출혈2
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackBlooding = 2;
				}
			}
		}
		else if (ActorHasTag("ShadowSheild"))
		{
			//체력50, 공격패턴 : 피해10 주고 체력 3 회복

			AttackType = "Strong";

			if (healthbar)
			{
				finaldamage = 10 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 2;
					InBreathe = false;
				}

				TurnEndHeal = 3;

				healthbar->LoseNagative();
				healthbar->GainPositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = false;
			}
		}
		else if (ActorHasTag("ShadowAxe"))
		{
			//체력40, 공격패턴 : 숨고르기(연속x) / 피해10 // 혼자 남으면 피해15+숨고르기
			int x = RandTwo(rng);

			if (ShadowAxePattern) // 전 턴 숨고르기이면
			{
				x = 0; // 피해 10
			}
			else
			{
				x = RandTwo(rng); // 패턴 2개
			}

			AttackType = "Knife2";

			if (healthbar)
			{
				int FieldMob = 0; // 필드에 있는 남은 몹 수
				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;

					if (Enemy != nullptr && Enemy->Life)
					{
						FieldMob += 1;
					}
				}

				if (FieldMob == 1) // 나 혼자 남았을 때 : 피해15 + 숨고르기
				{
					finaldamage = 15 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackType = "Normal2Breath";

					healthbar->LoseNagative();
					healthbar->GainPositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;

					ConfirmedBreathe = true;
				}
				else if (x == 0) // 피해10
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;

					ShadowAxePattern = false;
				}
				else if (x == 1) // 숨고르기
				{
					// 공격이 아니므로 숨고르기 소모 x

					AttackType = "Breath";

					healthbar->LoseNagative();
					healthbar->GainPositive();
					healthbar->AttackSet(0);
					DamageValue = 0;
					GetAttackNag = false;

					ConfirmedBreathe = true;

					ShadowAxePattern = true;
				}
			}
		}
		else if (ActorHasTag("Griffon"))
		{
			//체력35, 공격패턴 : 피해10 비상상태 / 피해8 돌진상태
			// 비상상태 -> 피해10 출혈3 / 돌진상태 -> 피해15 약화2
			int x = RandTwo(rng);

			AttackType = "Strong";

			if (healthbar)
			{
				if (GriffonCondition == 1) // 비상상태 공격 : 피해10, 출혈3
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					//AttackOrder = 1; // 빠공
					//HealthBarWidget->SetAttackSpeed(1); // 빠공

					GriffonCondition = 0; // 평소 상태 복귀

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 3;
					ConfirmedDebuffValue = 3;
				}
				else if (GriffonCondition == 2) // 돌진상태 공격 : 피해 15, 약화2
				{
					finaldamage = 15 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					//AttackOrder = 3; // 느공
					//HealthBarWidget->SetAttackSpeed(3); // 느공

					GriffonCondition = 0; // 평소 상태 복귀

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 2;
					ConfirmedDebuffValue = 2;
				}
				else if (x == 0) // 피해10 비상상태 진입
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					GriffonCondition = 1; // 1 : 비상상태 돌입

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 1) // 피해8 돌진상태 진입
				{
					finaldamage = 8 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					GriffonCondition = 2; // 2 : 돌진상태 돌입

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
			}
		}
		else if (ActorHasTag("Rafflesia"))
		{
			//체력10, 공격패턴 : 피해5,향기상태 / 피해5 약화3 / 피해5 부식3

			int x = 0;
			if (IamScent) // 지금 향기상태이면
			{
				x = RandTwo(rng); // 패턴 2개
			}
			else // 향기 상태 아니면
			{
				x = RandThree(rng); // 패턴 3개
			}

			AttackType = "Bite"; // 공격 효과음 타입

			if (healthbar)
			{
				if (x == 0) // 피해5 부식3
				{
					finaldamage = 5 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					ConfirmedScent = false;

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 1;
					ConfirmedDebuffValue = 3;
				}
				else if (x == 1) // 피해5 약화3
				{
					finaldamage = 5 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					ConfirmedScent = false;

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 2;
					ConfirmedDebuffValue = 3;
				}
				else if (x == 2) // 피해5, 향기상태
				{
					finaldamage = 5 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					ConfirmedScent = true; // 다음 턴 향기 상태

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
				}
			}
		}
		else if (ActorHasTag("Warlock"))
		{
			//1페이즈 체력100, 슬라임 - 흑마 - 고블린 시작, 자신 무적(하수인 죽을 때마다 체력 -10), 하수인 죽을 때 마다 새로 하수인 소환
			// 트롤 -> 거미무리 -> 골렘 -> 그림자 창병 -> 그림자 방패병 -> 그림자 도끼병 -> 그리폰 -> 라플레시아 총 10마리 하수인 상대 시 1페이즈 종료
			//2페이즈 켈베로스 체력90(머리 하나당 30)
			// 첫턴에 피해 10과 적중 시 약화(왼쪽 머리), 부식(가운데 머리), 출혈(오른쪽 머리) 3 부여
			// 추가 패턴으로 피해 5+(디버프 개수 만큼 +5) 피해를 줌.

			//총 상대 횟수(하수인 10마리 및 켈베로스 머리 3개)

			int x = RandFour(rng);

			AttackType = "Normal2";

			if (healthbar)
			{
				if (WarlockAttackmode && ActorHasTag("Enemy1") && WarlockAttackmodeFirst) // 2페이즈 첫 공격 왼쪽 머리일 시, 피해10 적중 시 약화3
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackWeakening = 3;

					WarlockAttackmodeFirst = false;
				}
				else if (WarlockAttackmode && ActorHasTag("Enemy1") && WarlockAttackmodeFirst) // 2페이즈 첫 공격 가운데 머리일 시, 피해10 적중 시 부식3
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackCorrosion = 3;

					WarlockAttackmodeFirst = false;
				}
				else if (WarlockAttackmode && ActorHasTag("Enemy1") && WarlockAttackmodeFirst) // 2페이즈 첫 공격 오른쪽 머리일 시, 피해10 적중 시 출혈3
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackBlooding = 3;

					WarlockAttackmodeFirst = false;
				}
				else if (WarlockAttackmode && (x == 0)) // 피해10, 적중 시 약화3
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackWeakening = 3;

					WarlockAttackCool = false;
				}
				else if (WarlockAttackmode && (x == 1)) // 피해10, 적중 시 부식3
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackCorrosion = 3;

					WarlockAttackCool = false;
				}
				else if (WarlockAttackmode && (x == 2)) // 피해10, 적중 시 출혈3
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackBlooding = 3;

					WarlockAttackCool = false;
				}
				else if (WarlockAttackmode && (x == 3)) // 피해5 + 디버프 개수 만큼 대미지 5 추가(연속으로 안나옴)
				{
					if (WarlockAttackCool)
					{
						ChangeDamage();
						return;
					}

					int k = 0;
					if (MyPlayer->GetStackBlooding > 0) { k += 1; }
					if (MyPlayer->GetStackWeakening > 0) { k += 1; }
					if (MyPlayer->GetStackCorrosion > 0) { k += 1; }

					finaldamage = 5 + (5 * k) + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;

					WarlockAttackCool = true;
				}
				else if (!WarlockAttackmode) // 1페이즈 일 시
				{
					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(0);
					DamageValue = 0;
					GetAttackNag = false;
				}
			}
		}
		else if (ActorHasTag("SkullHead"))
		{
		//공격패턴 : 행동x

		if (healthbar)
		{
			// 행동 안하므로 숨고르기 소모 x
			healthbar->LoseNagative();
			healthbar->LosePositive();
			healthbar->AttackSet(0);
			DamageValue = 0;
			GetAttackNag = false;
		}
		}
		else if (ActorHasTag("SkeletonSoldier"))
		{
			//체력10, 공격패턴 : 피해15 / 피해8+적중 시 약화2, 부식2
			int x = RandTwo(rng);

			AttackType = "Knife2";

			if (healthbar)
			{
				if (x == 0) // 피해15
				{
					finaldamage = 15 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 1) // 피해8+적중 시 약화2, 부식2
				{
					finaldamage = 8 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackCorrosion = 2;
					AttackStackWeakening = 2;
				}
			}
		}
		else if (ActorHasTag("SkeletonArcher"))
		{
			//체력10, 공격패턴 : 피해10+적중 시 출혈2 / 피해 6 두번(두번째 공격은 패링이 안됨)
			int x = RandTwo(rng);

			AttackType = "Knife2";

			if (healthbar)
			{
				if (x == 0) // 피해10+적중 시 출혈2
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackBlooding = 2;
				}
				else if (x == 1) // 피해 6 두번(두번째 공격은 패링이 안됨)
				{
					finaldamage = 6 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackTryNum = 2; // 2번 공격

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
			}
		}
		else if (ActorHasTag("SkeletonGuardDog"))
		{
			//체력10, 공격패턴 : 피해30(패링 당할 시 기절) / 피해15+없는 디버프를 하나 건다.(셋 모두 걸려 있으면 출혈2)
			int x = RandTwo(rng);

			AttackType = "Knife2";

			if (healthbar)
			{
				if (LastTurnParrying && SkulldogSpecial) // 지난 턴에 기믹 파훼 당했으면
				{ // 기절
					SkulldogSpecial = false;
					LastTurnParrying = false;

					// 행동 안하므로 숨고르기 소모 x

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(0);
					DamageValue = 0;
					GetAttackNag = false;
					GolemDelay = false;
				}
				else if (x == 0) // 피해30(패링 당할 시 기절)
				{
					finaldamage = 30 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					SkulldogSpecial = true; // 특수 기믹 on

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 1) // 피해15+없는 디버프를 하나 건다.(셋 모두 걸려 있으면 출혈2)
				{
					finaldamage = 15 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					SkulldogSpecial = false;

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;

					int b = MyPlayer->GetStackBlooding;
					int w = MyPlayer->GetStackWeakening;
					int c = MyPlayer->GetStackCorrosion;

					if (b > 0 || w > 0 || c > 0)
					{ // 플레이어가 디버프를 보유하고 있으면
						if (b > 0 && w == 0 && c == 0)
						{ // 출혈만 가지고 있음
							int j = RandTwo(rng);
							if (j == 0)
							{
								ConfirmedDebuffType = 1;
								ConfirmedDebuffValue = 2;
							}
							else if (j == 1)
							{
								ConfirmedDebuffType = 2;
								ConfirmedDebuffValue = 2;
							}
						}
						else if (b == 0 && w > 0 && c == 0)
						{ // 약화만 가지고 있음
							int j = RandTwo(rng);
							if (j == 0)
							{
								ConfirmedDebuffType = 1;
								ConfirmedDebuffValue = 2;
							}
							else if (j == 1)
							{
								ConfirmedDebuffType = 3;
								ConfirmedDebuffValue = 2;
							}
						}
						else if (b == 0 && w == 0 && c > 0)
						{ // 부식만 가지고 있음
							int j = RandTwo(rng);
							if (j == 0)
							{
								ConfirmedDebuffType = 2;
								ConfirmedDebuffValue = 2;
							}
							else if (j == 1)
							{
								ConfirmedDebuffType = 3;
								ConfirmedDebuffValue = 2;
							}
						}
						else if (b > 0 && w > 0 && c == 0)
						{ // 출혈, 약화를 가지고 있음
							ConfirmedDebuffType = 1;
							ConfirmedDebuffValue = 2;
						}
						else if (b > 0 && w == 0 && c > 0)
						{ // 출혈, 부식을 가지고 있음
							ConfirmedDebuffType = 2;
							ConfirmedDebuffValue = 2;
						}
						else
						{ // 부식, 약화를 가지고 있거나 모든 디버프 가짐
							ConfirmedDebuffType = 3;
							ConfirmedDebuffValue = 2;
						}
					}
					else
					{
						int j = RandThree(rng);
						if (j == 0)
						{
							ConfirmedDebuffType = 1;
							ConfirmedDebuffValue = 2;
						}
						else if (j == 1)
						{
							ConfirmedDebuffType = 2;
							ConfirmedDebuffValue = 2;
						}
						else if (j == 2)
						{
							ConfirmedDebuffType = 3;
							ConfirmedDebuffValue = 2;
						}
					}


				}
			}
		}
		else if (ActorHasTag("Lich"))
		{
		//체력80, 공격패턴 : 피해10, 적중 시 올디버프2부여 / 몬스터가 하나라도 죽었으면 랜덤확률로 해골 소환 기믹 실행
		int x = RandTwo(rng);

		AttackType = "Strong";

		if (healthbar)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				if ((!MyWindow->EnemyLife[1] || !MyWindow->EnemyLife[2] || !MyWindow->EnemyLife[3]) && x == 1) // 랜덤 해골 소환
				{ // 누구라도 죽었으면 랜덤 확률로 소환 기믹

					int xx = RandThree(rng);
					LichReviveSkill = xx + 1;

					healthbar->LoseNagative();
					healthbar->GainPositive();
					healthbar->AttackSet(0);
					DamageValue = 0;
					GetAttackNag = false;
				}
				else // 피해10 +  적중 시 올디버프2
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackCorrosion = 2;
					AttackStackBlooding = 2;
					AttackStackWeakening = 2;
				}
			}
		}
		}
		else if (ActorHasTag("Beholder"))
		{
			//체력80, 공격패턴 : 피해10 / 피해20+적중 시 매혹 디버프(반드시 선공:디버프 해제로 해제 가능)
			int x = RandTwo(rng);

			AttackType = "Strong";

			if (healthbar)
			{
				if (x == 0 && firstturnbeholder == 0) // 피해20, 매혹(눈 뜬 상태)
				{
					finaldamage = 20 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					firstturnbeholder = 1; // 눈 뜬 상태

					healthbar->BeholderEye(true);
					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 1 && firstturnbeholder == 0) // 피해10(눈 감은 상태)
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					firstturnbeholder = 2; // 눈 감은 상태

					healthbar->BeholderEye(false);
					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				if (firstturnbeholder == 2) // 피해20, 매혹(눈 뜬 상태)
				{
					finaldamage = 20 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					firstturnbeholder = 1; // 눈 뜬 상태

					healthbar->BeholderEye(true);
					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (firstturnbeholder == 1) // 피해10(눈 감은 상태)
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					firstturnbeholder = 2; // 눈 감은 상태

					healthbar->BeholderEye(false);
					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
			}
		}
		else if (ActorHasTag("FallenFairy"))
		{
		//체력40, 공격패턴 : 정해짐

		AttackType = "Bite";

		if (healthbar)
		{
			if (FairyTurn == 0) // 피해12, 적중 시 올 디버프 2 부여
			{
				finaldamage = 12 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 2;
					InBreathe = false;
				}

				FairyTurn = 1;

				healthbar->GainNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = true;
				AttackStackCorrosion = 2;
				AttackStackWeakening = 2;
				AttackStackBlooding = 2;
			}
			else if (FairyTurn == 1) // 피해15, 적중 시 올 디버프 1 부여
			{
				finaldamage = 15 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 2;
					InBreathe = false;
				}

				FairyTurn = 2;

				healthbar->GainNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = true;
				AttackStackCorrosion = 1;
				AttackStackWeakening = 1;
				AttackStackBlooding = 1;
			}
			else if (FairyTurn == 2) // 본인 체력 50퍼 깎음. 깎은 체력의 3배만큼 피해
			{
				finaldamage = ((ShowCurrentHP() / 2) * 3) + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 2;
					InBreathe = false;
				}

				FairyTurn = 3;
				FairyLoseHPValue = ShowCurrentHP() / 2;

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = false;
			}
			else if (FairyTurn == 3) // 피해30, 스스로 사망
			{
				finaldamage = 30 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 2;
					InBreathe = false;
				}

				FairyTurn = 4;

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = false;
			}
		}
		}
		else if (ActorHasTag("Baphomet"))
		{
			//체력40, 공격패턴 : 피해20+적중 시 출혈3 / 피해0+출혈10부여 / 출혈 15 이상일 시, 출혈 전부 제거하고 그 두배의 피해를 줌
			int x = RandTwo(rng);

			AttackType = "Strong";

			if (healthbar)
			{
				if (MyPlayer->GetStackBlooding >= 15) // 출혈 전부 제거 및 그 두배의 피해
				{
					finaldamage = (MyPlayer->GetStackBlooding * 2) + BaphometValue + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					MyPlayer->GetStackBlooding = 0;
					MyPlayer->UpdateNagativeEffect();
					BaphometValue += 1;

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 0) // 피해20, 적중 시 출혈3
				{
					finaldamage = 20 + BaphometValue + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					BaphometValue += 1;

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackBlooding = 3;
				}
				else if (x == 1) // 피해0, 출혈 10 부여
				{
					finaldamage = 0 + BaphometValue + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					BaphometValue += 1;

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 3;
					ConfirmedDebuffValue = 10;
				}
			}
		}
		else if (ActorHasTag("Durahan"))
		{
		//체력100, 공격패턴 : 힘겨루기 상태 / 피해 5 주고 적중 시 머리의 추가 데미지가 5 오름

		int x = RandTwo(rng);
		AttackType = "Strong";

		if (healthbar)
		{
			if (x == 0) // 피해 5 주고 적중시 머리의 추가 대미지 5 상승
			{
				finaldamage = 5 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 2;
					InBreathe = false;
				}

				DurahanTryUp = true;

				healthbar->LoseNagative();
				healthbar->GainPositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = false;
			}
			else if (x == 1) // 힘겨루기 상태
			{
				// 숨고르기 소모 x

				DurahanParryingMode = true;

				finaldamage = 30;

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = false;
			}
		}
		}
		else if (ActorHasTag("MagicBall"))
		{
		//왕 2페이즈가 던지는 것

		int x = RandSeven(rng);
		AttackType = "Strong";

		if (healthbar)
		{
			if (x == 0) // 화염구 = 체력10, 대미지20
			{
				SetBall(0);

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(20);
				DamageValue = 20;
				GetAttackNag = false;
			}
			else if (x == 1) // 얼음구 = 체력15, 대미지10
			{
				SetBall(1);

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(10);
				DamageValue = 10;
				GetAttackNag = false;
			}
			else if (x == 2) // 저주구 = 체력10, 대미지10, 적중 시 약화 2
			{
				SetBall(2);

				healthbar->GainNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(10);
				DamageValue = 10;
				GetAttackNag = true;
				AttackStackWeakening = 2;
			}
			else if (x == 3) // 산성구 = 체력10, 대미지10, 적중 시 부식 2
			{
				SetBall(3);

				healthbar->GainNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(10);
				DamageValue = 10;
				GetAttackNag = true;
				AttackStackCorrosion = 2;
			}
			else if (x == 4) // 혈구 = 체력10, 대미지10, 적중 시 출혈 3
			{
				SetBall(4);

				healthbar->GainNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(10);
				DamageValue = 10;
				GetAttackNag = true;
				AttackStackBlooding = 3;
			}
			else if (x == 5) // 강철구 = 체력30, 대미지5
			{
				SetBall(5);

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(5);
				DamageValue = 5;
				GetAttackNag = false;
			}
			else if (x == 6) // 전기구 = 체력5, 대미지30
			{
				SetBall(6);

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(30);
				DamageValue = 30;
				GetAttackNag = false;
			}
		}
		}
		else if (ActorHasTag("DemonLordHead"))
		{
		//체력150, 공격패턴 : 피해15+ 적중 시 취약or약화or출혈을 2 부여
		int x = RandThree(rng);

		AttackType = "Bite";

		if (healthbar)
		{
			if (x == 0) // 피해15, 적중 시 부식2
			{
				finaldamage = 15 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 15;
					InBreathe = false;
				}

				healthbar->GainNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = true;
				AttackStackCorrosion = 2;
			}
			else if (x == 1) // 피해15, 적중 시 약화2
			{
				finaldamage = 15 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 15;
					InBreathe = false;
				}

				healthbar->GainNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = true;
				AttackStackWeakening = 2;
			}
			else if (x == 2) // 피해15, 적중 시 출혈2
			{
				finaldamage = 15 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 15;
					InBreathe = false;
				}

				healthbar->GainNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = true;
				AttackStackBlooding = 2;
			}
		}
		}
		else if (ActorHasTag("DemonLordLeft"))
		{
		//체력100, 공격패턴 : 피해15, 피해10 + 모든 부위 피해량 1 증가, 강타 버프 얻음
		int x = 0;

		if (GetPowerHit)
		{
			x = RandTwo(rng);
		}
		else
		{
			x = RandThree(rng);
		}

		AttackType = "Strong";

		bool superhiting1 = false;
		bool superhiting2 = false;
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && (Enemy->ActorHasTag("Enemy1")) && Enemy->Life)
			{
				if (Enemy->GetPowerHit)
				{
					superhiting1 = true;
				}
			}
		}
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && (Enemy->ActorHasTag("Enemy3")) && Enemy->Life)
			{
				if (Enemy->GetPowerHit)
				{
					superhiting2 = true;
				}
			}
		}

		if (healthbar)
		{
			if (superhiting1 && superhiting2) // 피해 30
			{
				finaldamage = 30 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 30;
					InBreathe = false;
				}

				if (GetPowerHit)
				{
					finaldamage = round(float(finaldamage) * 1.25);
				}

				SoonLosePower = true;

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = false;
			}
			else if (x == 0) // 피해15
			{
				finaldamage = 15 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 15;
					InBreathe = false;
				}

				if (GetPowerHit)
				{
					finaldamage = round(float(finaldamage) * 1.25);
				}

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = false;
			}
			else if (x == 1) // 피해10, 모든 부위 피해량 1 증가
			{
				finaldamage = 10 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 10;
					InBreathe = false;
				}

				if (GetPowerHit)
				{
					finaldamage = round(float(finaldamage) * 1.25);
				}

				AllStrongBuff = true;

				healthbar->LoseNagative();
				healthbar->GainPositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = false;
			}
			else if (x == 2) // 강타버프
			{
				SoonPowerHit = true;

				healthbar->LoseNagative();
				healthbar->GainPositive();
				healthbar->AttackSet(0);
				DamageValue = 0;
				GetAttackNag = false;
			}
		}
		}
		else if (ActorHasTag("DemonLordRight"))
		{
		//체력100, 공격패턴 : 피해15, 피해10 + 체력 10 회복, 강타 버프 얻음
		int x = 0;

		if (GetPowerHit)
		{
			x = RandTwo(rng);
		}
		else
		{
			x = RandThree(rng);
		}

		AttackType = "Strong";

		bool superhiting1 = false;
		bool superhiting2 = false;
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && (Enemy->ActorHasTag("Enemy1")) && Enemy->Life)
			{
				if (Enemy->GetPowerHit)
				{
					superhiting1 = true;
				}
			}
		}
		for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			AEnemyCharacter* Enemy = *Iter;

			if (Enemy != nullptr && (Enemy->ActorHasTag("Enemy3")) && Enemy->Life)
			{
				if (Enemy->GetPowerHit)
				{
					superhiting2 = true;
				}
			}
		}

		if (healthbar)
		{
			if (superhiting1 && superhiting2) // 피해 30
			{
				finaldamage = 30 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 30;
					InBreathe = false;
				}

				if (GetPowerHit)
				{
					finaldamage = round(float(finaldamage) * 1.25);
				}

				SoonLosePower = true;

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = false;
			}
			else if (x == 0) // 피해15
			{
				finaldamage = 15 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 15;
					InBreathe = false;
				}

				if (GetPowerHit)
				{
					finaldamage = round(float(finaldamage) * 1.25);
				}

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = false;
			}
			else if (x == 1) // 피해10, 체력 10 회복
			{
				finaldamage = 10 + BonusDamageValue;
				if (InBreathe)
				{
					finaldamage += 10;
					InBreathe = false;
				}

				if (GetPowerHit)
				{
					finaldamage = round(float(finaldamage) * 1.25);
				}

				TurnEndHeal = 10;

				healthbar->LoseNagative();
				healthbar->GainPositive();
				healthbar->AttackSet(finaldamage);
				DamageValue = finaldamage;
				GetAttackNag = false;
			}
			else if (x == 2) // 강타버프
			{
				SoonPowerHit = true;

				healthbar->LoseNagative();
				healthbar->GainPositive();
				healthbar->AttackSet(0);
				DamageValue = 0;
				GetAttackNag = false;
			}
		}
		}
		else if (ActorHasTag("SkeletonCenturion"))
		{
			//체력200, 공격패턴 : 피해20+적중 시 출혈5 / 피해10+직접 피해 무효화 / 피해20+적중 시 체력 5 회복
			int x = RandThree(rng);

			AttackType = "Strong";

			if (healthbar)
			{
				if (x == 0) // 피해20, 출혈5
				{
					finaldamage = 20 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					FlagOnBlue = false;
					healthbar->CenturionFlagOn(0);

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackBlooding = 5;
				}
				else if (x == 1) // 피해10, 직접 피해 무효화
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					FlagOnBlue = true;
					healthbar->CenturionFlagOn(1);

					healthbar->LoseNagative();
					healthbar->GainPositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 2) // 피해20, 체력 5 회복
				{
					finaldamage = 20 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					TurnEndHeal = 5;

					FlagOnBlue = false;
					healthbar->CenturionFlagOn(2);

					healthbar->LoseNagative();
					healthbar->GainPositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
			}
		}
	}

}

void AEnemyCharacter::PlayHealEffect(int value)
{
	if (IamWarlock)
	{
		return;
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->PlayHealEffect(value);
					}
				}
			}
		}
	}
}

int AEnemyCharacter::ShowLoseHP()
{
	int xzrs = 0;

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						xzrs = healthbar->ShowLoseHP();
					}
				}
			}
		}
	}

	return xzrs;
}

int AEnemyCharacter::ShowCurrentHP()
{
	int xzrs = 0;

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						xzrs = healthbar->ShowCurrentHP();
					}
				}
			}
		}
	}

	return xzrs;
}

void AEnemyCharacter::CleanzeDebuff()
{
	GetStackCorrosion = 0; // 보유 디버프 스택
	GetStackWeakening = 0;
	GetStackBlooding = 0;
	ConfirmedStackCorrosion = 0; // 확정 관련 디버프 스택
	ConfirmedStackWeakening = 0;
	ConfirmedStackBlooding = 0;
}

void AEnemyCharacter::LoseHP(int value)
{
	if (IamWarlock)
	{
		return;
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->CurrentHealth -= value;
						if (healthbar->CurrentHealth < 0)
						{
							healthbar->CurrentHealth = 0;
						}
						healthbar->UpdateHealthBar(0);
					}
				}
			}
		}
	}
}

void AEnemyCharacter::SetBall(int idx)
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (idx == 0)
					{
						healthbar->StatSet(10);
						healthbar->SetMobImage("Ball1");
					}
					else if (idx == 1)
					{
						healthbar->StatSet(15);
						healthbar->SetMobImage("Ball2");
					}
					else if (idx == 2)
					{
						healthbar->StatSet(10);
						healthbar->SetMobImage("Ball3");
					}
					else if (idx == 3)
					{
						healthbar->StatSet(10);
						healthbar->SetMobImage("Ball4");
					}
					else if (idx == 4)
					{
						healthbar->StatSet(10);
						healthbar->SetMobImage("Ball5");
					}
					else if (idx == 5)
					{
						healthbar->StatSet(30);
						healthbar->SetMobImage("Ball6");
					}
					else if (idx == 6)
					{
						healthbar->StatSet(5);
						healthbar->SetMobImage("Ball7");
					}
				}
			}
		}
	}
}

void AEnemyCharacter::ReviveMawang()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
					UE_LOG(LogTemp, Log, TEXT("Enemy Cast1"));
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
					UE_LOG(LogTemp, Log, TEXT("Enemy Cast2"));
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
					UE_LOG(LogTemp, Log, TEXT("Enemy Cast3"));
				}

				if (healthbar)
				{
					if (ActorHasTag("DemonLordHead"))
					{
						healthbar->StathalfSet(150);
						healthbar->SetMobImage("DemonLordHead");
					}
					else if (ActorHasTag("DemonLordLeft"))
					{
						healthbar->StathalfSet(100);
						healthbar->SetMobImage("DemonLordLeft");
					}
					else if (ActorHasTag("DemonLordRight"))
					{
						healthbar->StathalfSet(100);
						healthbar->SetMobImage("DemonLordRight");
					}
				}

			}
		}
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AEnemyCharacter::CalculateTurnEnd(int mobnum, int Damage, int Card, int Order, bool BreatheOn, bool GetCorrosion, bool EnemyCorrosion, bool GetWeaken, bool EnemyWeaken)
{
	if (IamWarlock)
	{
		return 0;
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						if (Card == 2) // 확정 출혈 4
						{
							ConfirmedStackBlooding = 4;
						}
						else if (Card == 6 && Order == 1) // 선공시 확정 약화 2
						{
							ConfirmedStackWeakening = 2;
						}
						else if (Card == 6 && Order == 2) // 패링시 확정 부식 2
						{
							ConfirmedStackCorrosion = 2;
						}
						else if (Card == 7 || Card == 107) // 확정 약화 2
						{
							ConfirmedStackWeakening = 2;
						}
						else if (Card == 10 || Card == 110) // 확정 약화,출혈,부식 2 부여
						{
							ConfirmedStackCorrosion = 2;
							ConfirmedStackWeakening = 2;
							ConfirmedStackBlooding = 2;
						}
						else if (Card == 19) // 확정 출혈2, 대상 출혈 보유시 추가 출혈2
						{
							ConfirmedStackBlooding = 2;
							if (GetStackBlooding > 0)
							{
								ConfirmedStackBlooding += 2;
							}
						}
						else if (Card == 119) // 확정 출혈3, 대상 출혈 보유시 추가 출혈3
						{
							ConfirmedStackBlooding = 3;
							if (GetStackBlooding > 0)
							{
								ConfirmedStackBlooding += 3;
							}
						}
						else if (Card == 24) // 확정 부식2
						{
							ConfirmedStackCorrosion = 2;
						}
						else if (Card == 124) // 확정 부식3
						{
							ConfirmedStackCorrosion = 3;
						}
						else if (Card == 32 && Card == 132) // 확정 출혈 2 부여
						{
							if (MyPlayer->Combo_Madness == 2 || MyPlayer->Combo_Madness == 3)
							{
								ConfirmedStackBlooding += 2;
							}
						}
						else if (Card == 34 && Order == 1) // 확정 약화,출혈,부식 2 부여
						{
							ConfirmedStackCorrosion = 2;
							ConfirmedStackWeakening = 2;
							ConfirmedStackBlooding = 2;
						}
						else if (Card == 41 || Card == 141) // 출혈 부여
						{
							if (Card == 41)
							{
								ConfirmedStackBlooding = 2;
								if (MyPlayer->Combo_Madness == 2 || MyPlayer->Combo_Madness == 3)
								{
									ConfirmedStackBlooding += 2;
								}
							}
							else if (Card == 141)
							{
								ConfirmedStackBlooding = 4;
								if (MyPlayer->Combo_Madness == 2 || MyPlayer->Combo_Madness == 3)
								{
									ConfirmedStackBlooding += 2;
								}
							}
						}
						else if (Card == 42 || Card == 142) // 확정 출혈 4 부여
						{
							if (Card == 42)
							{
								ConfirmedStackBlooding = 4;
								if (MyPlayer->Combo_Madness == 2 || MyPlayer->Combo_Madness == 3)
								{
									ConfirmedStackBlooding += 2;
								}
							}
							else if (Card == 142)
							{
								ConfirmedStackBlooding = 6;
								if (MyPlayer->Combo_Madness == 2 || MyPlayer->Combo_Madness == 3)
								{
									ConfirmedStackBlooding += 2;
								}
							}
						}
						else if (Card == 43 || Card == 143) // 확정 출혈 2 부여
						{
							if (Card == 43)
							{
								ConfirmedStackBlooding = 2;
								if (MyPlayer->Combo_Madness == 2 || MyPlayer->Combo_Madness == 3)
								{
									ConfirmedStackBlooding += 2;
								}
							}
							else if (Card == 143)
							{
								ConfirmedStackBlooding = 4;
								if (MyPlayer->Combo_Madness == 2 || MyPlayer->Combo_Madness == 3)
								{
									ConfirmedStackBlooding += 2;
								}
							}
						}
						else if (Card == 44 || Card == 144) // 피의 축제 / 선택한 적의 출혈만큼 모든 적에게 출혈 부여
						{
							int BloodingStack = ConfirmedStackBlooding;

							for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
							{
								AEnemyCharacter* Enemy = *Iter;

								if (Enemy != nullptr)
								{
									if (Enemy->ActorHasTag("Enemy1") && Enemy->Life)
									{
										Enemy->ConfirmedStackBlooding += BloodingStack;
									}
									else if (Enemy->ActorHasTag("Enemy2") && Enemy->Life)
									{
										Enemy->ConfirmedStackBlooding += BloodingStack;
									}
									else if (Enemy->ActorHasTag("Enemy3") && Enemy->Life)
									{
										Enemy->ConfirmedStackBlooding += BloodingStack;
									}
								}
							}
						}
						else if (Card == 102) // 확정 출혈 6
						{
							ConfirmedStackBlooding = 6;
						}
						else if (Card == 106 && Order == 1) // 선공시 확정 약화 3
						{
							ConfirmedStackWeakening = 3;
						}
						else if (Card == 106 && Order == 2) // 패링시 확정 부식 3
						{
							ConfirmedStackCorrosion = 3;
						}
						else if (Card == 134 && Order == 1) // 확정 약화,출혈,부식 2 부여
						{
							ConfirmedStackCorrosion = 3;
							ConfirmedStackWeakening = 3;
							ConfirmedStackBlooding = 3;
						}
						else if (Card == 148) // 확정 부식 2 부여
						{
							ConfirmedStackCorrosion = 2;
						}

						if (Order == 1)
						{
							if (FirstStrikeSound != nullptr)
							{
								UGameplayStatics::PlaySound2D(this, FirstStrikeSound);
							}
						}
						else if (Order == 2)
						{
							if (ParryingSound != nullptr)
							{
								UGameplayStatics::PlaySound2D(this, ParryingSound);
							}
						}
						else if (Order == 3)
						{
							if (CounterAttackSound != nullptr)
							{
								UGameplayStatics::PlaySound2D(this, CounterAttackSound);
							}
						}

						int kk = RandFour(rng); // 25퍼 확률

						if (kk == 3)
						{
							if (AForgottenKnightCharacter::MyAccessoryData[2] == 2) // 출혈
							{
								ConfirmedStackBlooding += 1;
							}
							else if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // 부식
							{
								ConfirmedStackCorrosion += 1;
							}
							else if (AForgottenKnightCharacter::MyAccessoryData[2] == 4) // 약화
							{
								ConfirmedStackWeakening += 1;
							}
						}

						return healthbar->TurnEnd(mobnum, Damage, Card, Order, BreatheOn, GetCorrosion, EnemyCorrosion, GetWeaken, EnemyWeaken);
					}
				}
			}
		}
	}
	return false;
}

void AEnemyCharacter::OnBreatheEffect(int Turn)
{
	if (IamWarlock)
	{
		return;
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->OnBreatheEffect(Turn);
					}
				}
			}
		}
	}
}

void AEnemyCharacter::ChangeWidgetImage(int CardNum, int Order)
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->ConnectImage(CardNum, Order);
					}
				}
			}
		}
	}
}

bool AEnemyCharacter::LifeCheck()
{
	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar->CurrentHealth > 0)
					{
						Life = true;
						return true;
					}
					else
					{
						Life = false;
						return false;
					}
				}
			}
		}
	}

	return true;
}

void AEnemyCharacter::ChangeBreatheEffect(bool TurnOn)
{
	if (IamWarlock)
	{
		return;
	}

	AForgottenKnightCharacter* MyPlayer = Cast<AForgottenKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyPlayer)
	{
		AController_MainCharacter* MyController = Cast<AController_MainCharacter>(MyPlayer->Controller);
		if (MyController)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				UEnemyHealthBar* healthbar = nullptr;
				if (ActorHasTag("Enemy1"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar1);
				}
				else if (ActorHasTag("Enemy2"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar2);
				}
				else if (ActorHasTag("Enemy3"))
				{
					healthbar = Cast<UEnemyHealthBar>(MyWindow->EnemyHealthbar3);
				}

				if (healthbar)
				{
					if (healthbar)
					{
						healthbar->ChangeBreatheEffect(TurnOn);
					}
				}
			}
		}
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

