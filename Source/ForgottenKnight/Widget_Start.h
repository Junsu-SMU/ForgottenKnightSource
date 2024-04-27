// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Start.generated.h"

/**
 *
 */
UCLASS()
class FORGOTTENKNIGHT_API UWidget_Start : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		void OnStartClicked();

	UFUNCTION()
		void OnQuitClicked();

	UFUNCTION()
		void OnLoadClicked();

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FString OutsideMap;

public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Start;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Load;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Quit;

};
