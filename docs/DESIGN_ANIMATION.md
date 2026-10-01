# DESIGN_ANIMATION

## 확정 사항
- 캐릭터는 Pirate 모델 단일 사용 (`Content/Pirate/`). 매니퀸 통일은 하지 않는다.
- 무기는 한손검과 활. 무기에 따라 애니메이션 연출이 달라진다.
- 맨손(`Unarmed`) 상태가 존재한다. 이동/대기 등 Locomotion 애니메이션은 있지만 **공격과 스킬은 불가능**하다. 공격 가능 여부는 코드 분기가 아니라 WeaponTable의 데이터(예: 공격 가능 플래그, 사용 가능 스킬 목록이 비어 있음)로 표현한다.

## 목표 구조 (Animation Layer Interface)
- Animation Layer Interface에 `Locomotion`/`Attack` 등 함수 시그니처를 정의한다.
- 무기별 AnimLayer: `AL_Unarmed`, `AL_OneHandSword`, `AL_Bow`.
- 메인 AnimInstance가 무기 전환 시 `LinkAnimClassLayers`로 레이어를 링크한다.
- 무기 → AnimLayer/메시/소켓 매핑은 WeaponTable에 둔다.
- 공격 히트/콤보 윈도우 같은 타이밍은 C++ 타이머가 아니라 AnimNotify가 소유 캐릭터를 콜백한다.

## 장착/해제 흐름
미정 (Phase 3에서 작성): 소켓 부착, 레이어 링크, 장착 모션 순서.

## 미정
- Pirate 스켈레톤: `Mesh_UE4` vs `Mesh_UE5` (Phase 3에서 확정)
- 이동/공격 애니메이션 애셋 출처
