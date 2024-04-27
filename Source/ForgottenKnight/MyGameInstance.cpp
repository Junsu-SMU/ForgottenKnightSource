// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "DataTable_Dialogue.h"

UMyGameInstance::UMyGameInstance()
{
	FString DialogueDataPath = TEXT("DataTable'/Game/Widget/Widget_UI/Outside/DataTable_Dialogue1'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Dialogue(*DialogueDataPath);
	if (DT_Dialogue.Succeeded())
	{
		DialogueTable = DT_Dialogue.Object;
	}
}

FDialogueTableRow* UMyGameInstance::GetDialogueData(int DialNum)
{
	return DialogueTable->FindRow<FDialogueTableRow>(*FString::FromInt(DialNum), FString(""));
}
