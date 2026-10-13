// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyAnimInstance.h"
#include "GameFramework/Pawn.h"

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 에디터 미리보기처럼 소유 폰이 없을 때는 0으로 둔다
	const APawn* OwnerPawn = TryGetPawnOwner();
	if (!OwnerPawn)
	{
		Speed = 0.f;
		Direction = 0.f;
		return;
	}

	const FVector Velocity = OwnerPawn->GetVelocity();
	Speed = Velocity.Size2D();

	// 거의 멈췄을 때는 방향이 불안정하므로 0으로 둔다
	if (Speed > KINDA_SMALL_NUMBER)
	{
		const float VelocityYaw = Velocity.ToOrientationRotator().Yaw;
		Direction = FMath::FindDeltaAngleDegrees(OwnerPawn->GetActorRotation().Yaw, VelocityYaw);
	}
	else
	{
		Direction = 0.f;
	}
}
