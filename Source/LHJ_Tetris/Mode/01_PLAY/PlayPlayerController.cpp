// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_PLAY/PlayPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "Mode/01_PLAY/PlayGameMode.h"
#include "Mode/01_PLAY/Block.h"
#include "Mode/01_PLAY/UI/PauseMenuWidget.h"


void APlayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 마우스 커서 보이기
	bShowMouseCursor = true;

	// 인풋 모드: 게임 전용
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMapping, 0);
	}
}

void APlayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(IA_MoveLeft, ETriggerEvent::Started, this, &APlayPlayerController::OnMoveLeft);
		Input->BindAction(IA_MoveRight, ETriggerEvent::Started, this, &APlayPlayerController::OnMoveRight);
		Input->BindAction(IA_MoveDown, ETriggerEvent::Started, this, &APlayPlayerController::OnMoveDown);
		Input->BindAction(IA_RotateLeft, ETriggerEvent::Started, this, &APlayPlayerController::OnRotateLeft);
		Input->BindAction(IA_RotateRight, ETriggerEvent::Started, this, &APlayPlayerController::OnRotateRight);
		Input->BindAction(IA_Pause, ETriggerEvent::Started, this, &APlayPlayerController::OnPause);
	}
}

void APlayPlayerController::OnMoveLeft(const FInputActionInstance& Instance)
{
	if (APlayGameMode* GM = GetWorld()->GetAuthGameMode<APlayGameMode>())
	{
		if (ABlock* Block = GM->GetCurrentBlock())
		{
			Block->Move(FIntPoint(1, 0)); // 왼쪽 이동
		}
	}
}

void APlayPlayerController::OnMoveRight(const FInputActionInstance& Instance)
{
	if (APlayGameMode* GM = GetWorld()->GetAuthGameMode<APlayGameMode>())
	{
		if (ABlock* Block = GM->GetCurrentBlock())
		{
			Block->Move(FIntPoint(-1, 0)); // 오른쪽 이동
		}
	}
}

void APlayPlayerController::OnMoveDown(const FInputActionInstance& Instance)
{
	if (APlayGameMode* GM = GetWorld()->GetAuthGameMode<APlayGameMode>())
	{
		if (ABlock* Block = GM->GetCurrentBlock())
		{
			Block->TryMoveDown(); // 여기서 자동으로 이동하거나, 고정 + 새 블록
		}
	}
}

void APlayPlayerController::OnRotateLeft(const FInputActionInstance& Instance)
{
	if (APlayGameMode* GM = GetWorld()->GetAuthGameMode<APlayGameMode>())
	{
		if (ABlock* Block = GM->GetCurrentBlock())
		{
			Block->Rotate(false); // 반시계 방향
		}
	}
}

void APlayPlayerController::OnRotateRight(const FInputActionInstance& Instance)
{
	if (APlayGameMode* GM = GetWorld()->GetAuthGameMode<APlayGameMode>())
	{
		if (ABlock* Block = GM->GetCurrentBlock())
		{
			Block->Rotate(true); // 시계 방향
		}
	}
}

void APlayPlayerController::OnPause(const FInputActionInstance& Instance)
{
	if (!PauseMenuClass) return;

	if (!PauseMenuWidget)
	{
		PauseMenuWidget = CreateWidget<UUserWidget>(this, PauseMenuClass);
	}

	if (PauseMenuWidget && !PauseMenuWidget->IsInViewport())
	{
		PauseMenuWidget->AddToViewport();

		bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		SetInputMode(InputMode);
		SetPause(true); // 게임 일시정지
	}
}