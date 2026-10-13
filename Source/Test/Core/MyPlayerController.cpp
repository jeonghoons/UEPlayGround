// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 로컬 플레이어 컨트롤러에만 매핑 컨텍스트를 등록한다
	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 검은사막식: 커서를 숨기고 마우스는 항상 카메라를 돌린다 (커서 모드 전환은 Phase 6)
	if (IsLocalPlayerController())
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
}
