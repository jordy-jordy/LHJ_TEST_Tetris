// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PlayGameMode.generated.h"


/**
 * 
 */
UCLASS()
class LHJ_TETRIS_API APlayGameMode : public AGameMode
{
	GENERATED_BODY()
	

public:
	class ABlock* GetCurrentBlock() const;
	class AGridManager* GetGridManager() const;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class AGridManager* GridManager;

	// GridManager 클래스 지정
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGridManager> GridManagerClass;

	// Block 생성용 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABlock> BlockClass;

	UPROPERTY()
	ABlock* CurrentBlock;

	void SetupCamera();

};
