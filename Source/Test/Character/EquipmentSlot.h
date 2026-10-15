// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/EnumRange.h"
#include "EquipmentSlot.generated.h"

/**
 *  몸(Leader) 위에 겹쳐 입히는 장비 칸. 칸마다 Follower 스켈레탈 메시 컴포넌트가 하나씩 생긴다.
 *  Hat/Hair 분리 여부는 테스트 후 확정 예정이라 칸을 추가/삭제해도 이 enum만 고치면 된다.
 *  무기는 소켓 부착 방식이라 여기에 포함하지 않는다.
 */
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	Hat,
	Hair,
	Top,
	Bottom,
	/** 한벌 옷. Top/Bottom과 동시 착용 시 처리는 EquipmentTable 작업에서 정한다 */
	Outfit,
	Hands,
	Feet
};
ENUM_RANGE_BY_FIRST_AND_LAST(EEquipmentSlot, EEquipmentSlot::Hat, EEquipmentSlot::Feet);
