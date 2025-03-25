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

// 해당 위치의 블록을 가져오는 함수
UStaticMeshComponent* AGridManager::GetCell(int32 _X, int32 _Y) const
{
	if (!IsInside(_X, _Y)) return nullptr;
	return GridMap[_Y * GridWidth + _X];
}

// 해당 위치에 블록을 저장하는 함수
void AGridManager::SetCell(int32 _X, int32 _Y, UStaticMeshComponent* _Mino)
{
	if (!IsInside(_X, _Y)) return;
	GridMap[_Y * GridWidth + _X] = _Mino;
}

void AGridManager::ClearFullLines()
{
	TArray<int32> ClearedRows;

	// 1. 삭제할 줄 탐색
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		bool bFull = true;

		for (int32 X = 0; X < GridWidth; ++X)
		{
			if (GetCell(X, Y) == nullptr)
			{
				bFull = false;
				break;
			}
		}

		if (bFull == true)
		{
			ClearedRows.Add(Y);
		}
	}

	if (ClearedRows.Num() == 0) return;

	// 2. 가득 찬 줄 제거 (아래에서 위로)
	for (int32 Row : ClearedRows)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			UStaticMeshComponent* Mino = GetCell(X, Row);
			if (Mino)
			{
				Mino->UnregisterComponent();
				Mino->DestroyComponent(false);

				// GridMap도 비워주고
				SetCell(X, Row, nullptr);

				// BlockOwner에게서도 제거
				if (AActor* MinoOwner = Mino->GetOwner())
				{
					if (ABlock* BlockOwner = Cast<ABlock>(MinoOwner))
					{
						BlockOwner->RemoveMino(Mino);
					}
				}
			}
		}
		FlushRenderingCommands();
	}

	// 3. 위 줄들 아래로 이동 (위에서 아래로 순회)
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		// 현재 Y보다 아래에 몇 줄이 삭제됐는지 계산
		int32 NumClearedBelow = 0;
		for (int32 ClearedY : ClearedRows)
		{
			if (ClearedY < Y)
			{
				NumClearedBelow++;
			}
		}

		if (NumClearedBelow > 0)
		{
			for (int32 X = 0; X < GridWidth; ++X)
			{
				UStaticMeshComponent* Mino = GetCell(X, Y);
				SetCell(X, Y, nullptr);

				if (Mino)
				{
					SetCell(X, Y - NumClearedBelow, Mino);
					FVector Loc = Mino->GetComponentLocation();
					Mino->SetWorldLocation(Loc - FVector(0, 100.f * NumClearedBelow, 0));
				}
			}
		}
	}
}
