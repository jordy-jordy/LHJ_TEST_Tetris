// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Components/StaticMeshComponent.h"

#include "LHJ_Tetris.h"
#include "Global/GlobalConst.h"
#include "Mode/01_PLAY/PlayGameMode.h"
#include "Mode/01_PLAY/GridManager.h"


ABlock::ABlock()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ABlock::BeginPlay()
{
	Super::BeginPlay();

}

void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlock::RemoveMino(UStaticMeshComponent* _Mino)
{
	if (_Mino)
	{
		Minos.Remove(_Mino);
	}
}

void ABlock::CreateMinos()
{
	for (int32 i = 0; i < 4; ++i)
	{
		FString Name = FString::Printf(TEXT("Mino_%d"), i);
		UStaticMeshComponent* Mino = NewObject<UStaticMeshComponent>(this, *Name);

		Mino->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Mino->RegisterComponent();

		SetupMinoVisual(Mino); // 메시, 머티리얼 설정

		Minos.Add(Mino);
	}
}

void ABlock::SetupMinoVisual(UStaticMeshComponent* _Mino)
{
	if (!_Mino) return;

	if (DefaultMinoMesh)
	{
		_Mino->SetStaticMesh(DefaultMinoMesh);
	}

	if (BlockTypeMaterials.Contains(BlockType))
	{
		_Mino->SetMaterial(0, BlockTypeMaterials[BlockType]);
	}
	else if (DefaultMinoMaterial)
	{
		_Mino->SetMaterial(0, DefaultMinoMaterial);
	}

	_Mino->SetWorldScale3D(FVector(0.95f));
}

void ABlock::Initialize(EBlockType Type)
{
	BlockType = Type;

	TArray<FIntPoint> Offsets;

	switch (Type)
	{
	case EBlockType::I: Offsets = { {-2,0}, {-1,0}, {0,0}, {1,0} }; break;
	case EBlockType::O: Offsets = { {0,0}, {0,1}, {1,0}, {1,1} }; break;
	case EBlockType::T: Offsets = { {-1,0}, {0,0}, {1,0}, {0,1} }; break;
	case EBlockType::S: Offsets = { {0,0}, {1,0}, {-1,1}, {0,1} }; break;
	case EBlockType::Z: Offsets = { {-1,0}, {0,0}, {0,1}, {1,1} }; break;
	case EBlockType::J: Offsets = { {-1,0}, {-1,1}, {0,0}, {1,0} }; break;
	case EBlockType::L: Offsets = { {-1,0}, {0,0}, {1,0}, {1,1} }; break;
	}

	CreateMinos(); // 여기서 미노 생성

	SetShapeOffsets(Offsets); // 위치 지정
}

void ABlock::SetShapeOffsets(const TArray<FIntPoint>& Offsets)
{
	for (int32 i = 0; i < Minos.Num(); ++i)
	{
		if (Offsets.IsValidIndex(i))
		{
			Minos[i]->SetRelativeLocation(FVector(Offsets[i].X * 100.f, Offsets[i].Y * 100.f, 0));
		}
	}
}

bool ABlock::CanRotate(bool bClockwise) const
{
	if (BlockType == EBlockType::O)
		return true; // 회전 무시 → 항상 회전 가능하다고 처리

	// 이하 기존 회전 가능 여부 체크
	APlayGameMode* GM = GetWorld()->GetAuthGameMode<APlayGameMode>();
	if (!GM) return false;

	AGridManager* GridManager = GM->GetGridManager();
	if (!GridManager) return false;

	FVector CenterLoc = Minos[1]->GetRelativeLocation();
	int32 CenterX = FMath::RoundToInt(CenterLoc.X / 100.f);
	int32 CenterY = FMath::RoundToInt(CenterLoc.Y / 100.f);

	for (int32 i = 0; i < Minos.Num(); ++i)
	{
		if (i == 1) continue;

		FVector RelLoc = Minos[i]->GetRelativeLocation();
		int32 X = FMath::RoundToInt(RelLoc.X / 100.f);
		int32 Y = FMath::RoundToInt(RelLoc.Y / 100.f);

		int32 LocalX = X - CenterX;
		int32 LocalY = Y - CenterY;

		int32 RotatedX = bClockwise ? -LocalY : LocalY;
		int32 RotatedY = bClockwise ? LocalX : -LocalX;

		int32 GridX = CurrentGridPos.X + CenterX + RotatedX;
		int32 GridY = CurrentGridPos.Y + CenterY + RotatedY;

		if (!GridManager->IsInside(GridX, GridY))
			return false;

		if (GridManager->GetCell(GridX, GridY) != nullptr)
			return false;
	}

	return true;
}

void ABlock::Rotate(bool bClockwise)
{
	if (BlockType == EBlockType::O)
		return;

	// 회전 불가능 → Wall Kick 시도
	if (!CanRotate(bClockwise))
	{if (!TryWallKickRotation(bClockwise))
		{
			return; // 보정 실패 → 회전하지 않음
		}
	}

	// 중심 미노 기준 회전
	FVector CenterLoc = Minos[1]->GetRelativeLocation();
	int32 CenterX = FMath::RoundToInt(CenterLoc.X / 100.f);
	int32 CenterY = FMath::RoundToInt(CenterLoc.Y / 100.f);

	for (int32 i = 0; i < Minos.Num(); ++i)
	{
		if (i == 1) continue;

		FVector RelLoc = Minos[i]->GetRelativeLocation();
		int32 X = FMath::RoundToInt(RelLoc.X / 100.f);
		int32 Y = FMath::RoundToInt(RelLoc.Y / 100.f);

		int32 LocalX = X - CenterX;
		int32 LocalY = Y - CenterY;

		int32 RotatedX = bClockwise ? -LocalY : LocalY;
		int32 RotatedY = bClockwise ? LocalX : -LocalX;

		int32 NewX = CenterX + RotatedX;
		int32 NewY = CenterY + RotatedY;

		Minos[i]->SetRelativeLocation(FVector(NewX * 100.f, NewY * 100.f, 0));
	}
}

