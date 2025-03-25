// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_PLAY/PlayPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "Mode/01_PLAY/PlayGameMode.h"
#include "Mode/01_PLAY/Block.h"


void APlayPlayerController::BeginPlay()
{
	Super::BeginPlay();

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
			Block->Move(FIntPoint(0, -1)); // 아래로 이동
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
