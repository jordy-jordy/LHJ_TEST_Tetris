// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_PLAY/PlayGameMode.h"

#include "Mode/01_PLAY/GridManager.h"


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
}