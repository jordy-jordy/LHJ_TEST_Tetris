// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"



UCLASS()
class LHJ_TETRIS_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 맵 그리드 생성
	void InitializeGrid();

	// 맵 안에 있는지 체크
	bool IsInside(int32 X, int32 Y) const;

	// 해당 위치의 블록을 가져오는 함수
	AActor* GetCell(int32 X, int32 Y) const;

	// 해당 위치에 블록을 저장하는 함수
	void SetCell(int32 X, int32 Y, AActor* Block);

private:
	UPROPERTY()
	TArray<AActor*> GridMap;

	int32 GridWidth;	// 가로
	int32 GridHeight;	// 세로
};
