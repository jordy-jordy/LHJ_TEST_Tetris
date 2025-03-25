// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Components/StaticMeshComponent.h"

#include "Mode/01_PLAY/PlayGameMode.h"
#include "Mode/01_PLAY/GridManager.h"


ABlock::ABlock()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    CreateMinos();
}

void ABlock::BeginPlay()
{
    Super::BeginPlay();
}

void ABlock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABlock::CreateMinos()
{
    for (int32 i = 0; i < 4; ++i)
    {
        UStaticMeshComponent* Mino = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("Mino%d"), i));
        Mino->SetupAttachment(RootComponent); // ��� Root�� ����
        Minos.Add(Mino);
    }
}

void ABlock::Initialize(EBlockType Type)
{
    BlockType = Type;

    // ��Ϻ� ������ ����
    TArray<FIntPoint> Offsets;
    switch (Type)
    {
    case EBlockType::I: Offsets = { {0,1}, {1,1}, {2,1}, {3,1} }; break;
    case EBlockType::O: Offsets = { {0,0}, {0,1}, {1,0}, {1,1} }; break;
    case EBlockType::T: Offsets = { {1,0}, {0,1}, {1,1}, {2,1} }; break;
    case EBlockType::S: Offsets = { {1,0}, {2,0}, {0,1}, {1,1} }; break;
    case EBlockType::Z: Offsets = { {0,0}, {1,0}, {1,1}, {2,1} }; break;
    case EBlockType::J: Offsets = { {0,0}, {0,1}, {1,1}, {2,1} }; break;
    case EBlockType::L: Offsets = { {2,0}, {0,1}, {1,1}, {2,1} }; break;
    }

    SetShapeOffsets(Offsets);
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

void ABlock::Move(FIntPoint Direction)
{
    if (!CanMove(Direction)) return;

    CurrentGridPos += FVector2D(Direction);
    SetActorLocation(FVector(CurrentGridPos.X * 100.f, CurrentGridPos.Y * 100.f, 0));
}

void ABlock::Rotate(bool bClockwise)
{
    if (!CanRotate(bClockwise)) return;

    FRotator Rotation = bClockwise ? FRotator(0, -90, 0) : FRotator(0, 90, 0);
    AddActorLocalRotation(Rotation); // Root ���� ȸ��
}

bool ABlock::CanMove(FIntPoint Direction) const
{
    APlayGameMode* GM = GetWorld()->GetAuthGameMode<APlayGameMode>();
    if (!GM) return false;

    AGridManager* GridManager = GM->GetGridManager(); // �̰� PlayGameMode�� ������ ��
    if (!GridManager) return false;

    for (const UStaticMeshComponent* Mino : Minos)
    {
        FVector RelLoc = Mino->GetRelativeLocation();
        int32 OffsetX = FMath::RoundToInt(RelLoc.X / 100.f);
        int32 OffsetY = FMath::RoundToInt(RelLoc.Y / 100.f);

        int32 TargetX = CurrentGridPos.X + OffsetX + Direction.X;
        int32 TargetY = CurrentGridPos.Y + OffsetY + Direction.Y;

        if (!GridManager->IsInside(TargetX, TargetY))
        {
            return false; // �ϳ��� �׸��� ���̸� �̵� �Ұ�
        }
    }

    return true;
}

bool ABlock::CanRotate(bool bClockwise) const
{
    APlayGameMode* GM = GetWorld()->GetAuthGameMode<APlayGameMode>();
    if (!GM) return false;

    AGridManager* GridManager = GM->GetGridManager();
    if (!GridManager) return false;

    for (const UStaticMeshComponent* Mino : Minos)
    {
        FVector RelLoc = Mino->GetRelativeLocation();
        int32 X = FMath::RoundToInt(RelLoc.X / 100.f);
        int32 Y = FMath::RoundToInt(RelLoc.Y / 100.f);

        int32 RotatedX, RotatedY;

        if (bClockwise)
        {
            RotatedX = -Y;
            RotatedY = X;
        }
        else
        {
            RotatedX = Y;
            RotatedY = -X;
        }

        int32 WorldX = CurrentGridPos.X + RotatedX;
        int32 WorldY = CurrentGridPos.Y + RotatedY;

        if (!GridManager->IsInside(WorldX, WorldY))
        {
            return false; // ȸ�� ����� �� ���̸� �Ұ�
        }
    }
    return true;
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
