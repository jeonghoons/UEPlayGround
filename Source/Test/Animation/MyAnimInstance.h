// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 *  플레이어 캐릭터의 메인 AnimInstance. 이 프로젝트는 AnimInstance 하나만 쓴다.
 *  애님 그래프가 읽을 값(Speed 등)을 C++에서 계산하며, 구체 애님 블루프린트(ABP_Player)가 이 클래스를 상속한다.
 *  무기별 Animation Layer 링크는 Phase 3에서 이 클래스에 추가한다.
 */
UCLASS(abstract)
class UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	/** 수평면 이동 속도(cm/s). 블렌드 스페이스의 Speed 축에 연결한다 */
	UPROPERTY(BlueprintReadOnly, Category="Locomotion")
	float Speed = 0.f;

	/** 캐릭터가 바라보는 방향 대비 이동 방향의 각도(-180~180도, 정면 0, 오른쪽 +). 블렌드 스페이스의 Direction 축에 연결한다.
	 *  지금은 이동 방향을 바라보므로 항상 0 근처이며, 스트레이프(활 조준 등)에서 의미가 생긴다 */
	UPROPERTY(BlueprintReadOnly, Category="Locomotion")
	float Direction = 0.f;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