bool ABlock::CanMove(FIntPoint Direction) const
{
	APlayGameMode* GM = GetWorld()->GetAuthGameMode<APlayGameMode>();
	if (!GM) return false;

	AGridManager* GridManager = GM->GetGridManager(); // 이거 PlayGameMode에 만들어야 함
	if (!GridManager) return false;

	for (const UStaticMeshComponent* Mino : Minos)
	{
		FVector RelLoc = Mino->GetRelativeLocation();
		int32 OffsetX = FMath::RoundToInt(RelLoc.X / 100.f);
		int32 OffsetY = FMath::RoundToInt(RelLoc.Y / 100.f);

		int32 TargetX = CurrentGridPos.X + OffsetX + Direction.X;
		int32 TargetY = CurrentGridPos.Y + OffsetY + Direction.Y;

		if (!GridManager->IsInside(TargetX, TargetY))
			return false;

		if (GridManager->GetCell(TargetX, TargetY) != nullptr)
			return false;
	}

	return true;
}

void ABlock::Move(FIntPoint Direction)
{
	if (!CanMove(Direction)) return;

	CurrentGridPos += FVector2D(Direction);
	SetActorLocation(FVector(CurrentGridPos.X * 100.f, CurrentGridPos.Y * 100.f, 0));
}

int32 ABlock::GetMaxYOffset() const
{
	int32 MaxY = 0;

	for (const UStaticMeshComponent* Mino : Minos)
	{
		FVector RelLoc = Mino->GetRelativeLocation();
		int32 OffsetY = FMath::RoundToInt(RelLoc.Y / 100.f);
		if (OffsetY > MaxY)
		{
			MaxY = OffsetY;
		}
	}
	return MaxY;
}

void ABlock::SetCurrentGridPos(FVector2D NewPos)
{
	CurrentGridPos = NewPos;
	SetActorLocation(FVector(NewPos.X * 100.f, NewPos.Y * 100.f, 0.f));
}

bool ABlock::TryWallKickRotation(bool bClockwise)
{
	TArray<FIntPoint> KickOffsets = {
		{ -1, 0 }, // 왼쪽으로 한 칸
		{ 1, 0 }   // 오른쪽으로 한 칸
	};

	FVector2D OriginalPos = CurrentGridPos;

	for (const FIntPoint& Offset : KickOffsets)
	{
		CurrentGridPos += FVector2D(Offset);

		if (CanRotate(bClockwise))
		{
			SetActorLocation(FVector(CurrentGridPos.X * 100.f, CurrentGridPos.Y * 100.f, 0));
			return true;
		}

		// 원래 위치로 복구하고 다음 시도
		CurrentGridPos = OriginalPos;
	}

	return false;
}

bool ABlock::TryMoveDown()
{
	FIntPoint Down = FIntPoint(0, -1); // 아래로

	if (CanMove(Down))
	{
		Move(Down);
		return true;
	}
	else
	{
		// 더 이상 아래로 이동 불가능하면 현재 블록을 그리드에 고정
		FixToGrid();

		// 새로운 블록 스폰 (게임 모드에 요청)
		APlayGameMode* GM = GetWorld()->GetAuthGameMode<APlayGameMode>();
		if (GM)
		{
			GM->SpawnNewBlock();
		}

		// Destroy() 호출하지 않고, 고정된 블록은 그대로 남겨둠
		// 필요에 따라 이 블록을 "fixed" 상태로 변경하는 플래그를 설정할 수 있음.
		return false;
	}
}

void ABlock::FixToGrid()
{
	APlayGameMode* GM = GetWorld()->GetAuthGameMode<APlayGameMode>();
	if (!GM) return;

	AGridManager* Grid = GM->GetGridManager();
	if (!Grid) return;

	// 블록을 그리드에 등록
	for (UStaticMeshComponent* Mino : Minos)
	{
		FVector RelLoc = Mino->GetRelativeLocation();
		int32 OffsetX = FMath::RoundToInt(RelLoc.X / 100.f);
		int32 OffsetY = FMath::RoundToInt(RelLoc.Y / 100.f);

		int32 X = CurrentGridPos.X + OffsetX;
		int32 Y = CurrentGridPos.Y + OffsetY;

		Grid->SetCell(X, Y, Mino);
	}

	bool bAnyLineCleared = true;

	while (bAnyLineCleared)
	{
		bAnyLineCleared = false;

		for (int32 Y = 0; Y < UGlobalConst::MapHeight; ++Y)
		{
			if (Grid->IsLineFull(Y))
			{
				Grid->ClearLine(Y);
				Grid->ShiftDownRowsAbove(Y);
				bAnyLineCleared = true;
				break; // 다시 처음부터 검사해야 해서 break
			}
		}
	}
}
