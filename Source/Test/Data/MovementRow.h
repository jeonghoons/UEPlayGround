// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MovementRow.generated.h"

/**
 *  MovementTable 한 행. 이동 속도와 카메라 수치를 정의한다.
 *  원본은 Data/MovementTable.csv이며 첫 컬럼 Name이 ID(RowName)다 (DESIGN_DATA.md).
 *  컬럼명은 아래 프로퍼티명과 1:1로 맞춘다.
 */
USTRUCT(BlueprintType)
struct FMovementRow : public FTableRowBase
{
	GENERATED_BODY()

	/** 최대 이동 속도(cm/s). 자동 달리기의 최고 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxSpeedCmSec = 600.f;

	/** 이동 입력 시 가속도(cm/s^2) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AccelCmSec2 = 2048.f;

	/** 입력이 없을 때 감속도(cm/s^2) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BrakeDecelCmSec2 = 2048.f;

	/** 이동 방향으로 회전하는 속도(deg/s) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationRateYawDegSec = 500.f;

	/** 카메라 붐 길이(cm) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CameraArmLengthCm = 400.f;

	/** 카메라 피치 하한(deg, 아래를 보는 쪽이 음수) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CameraMinPitchDeg = -60.f;

	/** 카메라 피치 상한(deg) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CameraMaxPitchDeg = 30.f;

	/** 마우스 좌우 감도 배율 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LookSensitivityYaw = 1.f;

	/** 마우스 상하 감도 배율 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LookSensitivityPitch = 1.f;
};
