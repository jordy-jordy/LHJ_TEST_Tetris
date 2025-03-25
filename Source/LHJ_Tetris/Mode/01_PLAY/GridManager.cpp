// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_PLAY/GridManager.h"

#include "Global/GlobalConst.h" 


// Sets default values
AGridManager::AGridManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 맵 그리드 생성
void AGridManager::InitializeGrid()
{
    GridWidth = UGlobalConst::MapWidth;
    GridHeight = UGlobalConst::MapHeight;

    const int32 TotalCells = GridWidth * GridHeight;
    GridMap.SetNum(TotalCells);

    for (int32 Index = 0; Index < TotalCells; ++Index)
    {
        GridMap[Index] = nullptr; // 모두 빈 칸으로 초기화
    }

    UE_LOG(LogTemp, Log, TEXT("Grid initialized: %d x %d"), GridWidth, GridHeight);
}

// 맵 안에 있는지 체크
bool AGridManager::IsInside(int32 X, int32 Y) const
{
    return X >= 0 && X < GridWidth && Y >= 0 && Y < GridHeight;
}

// 해당 위치의 블록을 가져오는 함수
AActor* AGridManager::GetCell(int32 X, int32 Y) const
{
    if (!IsInside(X, Y)) return nullptr;
    return GridMap[Y * GridWidth + X];
}

// 해당 위치에 블록을 저장하는 함수
void AGridManager::SetCell(int32 X, int32 Y, AActor* Block)
{
    if (!IsInside(X, Y)) return;
    GridMap[Y * GridWidth + X] = Block;
}