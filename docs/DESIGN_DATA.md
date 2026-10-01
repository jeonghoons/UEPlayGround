# DESIGN_DATA

## 원칙
- 스탯, 장비/무기 애셋, 스킬(쿨타임/시전시간/판정 방식), 이동 수치, UI 정의는 모두 DataTable로 관리한다. 코드에 수치를 하드코딩하지 않는다.
- 원본은 저장소 루트 `Data/*.csv`(또는 `.xlsx`)이며 서버도 같은 파일을 본다. UE는 여기서 DataTable로 임포트한다.
- 버전 관리/마이그레이션은 포트폴리오 단계라 생략한다.

## 테이블 목록 (컬럼은 Phase 1에서 확정)
| 테이블 | 용도 |
|---|---|
| StatTable | 캐릭터/몬스터 기본 스탯 |
| WeaponTable | 무기 타입(한손검/활), 메시, 소켓, AnimLayer |
| EquipmentTable | 장비 파츠, 스탯 보정 |
| SkillTable | 쿨타임, 시전시간, 판정 Shape(Sector/Line/Projectile), 범위, 데미지 계수 |
| MonsterTable | 몬스터 BP, 스탯, 애니메이션 |
| ItemTable | 아이템 BP, 종류 |
| MovementTable | 이동 속도, 카메라 관련 수치 |
| UITable | UI 정의 |

## 규칙 (초안)
- 모든 Row는 정수 ID를 가진다 (ID 규칙은 Phase 1에서 확정).
- 애셋 참조는 `TSoftObjectPtr`/`TSoftClassPtr` 경로를 사용한다.
- 조회는 `UDataManager`(GameInstanceSubsystem)를 통해서만 한다.

## 미정
- ID 번호 규칙, CSV 컬럼 명명 규칙, 서버가 읽는 경로
