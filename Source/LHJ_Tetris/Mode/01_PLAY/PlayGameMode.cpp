// Fill out your copyright notice in the Description page of Project Settings.

#include "Mode/01_PLAY/PlayGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

#include "Global/GlobalConst.h"
#include "Mode/01_PLAY/GridManager.h"
#include "Mode/01_PLAY/Block.h"


void APlayGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void APlayGameMode::BeginPlay()
{
    Super::BeginPlay();

    // 맵 그리드 생성 및 초기화
    if (GridManagerClass)
    {
        GridManager = GetWorld()->SpawnActor<AGridManager>(GridManagerClass);
        GridManager->InitializeGrid();
    }

    if (BlockClass)
    {
        EBlockType RandomType = static_cast<EBlockType>(FMath::RandRange(0, static_cast<int32>(EBlockType::MAX) - 1));

        ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(BlockClass);
        if (NewBlock)
        {
            NewBlock->Initialize(RandomType);

            int32 SpawnX = UGlobalConst::MapWidth / 2;
            int32 MaxYOffset = NewBlock->GetMaxYOffset();
            int32 SpawnY = UGlobalConst::MapHeight - 1 - MaxYOffset;

            NewBlock->SetCurrentGridPos(FVector2D(SpawnX, SpawnY));
            CurrentBlock = NewBlock;
        }
    }

    // 카메라 세팅
    SetupCamera();
}

ABlock* APlayGameMode::GetCurrentBlock() const
{ 
    return CurrentBlock; 
}

AGridManager* APlayGameMode::GetGridManager() const
{
    return GridManager;
}

void APlayGameMode::SetupCamera()
{
    // 맵 사이즈 기준 계산
    const int32 MapWidth = UGlobalConst::MapWidth;
    const int32 MapHeight = UGlobalConst::MapHeight;

    const float CellSize = 100.f;

    // 중앙 위치 계산
    FVector Center = FVector(MapWidth * 0.5f * CellSize, MapHeight * 0.48f * CellSize, 0.f);

    // 위에서 내려다보게 Z축 높이 설정
    float ZHeight = FMath::Max(MapWidth, MapHeight) * CellSize * 1.2f;

    // 카메라 스폰 위치
    FVector CameraLocation = FVector(Center.X, Center.Y, ZHeight);
    FRotator CameraRotation = FRotator(-90.f, 90.f, 0.f); // 아래로 바라보는 회전

    // 카메라 액터 스폰
    ACameraActor* Camera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation);
    Camera->GetCameraComponent()->ProjectionMode = ECameraProjectionMode::Orthographic;
    Camera->GetCameraComponent()->OrthoWidth = (UGlobalConst::MapWidth * 4.0f) * CellSize;

    // 현재 ViewTarget을 이 카메라로 설정
    if (Camera)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (PC)
        {
            PC->SetViewTarget((AActor*)Camera);
        }
    }
}

void APlayGameMode::SpawnNewBlock()
{
    if (!BlockClass) return;

    EBlockType RandomType = static_cast<EBlockType>(FMath::RandRange(0, static_cast<int32>(EBlockType::MAX) - 1));

    ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(BlockClass);
    if (NewBlock)
    {
        NewBlock->Initialize(RandomType);

        int32 SpawnX = UGlobalConst::MapWidth / 2;
        int32 MaxYOffset = NewBlock->GetMaxYOffset();
        int32 SpawnY = UGlobalConst::MapHeight - 1 - MaxYOffset;

        NewBlock->SetCurrentGridPos(FVector2D(SpawnX, SpawnY));
        CurrentBlock = NewBlock;
    }
}
