// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class LHJ_TETRIS_API UTitleMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Input_Width;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Input_Height;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitGameButton;

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnExitGame();
};
