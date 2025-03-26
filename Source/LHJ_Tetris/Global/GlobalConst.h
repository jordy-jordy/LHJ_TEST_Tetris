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
	static int32 MapWidth;
	static int32 MapHeight;

	static void SetMapSize(int32 _W, int32 _H);

};
