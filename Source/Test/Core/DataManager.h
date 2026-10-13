// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataManager.generated.h"

class UDataTable;
struct FMovementRow;

/**
 *  DataTable 조회 창구. 게임플레이 코드는 DataTable을 직접 열지 않고 이 클래스로만 조회한다.
 *  테이블 경로는 UDataTableSettings(Project Settings)에서 읽는다. ID는 CSV 첫 컬럼(RowName)이다.
 *  반환 포인터는 DataTable이 소유하므로 저장하지 말고 즉시 사용한다. 행이 없으면 nullptr.
 */
UCLASS()
class UDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** MovementTable에서 ID로 행을 찾는다 */
	const FMovementRow* FindMovementRow(int32 Id) const;

private:

	/** 설정된 소프트 경로의 DataTable을 로드한다. 미지정/로드 실패 시 경고 후 nullptr */
	UDataTable* LoadTable(const TSoftObjectPtr<UDataTable>& TablePath, const TCHAR* TableName) const;

	UPROPERTY(Transient)
	TObjectPtr<UDataTable> MovementTable;
};
