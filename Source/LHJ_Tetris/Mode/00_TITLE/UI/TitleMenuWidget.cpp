// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_TITLE/UI/TitleMenuWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Global/GlobalConst.h"


void UTitleMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UTitleMenuWidget::OnStartClicked);
	}
	if (ExitGameButton)
	{
		ExitGameButton->OnClicked.AddDynamic(this, &UTitleMenuWidget::OnExitGame);
	}
}

void UTitleMenuWidget::OnStartClicked()
{
	if (!Input_Width || !Input_Height) return;

	int32 Width = FCString::Atoi(*Input_Width->GetText().ToString());
	int32 Height = FCString::Atoi(*Input_Height->GetText().ToString());

	UGlobalConst::SetMapSize(Width, Height);

	UGameplayStatics::OpenLevel(this, TEXT("PlayLevel")); // 원하는 레벨명으로 변경
}

void UTitleMenuWidget::OnExitGame()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
}
