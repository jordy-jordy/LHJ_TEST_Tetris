// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_PLAY/UI/PauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_ToTitle)
	{
		Btn_ToTitle->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnToTitle);
	}
	if (Btn_ExitGame)
	{
		Btn_ExitGame->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnExitGame);
	}
}

void UPauseMenuWidget::OnToTitle()
{
	UGameplayStatics::OpenLevel(this, TEXT("TitleLevel")); 
}

void UPauseMenuWidget::OnExitGame()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
}
