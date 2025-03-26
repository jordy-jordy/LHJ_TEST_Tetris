// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleHUD.h"
#include "Blueprint/UserWidget.h"


void ATitleHUD::BeginPlay()
{
	Super::BeginPlay();

	if (TitleMenuWidgetClass)
	{
		TitleMenuWidget = CreateWidget<UUserWidget>(GetWorld(), TitleMenuWidgetClass);
		if (TitleMenuWidget)
		{
			TitleMenuWidget->AddToViewport();

			// 인풋 모드 전환
			if (APlayerController* PC = GetOwningPlayerController())
			{
				PC->bShowMouseCursor = true;

				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(TitleMenuWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

				PC->SetInputMode(InputMode);
			}
		}
	}
}

