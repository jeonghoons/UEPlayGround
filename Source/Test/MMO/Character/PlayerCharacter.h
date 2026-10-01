// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 *  플레이어가 조종하는 Pirate 캐릭터의 C++ 베이스.
 *  메시/애님 블루프린트 등 구체 설정은 Blueprint 서브클래스(BP_PlayerCharacter)에서 한다.
 *  입력(WASD 이동, 마우스 카메라)과 데이터 연동은 Phase 2 이후에 추가한다.
 */
UCLASS(abstract)
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** 캐릭터 뒤에 카메라를 두는 붐 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** 추적 카메라 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

public:

	APlayerCharacter();

public:

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
