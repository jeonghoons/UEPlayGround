// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
struct FMovementRow;

/**
 *  플레이어가 조종하는 캐릭터의 C++ 베이스.
 *  메시/애님 블루프린트 등 구체 설정은 Blueprint 서브클래스(BP_PlayerCharacter)에서 한다.
 *  WASD 이동 입력을 처리한다. 마우스 카메라와 MovementTable 연동은 이후 단계에서 추가한다.
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

protected:

	/** WASD 이동 입력 액션 (BP_PlayerCharacter에서 지정, 값 타입 Axis2D) */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	/** 마우스 카메라 회전 입력 액션 (BP_PlayerCharacter에서 지정, 값 타입 Axis2D) */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	/** MovementTable에서 읽을 행 ID (BP_PlayerCharacter에서 지정). 0이면 데이터를 적용하지 않는다 */
	UPROPERTY(EditAnywhere, Category="Data")
	int32 MovementId = 0;

	virtual void BeginPlay() override;

	/** 컨트롤러가 붙을 때 카메라 피치 제한을 적용한다 */
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** 이동 입력: 컨트롤러 Yaw 기준으로 앞/오른쪽 방향 이동 */
	void Move(const FInputActionValue& Value);

	/** 마우스 입력: 컨트롤러(=카메라) 회전 */
	void Look(const FInputActionValue& Value);

private:

	/** UDataManager로 MovementId의 행을 찾는다. 없으면 경고 후 nullptr */
	const FMovementRow* FindMovementRow() const;

	/** MovementTable의 마우스 감도 배율 (데이터 적용 전 기본 1) */
	float LookSensitivityYaw = 1.f;
	float LookSensitivityPitch = 1.f;

public:

	APlayerCharacter();

public:

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
