# DESIGN_DATA

## 원칙
- 스탯, 장비/무기 애셋, 스킬(쿨타임/시전시간/판정 방식), 이동 수치, UI 정의는 모두 DataTable로 관리한다. 코드에 수치를 하드코딩하지 않는다.
- **서버와 UE는 저장소 루트 `Data/*.csv`를 읽어서 사용한다.** CSV가 공용 기준(허브)이다.
- 편집은 **xlsx** 또는 **UE DataTable 에디터**에서 할 수 있고, 어느 쪽에서 고쳐도 **항상 CSV와 일치**시킨다 (아래 "동기화 규칙").
- 버전 관리/마이그레이션은 포트폴리오 단계라 생략한다.

## 테이블 목록 (컬럼은 Row 구조체 작성 시 확정)
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

## 파일 위치
| 종류 | 경로 | 역할 |
|---|---|---|
| CSV | `Data/<Table>.csv` | 서버/UE가 읽는 기준. 예: `Data/SkillTable.csv` |
| xlsx | `Data/<Table>.xlsx` | 편집용. CSV와 같은 이름, 같은 폴더 |
| DataTable 애셋 | `Content/_Game/Data/DT_<Table>.uasset` | UE에서 쓰는 형태. CSV 소스는 위 `Data/<Table>.csv` |

## 동기화 규칙
- **CSV가 항상 최신이어야 한다.** xlsx나 DataTable을 고친 직후 CSV를 갱신하고, 나머지 한쪽도 맞춘다. 갱신 없이 작업을 끝내지 않는다.
- 한 테이블은 **한 번에 한 곳에서만** 편집한다. xlsx와 DataTable을 동시에 고치지 않는다 (양방향 병합 수단이 없다).
- xlsx에서 편집한 경우: xlsx → 다른 이름으로 저장 → `CSV UTF-8(쉼표로 분리)`로 `Data/<Table>.csv` 덮어쓰기 → UE에서 DataTable 우클릭 → Reimport.
- UE DataTable에서 편집한 경우: DataTable 에디터에서 수정 → 애셋 우클릭 → Asset Actions → Export as CSV로 `Data/<Table>.csv` 덮어쓰기 → xlsx도 같은 내용으로 갱신 (CSV를 Excel로 열어 xlsx로 저장).
- 커밋 전 확인: `Data/`에서 CSV와 xlsx의 수정 시각이 함께 갱신됐는지, DataTable이 CSV와 같은 값인지 본다.
- Excel 임시 파일(`~$*.xlsx`)은 커밋하지 않는다.

## CSV 규약
- 인코딩 UTF-8, 구분자 쉼표, 첫 행은 헤더.
- **키 컬럼**: 첫 컬럼 `Name`에 정수 ID를 문자열로 쓴다(예: `1001`). UE가 첫 컬럼을 RowName으로 쓰므로 Row 구조체에는 `Id` 필드를 두지 않는다. 조회는 `FName(FString::FromInt(Id))`로 한다.
- **컬럼명**: PascalCase, Row 구조체 프로퍼티명과 1:1로 맞춘다. 단위는 접미사로 표기한다(`Sec`, `Cm`, `Deg`).
- **값 표기**: Enum은 UENUM 항목명 문자열(`Sector`, `Line`, `Projectile`), 숫자 칸은 비우지 않고 `0`, bool은 `TRUE`/`FALSE`.
- **애셋 참조**: 전체 소프트 경로 문자열(예: `/Game/Pirate/...`). C++은 `TSoftObjectPtr`/`TSoftClassPtr`를 쓴다.
- **ID 간 참조**: 다른 테이블을 가리킬 때는 해당 테이블의 정수 ID를 쓴다(예: Skill의 `WeaponId`). 서버와 공유할 수 있는 형태다.

## ID 규칙
- 테이블마다 독립된 ID 공간이며 `1001`부터 시작한다.
- 종류 구분이 필요한 테이블은 앞자리로 나눈다. SkillTable은 앞 1자리가 무기 종류다(한손검 `1xxx`, 활 `2xxx`).

## 조회
- 조회는 `UDataManager`(GameInstanceSubsystem)를 통해서만 한다.

## 미정
- UE가 CSV를 읽는 방식: 현재는 DataTable 애셋으로 임포트해서 쓴다. 패키징 빌드에서 `Data/*.csv`를 런타임에 직접 읽어야 하는지는 서버 연동 단계에 가까워질 때 결정한다.

## MovementTable 컬럼
Row 구조체: `FMovementRow` (`Source/Test/Data/MovementRow.h`). 원본: `Data/MovementTable.csv`. ID `1001` = 플레이어 기본값.

| 컬럼 | 기본값 | 설명 |
|---|---|---|
| `MaxSpeedCmSec` | 600 | 자동 달리기 최고 속도(cm/s) |
| `AccelCmSec2` | 2048 | 이동 입력 시 가속도 |
| `BrakeDecelCmSec2` | 2048 | 입력 없을 때 감속도 |
| `RotationRateYawDegSec` | 500 | 이동 방향으로 도는 속도(deg/s) |
| `CameraArmLengthCm` | 400 | 카메라 붐 길이 |
| `CameraMinPitchDeg` / `CameraMaxPitchDeg` | -60 / 30 | 카메라 피치 제한 |
| `LookSensitivityYaw` / `LookSensitivityPitch` | 1 / 1 | 마우스 감도 배율 |

줌 범위, 점프/회피 수치는 해당 기능이 확정되면 컬럼을 추가한다.
