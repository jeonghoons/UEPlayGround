// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DataTableSettings.generated.h"

class UDataTable;

/**
 *  UDataManager가 로드할 DataTable 애셋 경로 설정.
 *  코드에 경로를 하드코딩하지 않기 위해 Project Settings(Game → Data Tables)에서 지정하며,
 *  값은 Config/DefaultGame.ini에 저장된다. 테이블이 늘어나면 프로퍼티를 추가한다.
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Data Tables"))
class UDataTableSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	/** FMovementRow 기반 DataTable (DT_MovementTable) */
	UPROPERTY(Config, EditAnywhere, Category="Tables", meta=(RequiredAssetDataTags="RowStructure=/Script/Test.MovementRow"))
	TSoftObjectPtr<UDataTable> MovementTable;
};
