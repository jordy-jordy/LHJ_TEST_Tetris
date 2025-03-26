// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GlobalConst.h"


int32 UGlobalConst::MapWidth = 10;
int32 UGlobalConst::MapHeight = 10;

void UGlobalConst::SetMapSize(int32 _W, int32 _H)
{
	MapWidth = _W;
	MapHeight = _H;
}
