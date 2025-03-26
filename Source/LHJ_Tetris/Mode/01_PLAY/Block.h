// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"


UENUM(BlueprintType)
enum class EBlockType : uint8
{
	I, O, T, S, Z, J, L, MAX
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
	
	// 초기화
	void Initialize(EBlockType Type);

	// 이동
	bool CanMove(FIntPoint Direction) const;
	void Move(FIntPoint Direction);

	// 회전
	bool CanRotate(bool bClockwise) const;
	void Rotate(bool bClockwise);

	// 블록의 최대 길이 얻기
	int32 GetMaxYOffset() const;

	void SetCurrentGridPos(FVector2D NewPos);

	bool TryWallKickRotation(bool bClockwise);

	bool TryMoveDown();

	void FixToGrid();

	void RemoveMino(UStaticMeshComponent* _Mino);

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

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* DefaultMinoMesh;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* DefaultMinoMaterial;

	UPROPERTY(EditDefaultsOnly)
	TMap<EBlockType, UMaterialInterface*> BlockTypeMaterials;

	void SetupMinoVisual(UStaticMeshComponent* _Mino);

};
