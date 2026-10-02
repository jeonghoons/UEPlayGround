// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

/**
 *  MMO 클라이언트의 게임 모드.
 *  Default Pawn / PlayerController 등 구체 설정은 Blueprint 서브클래스(BP_GameMode)에서 지정한다.
 *  이후 백엔드(IGameBackend) 생성/소유가 이 클래스의 책임이 된다.
 */
UCLASS(abstract)
class AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AMyGameMode();
};
