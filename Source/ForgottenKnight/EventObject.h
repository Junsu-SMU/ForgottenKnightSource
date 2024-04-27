// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventObject.generated.h"

UCLASS()
class FORGOTTENKNIGHT_API AEventObject : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitBox, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BoxComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* EnterSound;
	UPROPERTY(VisibleAnyWhere)
		class UWidgetComponent* InteractWidgetComp;
	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* InteractSound;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:	
	AEventObject();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
