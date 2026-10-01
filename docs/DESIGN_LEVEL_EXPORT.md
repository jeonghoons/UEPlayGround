# DESIGN_LEVEL_EXPORT

## 확정 사항
- 에디터 레벨이 곧 게임 레벨이다.
- 지형은 `.obj`, 게임 로직에 필요한 것(플레이어 스폰, 몬스터 스폰 등)은 `.json`으로 export하며 서버가 소비자 역할로 읽는다.
- 현재 단계에서는 스폰 위치에 `BP_Monster`를 배치하고, 사망 시 `BP_Item`을 드랍한다. 서버에서 스폰이 왔다고 가정하고 구현한다 (`ULocalGameBackend`).

## 제안 JSON 스키마 (Phase 7에서 확정)
```json
{
  "mapName": "DevMap",
  "markers": [
    { "id": 0, "type": "PlayerStart",  "position": {"x":0,"y":0,"z":0}, "yaw": 0 },
    { "id": 1, "type": "MonsterSpawn", "position": {"x":0,"y":0,"z":0}, "yaw": 0,
      "monsterId": 1, "spawnRadius": 300.0, "maxCount": 1 }
  ]
}
```
- `monsterId`는 MonsterTable의 ID를 가리킨다 (`DESIGN_DATA.md`).
- 좌표 단위는 UE 단위(cm)를 그대로 쓰고, 서버 쪽 변환이 필요하면 서버 정리 시 결정한다.

## 미정
- Export 실행 방식 (에디터 메뉴 / Commandlet)
- 마커 액터 클래스 설계 (`Source/Test/MMO/` 내 위치, 에디터 전용 코드 분리 여부)
- 지형 `.obj` 범위 (콜리전 메시만 / 전체 지형)
