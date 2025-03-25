// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"


UENUM(BlueprintType)
enum class EBlockType : uint8
{
	I, O, T, S, Z, J, L
};

UCLASS()
class LHJ_TETRIS_API ABlock : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlock();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize(EBlockType Type);

	void Move(FIntPoint Direction);

	void Rotate(bool bClockwise);

	bool CanMove(FIntPoint Direction) const;

	bool CanRotate(bool bClockwise) const;

	int32 GetMaxYOffset() const;

	void SetCurrentGridPos(FVector2D NewPos);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<UStaticMeshComponent*> Minos;

	EBlockType BlockType;

	FVector2D CurrentGridPos; // 그리드 기준 위치
	
	void SetShapeOffsets(const TArray<FIntPoint>& Offsets);

	void CreateMinos();

};
