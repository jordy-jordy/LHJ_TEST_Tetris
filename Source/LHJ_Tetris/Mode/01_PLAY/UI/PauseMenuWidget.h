// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class LHJ_TETRIS_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_ToTitle;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_ExitGame;

	UFUNCTION()
	void OnToTitle();

	UFUNCTION()
	void OnExitGame();
};
