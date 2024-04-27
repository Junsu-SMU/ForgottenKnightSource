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
std::uniform_int_distribution<int> RandTwo(0, 1); // [0, 1] ����
std::uniform_int_distribution<int> RandThree(0, 2); // [0, 2] ����
std::uniform_int_distribution<int> RandFour(0, 3); // [0, 3] ����
std::uniform_int_distribution<int> RandFive(0, 4); // [0, 4] ����
std::uniform_int_distribution<int> RandSeven(0, 6); // [0, 6] ����

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
					else if (ActorHasTag("Mantis")) //�縶��
					{
						healthbar->StatSet(15);
						healthbar->SetMobImage("Mantis");
					}
					else if (ActorHasTag("Mantis_Up")) //�縶��
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
						healthbar->StatSet(20); // �� 20
						healthbar->SetMobImage("Asura");
					}
					else if (ActorHasTag("Asura_Up"))
					{
						healthbar->StatSet(27); // �� 20
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
					else if (ActorHasTag("Random")) // ���� �ذ� ��ȯ(����4-3����)
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

	AttackOrder = 2; // ���� ���� : 1, ǥ�� : 2, ���� ���� : 3
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
			//ü��10, �������� : ����2+���� �� �ν�2 / ����4 / ����2+���� �� ��ȭ2
			int x = RandThree(rng);

			AttackType = "Bite";

			if (healthbar)
			{
				if (x == 0) // ����2, �ν�2
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
				else if (x == 1) // ����4
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
				else if (x == 2) // ����2, ��ȭ2
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
			//ü��7, �������� : ����3 ���� �� ����2 / ����6
			int x = RandTwo(rng);

			AttackType = "Knife2";

			if (healthbar)
			{
				if (x == 0) // ����3 ���� �� ����2
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
				else if (x == 1) // ����6
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
			//ü��25, �������� : ����6

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
		else if (ActorHasTag("Spider")) // �� 1/3 ���� ������ ���� ����� 1/3�� �۾���
		{
			//ü��15, �������� : ����3 ��ȭ3 / ����9
			int x = RandTwo(rng);
			float y = float(healthbar->CurrentHealth) / float(healthbar->MaxHealth);

			AttackType = "Bite";

			if (healthbar)
			{
				if (x == 0) // ����3 ��ȭ3
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
				else if (x == 1) // ����9
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
			//ü��15, �������� : ����4 ����2 / ����1 ����6 (��������10�̻��� �� ���� 15 ��)
			int x = RandTwo(rng);

			AttackType = "Knife";

			if (healthbar)
			{
				if (MyPlayer->GetStackBlooding >= 10) // ���� 15
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
				else if (x == 0) // ����4 ����2
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
				else if (x == 1) // ����1 ����6
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
			//ü��30, �������� : ����8 ����2 / ����2 ����6 (��������10�̻��� �� ���� 30 ��)
			int x = RandTwo(rng);

			AttackType = "Knife";

			if (healthbar)
			{
				if (MyPlayer->GetStackBlooding >= 10) // ���� 30
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
				else if (x == 0) // ����8 ����2
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
				else if (x == 1) // ����2 ����6
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
			//ü��40, �������� : ����10 / ���� �� �ൿx

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
					// �ൿ ���ϹǷ� ������ �Ҹ� x

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
			//ü��20, �������� : ����10 / ����6 ����2 / ����8 ��ȭ2 / ����8 �ν�2 / ���� 2 ������
			//���ݹ����� �ϳ��� ������ : ����8 ����4 -> ������ -> ����14
			int x = RandFive(rng);
			//int NumEnemy = MyController->ConfirmEnemyNum();

			if (healthbar)
			{
				if (AsuraRevivemode && AsuraOrder == 1) // ����8 ����4
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
				else if (AsuraRevivemode && AsuraOrder == 2) // ������
				{
					// ������ �ƴϹǷ� ������ �Ҹ� x

					AttackType = "Breath";

					healthbar->LoseNagative();
					healthbar->GainPositive();
					healthbar->AttackSet(0);
					DamageValue = 0;
					GetAttackNag = false;

					AsuraOrder = 3;

					ConfirmedBreathe = true;
				}
				else if (AsuraRevivemode && AsuraOrder == 3) // ����14
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
				else if (x == 0) // ����6 ����2
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
				else if (x == 1) // ����10
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
				else if (x == 2) // ����8 ��ȭ2
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
				else if (x == 3) // ����8 �ν�2
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
				else if (x == 4) // ����2 ������
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
			//�������� ü��, ����� 30�� ����
			//ü��27, �������� : ����13 / ����8 ����2 / ����11 ��ȭ2 / ����11 �ν�2 / ���� 3 ������
			//���ݹ����� �ϳ��� ������ : ����11 ����4 -> ������ -> ����19
			int x = RandFive(rng);
			//int NumEnemy = MyController->ConfirmEnemyNum();

			if (healthbar)
			{
				if (AsuraRevivemode && AsuraOrder == 1) // ����11 ����4
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
				else if (AsuraRevivemode && AsuraOrder == 2) // ������
				{
					// ������ �ƴϹǷ� ������ �Ҹ� x

					AttackType = "Breath";

					healthbar->LoseNagative();
					healthbar->GainPositive();
					healthbar->AttackSet(0);
					DamageValue = 0;
					GetAttackNag = false;

					AsuraOrder = 3;

					ConfirmedBreathe = true;
				}
				else if (AsuraRevivemode && AsuraOrder == 3) // ����19
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
				else if (x == 0) // ����8 ����2
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
				else if (x == 1) // ����13
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
				else if (x == 2) // ����11 ��ȭ2
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
				else if (x == 3) // ����11 �ν�2
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
				else if (x == 4) // ����3 ������
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
			//ü��30, �������� : ����12 ���߽� �ν�2 / ����10 ���߽� ����2
			int x = RandTwo(rng);

			AttackType = "Knife2";

			if (healthbar)
			{
				if (x == 0) // ����12 ���߽� �ν�2
				{
					finaldamage = 12 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					//AttackOrder = 1; // ����
					//healthbar->SetAttackSpeed(1); // ����

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					AttackStackCorrosion = 2;
				}
				else if (x == 1) // ����10 ���߽� ����2
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
			//ü��50, �������� : ����10 �ְ� ü�� 3 ȸ��

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
			//ü��40, �������� : ������(����x) / ����10 // ȥ�� ������ ����15+������
			int x = RandTwo(rng);

			if (ShadowAxePattern) // �� �� �������̸�
			{
				x = 0; // ���� 10
			}
			else
			{
				x = RandTwo(rng); // ���� 2��
			}

			AttackType = "Knife2";

			if (healthbar)
			{
				int FieldMob = 0; // �ʵ忡 �ִ� ���� �� ��
				for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
				{
					AEnemyCharacter* Enemy = *Iter;

					if (Enemy != nullptr && Enemy->Life)
					{
						FieldMob += 1;
					}
				}

				if (FieldMob == 1) // �� ȥ�� ������ �� : ����15 + ������
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
				else if (x == 0) // ����10
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
				else if (x == 1) // ������
				{
					// ������ �ƴϹǷ� ������ �Ҹ� x

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
			//ü��35, �������� : ����10 ������ / ����8 ��������
			// ������ -> ����10 ����3 / �������� -> ����15 ��ȭ2
			int x = RandTwo(rng);

			AttackType = "Strong";

			if (healthbar)
			{
				if (GriffonCondition == 1) // ������ ���� : ����10, ����3
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					//AttackOrder = 1; // ����
					//HealthBarWidget->SetAttackSpeed(1); // ����

					GriffonCondition = 0; // ��� ���� ����

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 3;
					ConfirmedDebuffValue = 3;
				}
				else if (GriffonCondition == 2) // �������� ���� : ���� 15, ��ȭ2
				{
					finaldamage = 15 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					//AttackOrder = 3; // ����
					//HealthBarWidget->SetAttackSpeed(3); // ����

					GriffonCondition = 0; // ��� ���� ����

					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = true;
					ConfirmedDebuffType = 2;
					ConfirmedDebuffValue = 2;
				}
				else if (x == 0) // ����10 ������ ����
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					GriffonCondition = 1; // 1 : ������ ����

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 1) // ����8 �������� ����
				{
					finaldamage = 8 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					GriffonCondition = 2; // 2 : �������� ����

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
			//ü��10, �������� : ����5,������ / ����5 ��ȭ3 / ����5 �ν�3

			int x = 0;
			if (IamScent) // ���� �������̸�
			{
				x = RandTwo(rng); // ���� 2��
			}
			else // ��� ���� �ƴϸ�
			{
				x = RandThree(rng); // ���� 3��
			}

			AttackType = "Bite"; // ���� ȿ���� Ÿ��

			if (healthbar)
			{
				if (x == 0) // ����5 �ν�3
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
				else if (x == 1) // ����5 ��ȭ3
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
				else if (x == 2) // ����5, ������
				{
					finaldamage = 5 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					ConfirmedScent = true; // ���� �� ��� ����

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
			//1������ ü��100, ������ - �渶 - ��� ����, �ڽ� ����(�ϼ��� ���� ������ ü�� -10), �ϼ��� ���� �� ���� ���� �ϼ��� ��ȯ
			// Ʈ�� -> �Ź̹��� -> �� -> �׸��� â�� -> �׸��� ���к� -> �׸��� ������ -> �׸��� -> ���÷��þ� �� 10���� �ϼ��� ��� �� 1������ ����
			//2������ �̺��ν� ü��90(�Ӹ� �ϳ��� 30)
			// ù�Ͽ� ���� 10�� ���� �� ��ȭ(���� �Ӹ�), �ν�(��� �Ӹ�), ����(������ �Ӹ�) 3 �ο�
			// �߰� �������� ���� 5+(����� ���� ��ŭ +5) ���ظ� ��.

			//�� ��� Ƚ��(�ϼ��� 10���� �� �̺��ν� �Ӹ� 3��)

			int x = RandFour(rng);

			AttackType = "Normal2";

			if (healthbar)
			{
				if (WarlockAttackmode && ActorHasTag("Enemy1") && WarlockAttackmodeFirst) // 2������ ù ���� ���� �Ӹ��� ��, ����10 ���� �� ��ȭ3
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
				else if (WarlockAttackmode && ActorHasTag("Enemy1") && WarlockAttackmodeFirst) // 2������ ù ���� ��� �Ӹ��� ��, ����10 ���� �� �ν�3
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
				else if (WarlockAttackmode && ActorHasTag("Enemy1") && WarlockAttackmodeFirst) // 2������ ù ���� ������ �Ӹ��� ��, ����10 ���� �� ����3
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
				else if (WarlockAttackmode && (x == 0)) // ����10, ���� �� ��ȭ3
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
				else if (WarlockAttackmode && (x == 1)) // ����10, ���� �� �ν�3
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
				else if (WarlockAttackmode && (x == 2)) // ����10, ���� �� ����3
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
				else if (WarlockAttackmode && (x == 3)) // ����5 + ����� ���� ��ŭ ����� 5 �߰�(�������� �ȳ���)
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
				else if (!WarlockAttackmode) // 1������ �� ��
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
		//�������� : �ൿx

		if (healthbar)
		{
			// �ൿ ���ϹǷ� ������ �Ҹ� x
			healthbar->LoseNagative();
			healthbar->LosePositive();
			healthbar->AttackSet(0);
			DamageValue = 0;
			GetAttackNag = false;
		}
		}
		else if (ActorHasTag("SkeletonSoldier"))
		{
			//ü��10, �������� : ����15 / ����8+���� �� ��ȭ2, �ν�2
			int x = RandTwo(rng);

			AttackType = "Knife2";

			if (healthbar)
			{
				if (x == 0) // ����15
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
				else if (x == 1) // ����8+���� �� ��ȭ2, �ν�2
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
			//ü��10, �������� : ����10+���� �� ����2 / ���� 6 �ι�(�ι�° ������ �и��� �ȵ�)
			int x = RandTwo(rng);

			AttackType = "Knife2";

			if (healthbar)
			{
				if (x == 0) // ����10+���� �� ����2
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
				else if (x == 1) // ���� 6 �ι�(�ι�° ������ �и��� �ȵ�)
				{
					finaldamage = 6 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					AttackTryNum = 2; // 2�� ����

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
			//ü��10, �������� : ����30(�и� ���� �� ����) / ����15+���� ������� �ϳ� �Ǵ�.(�� ��� �ɷ� ������ ����2)
			int x = RandTwo(rng);

			AttackType = "Knife2";

			if (healthbar)
			{
				if (LastTurnParrying && SkulldogSpecial) // ���� �Ͽ� ��� ���� ��������
				{ // ����
					SkulldogSpecial = false;
					LastTurnParrying = false;

					// �ൿ ���ϹǷ� ������ �Ҹ� x

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(0);
					DamageValue = 0;
					GetAttackNag = false;
					GolemDelay = false;
				}
				else if (x == 0) // ����30(�и� ���� �� ����)
				{
					finaldamage = 30 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					SkulldogSpecial = true; // Ư�� ��� on

					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 1) // ����15+���� ������� �ϳ� �Ǵ�.(�� ��� �ɷ� ������ ����2)
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
					{ // �÷��̾ ������� �����ϰ� ������
						if (b > 0 && w == 0 && c == 0)
						{ // ������ ������ ����
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
						{ // ��ȭ�� ������ ����
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
						{ // �νĸ� ������ ����
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
						{ // ����, ��ȭ�� ������ ����
							ConfirmedDebuffType = 1;
							ConfirmedDebuffValue = 2;
						}
						else if (b > 0 && w == 0 && c > 0)
						{ // ����, �ν��� ������ ����
							ConfirmedDebuffType = 2;
							ConfirmedDebuffValue = 2;
						}
						else
						{ // �ν�, ��ȭ�� ������ �ְų� ��� ����� ����
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
		//ü��80, �������� : ����10, ���� �� �õ����2�ο� / ���Ͱ� �ϳ��� �׾����� ����Ȯ���� �ذ� ��ȯ ��� ����
		int x = RandTwo(rng);

		AttackType = "Strong";

		if (healthbar)
		{
			UUI_PlayWindow* MyWindow = Cast<UUI_PlayWindow>(MyController->UI_PlayWindow);
			if (MyWindow)
			{
				if ((!MyWindow->EnemyLife[1] || !MyWindow->EnemyLife[2] || !MyWindow->EnemyLife[3]) && x == 1) // ���� �ذ� ��ȯ
				{ // ������ �׾����� ���� Ȯ���� ��ȯ ���

					int xx = RandThree(rng);
					LichReviveSkill = xx + 1;

					healthbar->LoseNagative();
					healthbar->GainPositive();
					healthbar->AttackSet(0);
					DamageValue = 0;
					GetAttackNag = false;
				}
				else // ����10 +  ���� �� �õ����2
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
			//ü��80, �������� : ����10 / ����20+���� �� ��Ȥ �����(�ݵ�� ����:����� ������ ���� ����)
			int x = RandTwo(rng);

			AttackType = "Strong";

			if (healthbar)
			{
				if (x == 0 && firstturnbeholder == 0) // ����20, ��Ȥ(�� �� ����)
				{
					finaldamage = 20 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					firstturnbeholder = 1; // �� �� ����

					healthbar->BeholderEye(true);
					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (x == 1 && firstturnbeholder == 0) // ����10(�� ���� ����)
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					firstturnbeholder = 2; // �� ���� ����

					healthbar->BeholderEye(false);
					healthbar->LoseNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				if (firstturnbeholder == 2) // ����20, ��Ȥ(�� �� ����)
				{
					finaldamage = 20 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					firstturnbeholder = 1; // �� �� ����

					healthbar->BeholderEye(true);
					healthbar->GainNagative();
					healthbar->LosePositive();
					healthbar->AttackSet(finaldamage);
					DamageValue = finaldamage;
					GetAttackNag = false;
				}
				else if (firstturnbeholder == 1) // ����10(�� ���� ����)
				{
					finaldamage = 10 + BonusDamageValue;
					if (InBreathe)
					{
						finaldamage += 2;
						InBreathe = false;
					}

					firstturnbeholder = 2; // �� ���� ����

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
		//ü��40, �������� : ������

		AttackType = "Bite";

		if (healthbar)
		{
			if (FairyTurn == 0) // ����12, ���� �� �� ����� 2 �ο�
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
			else if (FairyTurn == 1) // ����15, ���� �� �� ����� 1 �ο�
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
			else if (FairyTurn == 2) // ���� ü�� 50�� ����. ���� ü���� 3�踸ŭ ����
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
			else if (FairyTurn == 3) // ����30, ������ ���
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
			//ü��40, �������� : ����20+���� �� ����3 / ����0+����10�ο� / ���� 15 �̻��� ��, ���� ���� �����ϰ� �� �ι��� ���ظ� ��
			int x = RandTwo(rng);

			AttackType = "Strong";

			if (healthbar)
			{
				if (MyPlayer->GetStackBlooding >= 15) // ���� ���� ���� �� �� �ι��� ����
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
				else if (x == 0) // ����20, ���� �� ����3
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
				else if (x == 1) // ����0, ���� 10 �ο�
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
		//ü��100, �������� : ���ܷ�� ���� / ���� 5 �ְ� ���� �� �Ӹ��� �߰� �������� 5 ����

		int x = RandTwo(rng);
		AttackType = "Strong";

		if (healthbar)
		{
			if (x == 0) // ���� 5 �ְ� ���߽� �Ӹ��� �߰� ����� 5 ���
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
			else if (x == 1) // ���ܷ�� ����
			{
				// ������ �Ҹ� x

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
		//�� 2����� ������ ��

		int x = RandSeven(rng);
		AttackType = "Strong";

		if (healthbar)
		{
			if (x == 0) // ȭ���� = ü��10, �����20
			{
				SetBall(0);

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(20);
				DamageValue = 20;
				GetAttackNag = false;
			}
			else if (x == 1) // ������ = ü��15, �����10
			{
				SetBall(1);

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(10);
				DamageValue = 10;
				GetAttackNag = false;
			}
			else if (x == 2) // ���ֱ� = ü��10, �����10, ���� �� ��ȭ 2
			{
				SetBall(2);

				healthbar->GainNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(10);
				DamageValue = 10;
				GetAttackNag = true;
				AttackStackWeakening = 2;
			}
			else if (x == 3) // �꼺�� = ü��10, �����10, ���� �� �ν� 2
			{
				SetBall(3);

				healthbar->GainNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(10);
				DamageValue = 10;
				GetAttackNag = true;
				AttackStackCorrosion = 2;
			}
			else if (x == 4) // ���� = ü��10, �����10, ���� �� ���� 3
			{
				SetBall(4);

				healthbar->GainNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(10);
				DamageValue = 10;
				GetAttackNag = true;
				AttackStackBlooding = 3;
			}
			else if (x == 5) // ��ö�� = ü��30, �����5
			{
				SetBall(5);

				healthbar->LoseNagative();
				healthbar->LosePositive();
				healthbar->AttackSet(5);
				DamageValue = 5;
				GetAttackNag = false;
			}
			else if (x == 6) // ���ⱸ = ü��5, �����30
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
		//ü��150, �������� : ����15+ ���� �� ���or��ȭor������ 2 �ο�
		int x = RandThree(rng);

		AttackType = "Bite";

		if (healthbar)
		{
			if (x == 0) // ����15, ���� �� �ν�2
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
			else if (x == 1) // ����15, ���� �� ��ȭ2
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
			else if (x == 2) // ����15, ���� �� ����2
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
		//ü��100, �������� : ����15, ����10 + ��� ���� ���ط� 1 ����, ��Ÿ ���� ����
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
			if (superhiting1 && superhiting2) // ���� 30
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
			else if (x == 0) // ����15
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
			else if (x == 1) // ����10, ��� ���� ���ط� 1 ����
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
			else if (x == 2) // ��Ÿ����
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
		//ü��100, �������� : ����15, ����10 + ü�� 10 ȸ��, ��Ÿ ���� ����
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
			if (superhiting1 && superhiting2) // ���� 30
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
			else if (x == 0) // ����15
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
			else if (x == 1) // ����10, ü�� 10 ȸ��
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
			else if (x == 2) // ��Ÿ����
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
			//ü��200, �������� : ����20+���� �� ����5 / ����10+���� ���� ��ȿȭ / ����20+���� �� ü�� 5 ȸ��
			int x = RandThree(rng);

			AttackType = "Strong";

			if (healthbar)
			{
				if (x == 0) // ����20, ����5
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
				else if (x == 1) // ����10, ���� ���� ��ȿȭ
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
				else if (x == 2) // ����20, ü�� 5 ȸ��
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
	GetStackCorrosion = 0; // ���� ����� ����
	GetStackWeakening = 0;
	GetStackBlooding = 0;
	ConfirmedStackCorrosion = 0; // Ȯ�� ���� ����� ����
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
						if (Card == 2) // Ȯ�� ���� 4
						{
							ConfirmedStackBlooding = 4;
						}
						else if (Card == 6 && Order == 1) // ������ Ȯ�� ��ȭ 2
						{
							ConfirmedStackWeakening = 2;
						}
						else if (Card == 6 && Order == 2) // �и��� Ȯ�� �ν� 2
						{
							ConfirmedStackCorrosion = 2;
						}
						else if (Card == 7 || Card == 107) // Ȯ�� ��ȭ 2
						{
							ConfirmedStackWeakening = 2;
						}
						else if (Card == 10 || Card == 110) // Ȯ�� ��ȭ,����,�ν� 2 �ο�
						{
							ConfirmedStackCorrosion = 2;
							ConfirmedStackWeakening = 2;
							ConfirmedStackBlooding = 2;
						}
						else if (Card == 19) // Ȯ�� ����2, ��� ���� ������ �߰� ����2
						{
							ConfirmedStackBlooding = 2;
							if (GetStackBlooding > 0)
							{
								ConfirmedStackBlooding += 2;
							}
						}
						else if (Card == 119) // Ȯ�� ����3, ��� ���� ������ �߰� ����3
						{
							ConfirmedStackBlooding = 3;
							if (GetStackBlooding > 0)
							{
								ConfirmedStackBlooding += 3;
							}
						}
						else if (Card == 24) // Ȯ�� �ν�2
						{
							ConfirmedStackCorrosion = 2;
						}
						else if (Card == 124) // Ȯ�� �ν�3
						{
							ConfirmedStackCorrosion = 3;
						}
						else if (Card == 32 && Card == 132) // Ȯ�� ���� 2 �ο�
						{
							if (MyPlayer->Combo_Madness == 2 || MyPlayer->Combo_Madness == 3)
							{
								ConfirmedStackBlooding += 2;
							}
						}
						else if (Card == 34 && Order == 1) // Ȯ�� ��ȭ,����,�ν� 2 �ο�
						{
							ConfirmedStackCorrosion = 2;
							ConfirmedStackWeakening = 2;
							ConfirmedStackBlooding = 2;
						}
						else if (Card == 41 || Card == 141) // ���� �ο�
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
						else if (Card == 42 || Card == 142) // Ȯ�� ���� 4 �ο�
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
						else if (Card == 43 || Card == 143) // Ȯ�� ���� 2 �ο�
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
						else if (Card == 44 || Card == 144) // ���� ���� / ������ ���� ������ŭ ��� ������ ���� �ο�
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
						else if (Card == 102) // Ȯ�� ���� 6
						{
							ConfirmedStackBlooding = 6;
						}
						else if (Card == 106 && Order == 1) // ������ Ȯ�� ��ȭ 3
						{
							ConfirmedStackWeakening = 3;
						}
						else if (Card == 106 && Order == 2) // �и��� Ȯ�� �ν� 3
						{
							ConfirmedStackCorrosion = 3;
						}
						else if (Card == 134 && Order == 1) // Ȯ�� ��ȭ,����,�ν� 2 �ο�
						{
							ConfirmedStackCorrosion = 3;
							ConfirmedStackWeakening = 3;
							ConfirmedStackBlooding = 3;
						}
						else if (Card == 148) // Ȯ�� �ν� 2 �ο�
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

						int kk = RandFour(rng); // 25�� Ȯ��

						if (kk == 3)
						{
							if (AForgottenKnightCharacter::MyAccessoryData[2] == 2) // ����
							{
								ConfirmedStackBlooding += 1;
							}
							else if (AForgottenKnightCharacter::MyAccessoryData[2] == 3) // �ν�
							{
								ConfirmedStackCorrosion += 1;
							}
							else if (AForgottenKnightCharacter::MyAccessoryData[2] == 4) // ��ȭ
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

