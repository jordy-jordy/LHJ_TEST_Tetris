// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_PLAY/GridManager.h"
#include "RenderingThread.h"

#include "LHJ_Tetris.h"
#include "Global/GlobalConst.h" 
#include "Mode/01_PLAY/Block.h"


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

	UE_LOG(DEV_LOG, Warning, TEXT("Grid initialized: %d x %d"), GridWidth, GridHeight);

	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			FString Name = FString::Printf(TEXT("GridCell_%d_%d"), X, Y);
			UStaticMeshComponent* Cell = NewObject<UStaticMeshComponent>(this, *Name);

			Cell->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Cell->RegisterComponent();

			if (GridCellMesh)
				Cell->SetStaticMesh(GridCellMesh);

			if (GridCellMaterial)
				Cell->SetMaterial(0, GridCellMaterial);

			FVector Location = FVector(X * 100.f, Y * 100.f, -50.f); // 블록 아래 깔리게
			Cell->SetWorldLocation(Location);

			Cell->SetWorldScale3D(FVector(0.95f)); // 약간 작게 해서 테두리 느낌
			VisualGridCells.Add(Cell);
		}
	}
}

// 맵 안에 있는지 체크
bool AGridManager::IsInside(int32 _X, int32 _Y) const
{
	return _X >= 0 && _X < GridWidth && _Y >= 0 && _Y < GridHeight;
}

void AGridManager::SetCell(int32 X, int32 Y, UStaticMeshComponent* _Mino)
{
	if (!IsInside(X, Y)) return;
	GridMap[Y * GridWidth + X] = _Mino;
}

UStaticMeshComponent* AGridManager::GetCell(int32 X, int32 Y) const
{
	if (!IsInside(X, Y)) return nullptr;
	return GridMap[Y * GridWidth + X];
}

bool AGridManager::IsLineFull(int32 _Y) const
{
	bool bFull = true;

	for (int32 X = 0; X < GridWidth; ++X)
	{
		if (GetCell(X, _Y) == nullptr)
		{
			UE_LOG(DEV_LOG, Warning, TEXT("[IsLineFull] (%d, %d) is empty"), X, _Y);
			bFull = false;
		}
	}
	if (bFull)
	{
		UE_LOG(DEV_LOG, Warning, TEXT("[IsLineFull] Line %d is FULL"), _Y);
	}
	return bFull;
}

void AGridManager::ClearLine(int32 _Y)
{
	for (int32 X = 0; X < GridWidth; ++X)
	{
		UStaticMeshComponent* Mino = GetCell(X, _Y);
		if (Mino)
		{
			AActor* AOwner = Mino->GetOwner();
			ABlock* BlockOwner = Cast<ABlock>(AOwner);
			if (BlockOwner)
			{
				BlockOwner->RemoveMino(Mino);
			}

			Mino->DestroyComponent();
			SetCell(X, _Y, nullptr);
		}
	}
}

void AGridManager::ShiftDownRowsAbove(int32 _ClearedY)
{
	for (int32 Y = _ClearedY + 1; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			UStaticMeshComponent* Upper = GetCell(X, Y);
			if (Upper)
			{
				// 한 줄 아래로 이동
				SetCell(X, Y - 1, Upper);
				SetCell(X, Y, nullptr);

				// 위치 이동
				FVector WorldLoc = Upper->GetComponentLocation();
				WorldLoc.Y -= 100.f; // Y축 아래로
				Upper->SetWorldLocation(WorldLoc);
			}
		}
	}
}
