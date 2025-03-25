// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GlobalConst.generated.h"

/**
 * 
 */
UCLASS()
class LHJ_TETRIS_API UGlobalConst : public UObject
{
	GENERATED_BODY()
	
public:
	// 가로 크기
	static const int32 MapWidth = 10;

	// 세로 크기
	static const int32 MapHeight = 20;

};
