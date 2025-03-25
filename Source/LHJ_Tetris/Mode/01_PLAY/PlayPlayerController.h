// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "PlayPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class LHJ_TETRIS_API APlayPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_MoveLeft;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_MoveRight;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_MoveDown;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_RotateLeft;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_RotateRight;

	// 바인딩 함수
	void OnMoveLeft(const FInputActionInstance& Instance);
	void OnMoveRight(const FInputActionInstance& Instance);
	void OnMoveDown(const FInputActionInstance& Instance);
	void OnRotateLeft(const FInputActionInstance& Instance);
	void OnRotateRight(const FInputActionInstance& Instance);
};
