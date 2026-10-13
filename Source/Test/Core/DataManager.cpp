// Copyright Epic Games, Inc. All Rights Reserved.

#include "DataManager.h"
#include "DataTableSettings.h"
#include "MovementRow.h"
#include "Engine/DataTable.h"
#include "Test.h"

void UDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UDataTableSettings* Settings = GetDefault<UDataTableSettings>();
	MovementTable = LoadTable(Settings->MovementTable, TEXT("MovementTable"));

	// 로드 확인용 로그: 행 수와 각 행의 대표 값
	if (MovementTable)
	{
		UE_LOG(LogTest, Log, TEXT("UDataManager: MovementTable 로드 (행 %d개)"), MovementTable->GetRowMap().Num());
		MovementTable->ForeachRow<FMovementRow>(TEXT("UDataManager"), [](const FName& Key, const FMovementRow& Row)
		{
			UE_LOG(LogTest, Log, TEXT("UDataManager:   Movement[%s] MaxSpeedCmSec=%.1f CameraArmLengthCm=%.1f"),
				*Key.ToString(), Row.MaxSpeedCmSec, Row.CameraArmLengthCm);
		});
	}
}

const FMovementRow* UDataManager::FindMovementRow(int32 Id) const
{
	if (!MovementTable)
	{
		return nullptr;
	}

	// RowName은 ID를 문자열로 쓴 값이다 (DESIGN_DATA.md)
	return MovementTable->FindRow<FMovementRow>(FName(*FString::FromInt(Id)), TEXT("UDataManager::FindMovementRow"));
}

UDataTable* UDataManager::LoadTable(const TSoftObjectPtr<UDataTable>& TablePath, const TCHAR* TableName) const
{
	if (TablePath.IsNull())
	{
		UE_LOG(LogTest, Warning, TEXT("UDataManager: %s 경로가 비어 있음 (Project Settings → Game → Data Tables)"), TableName);
		return nullptr;
	}

	UDataTable* Table = TablePath.LoadSynchronous();
	if (!Table)
	{
		UE_LOG(LogTest, Warning, TEXT("UDataManager: %s 로드 실패: %s"), TableName, *TablePath.ToString());
	}
	return Table;
}
