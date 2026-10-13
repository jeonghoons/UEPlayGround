// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "DataManager.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "MovementRow.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Test.h"

APlayerCharacter::APlayerCharacter()
{
	// 캡슐 크기는 임시값. 캐릭터 메시에 맞춰 BP에서 조정한다.
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 컨트롤러 회전은 카메라에만 영향을 주고 캐릭터는 이동 방향을 바라본다
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 속도/가속/회전 속도/카메라 거리는 BeginPlay에서 MovementTable 값으로 적용한다
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

const FMovementRow* APlayerCharacter::FindMovementRow() const
{
	const UGameInstance* GameInstance = GetGameInstance();
	const UDataManager* DataManager = GameInstance ? GameInstance->GetSubsystem<UDataManager>() : nullptr;
	const FMovementRow* Row = DataManager ? DataManager->FindMovementRow(MovementId) : nullptr;
	if (!Row)
	{
		UE_LOG(LogTest, Warning, TEXT("%s: MovementTable에서 ID %d를 찾지 못함 (BP_PlayerCharacter → Data → Movement Id)"), *GetName(), MovementId);
	}
	return Row;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const FMovementRow* Row = FindMovementRow())
	{
		UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
		MovementComponent->MaxWalkSpeed = Row->MaxSpeedCmSec;
		MovementComponent->MaxAcceleration = Row->AccelCmSec2;
		MovementComponent->BrakingDecelerationWalking = Row->BrakeDecelCmSec2;
		MovementComponent->RotationRate = FRotator(0.f, Row->RotationRateYawDegSec, 0.f);

		CameraBoom->TargetArmLength = Row->CameraArmLengthCm;
		LookSensitivityYaw = Row->LookSensitivityYaw;
		LookSensitivityPitch = Row->LookSensitivityPitch;
	}
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController || !PlayerController->IsLocalController() || !PlayerController->PlayerCameraManager)
	{
		return;
	}

	if (const FMovementRow* Row = FindMovementRow())
	{
		PlayerController->PlayerCameraManager->ViewPitchMin = Row->CameraMinPitchDeg;
		PlayerController->PlayerCameraManager->ViewPitchMax = Row->CameraMaxPitchDeg;
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}
		else
		{
			UE_LOG(LogTest, Warning, TEXT("%s: MoveAction이 지정되지 않음 (BP_PlayerCharacter → Input)"), *GetName());
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}
		else
		{
			UE_LOG(LogTest, Warning, TEXT("%s: LookAction이 지정되지 않음 (BP_PlayerCharacter → Input)"), *GetName());
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	// 컨트롤러 Yaw만 사용해 수평면에서 앞/오른쪽 방향을 구한다
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, Input.Y);
	AddMovementInput(Right, Input.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X * LookSensitivityYaw);
	AddControllerPitchInput(LookAxis.Y * LookSensitivityPitch);
}
