// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;

/**
 *  MMO 클라이언트의 플레이어 컨트롤러.
 *  로컬 플레이어에 Enhanced Input 매핑 컨텍스트를 등록한다.
 */
UCLASS(abstract)
class AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	/** 항상 등록할 입력 매핑 컨텍스트 (BP_PlayerController에서 지정) */
	UPROPERTY(EditAnywhere, Category="Input")
	TArray<TObjectPtr<UInputMappingContext>> DefaultMappingContexts;

	virtual void SetupInputComponent() override;
};
