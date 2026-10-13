# DECISIONS

결정 로그입니다. 번복할 때는 기존 항목을 지우지 말고 새 항목을 추가합니다.

| 날짜 | 결정 | 이유 |
|---|---|---|
| 2026-10-01 | 목표는 IOCP 서버용 3D MMORPG 클라이언트이며, 서버 연동 전 단계로 개발 | 클라이언트를 먼저 완성한 뒤 서버를 정리 |
| 2026-10-01 | 기존 서버/클라이언트 프로젝트는 계획 단계에서만 참고하고, 문서·코드는 그 구조에 얽매이지 않음 | 서버도 비효율/비현업 구조를 정리할 예정 |
| 2026-10-01 | 정해지지 않은 사항은 사용자에게 묻거나, 검은사막/붉은사막과 유사한 개발 방식으로 제안 | 사용자 지침 |
| 2026-10-01 | C++ 중심이되 Blueprint 등 에디터 애셋이 필요하면 생성·설정·연결·확인까지 모든 단계를 안내 | 사용자 지정 |
| 2026-10-01 | 맨손 상태는 존재하되 공격/스킬 불가 (WeaponTable 데이터로 표현) | 사용자 지정 |
| 2026-10-01 | 카메라/조작은 검은사막식 (마우스 카메라 회전, 카메라 기준 WASD, 커서 모드는 별도 키 전환) | 사용자 선택 |
| 2026-10-01 | 이동 입력은 WASD뿐이며 달리기 전용 키 없음. 자동 달리기(속도 기반 블렌드 스페이스). 검은사막식 마우스 카메라 회전은 1차부터 구현 | 사용자 지정 |
| 2026-10-01 | 콤보 공격은 하지 않음. 스킬은 숫자키 1~5 + 무기별 스킬 목록 (검은사막식 커맨드 요소는 차용) | 사용자 지정 |
| 2026-10-01 | 공격 판정은 클라이언트가 AnimNotify 시점에 수행하고, 결과(맞은 대상 목록)를 백엔드에 보고 | 서버 연동 시 서버가 재검증하는 구조로 확장 용이 |
| 2026-10-01 | 클래스 접두사: 기본은 접두사 없이 `Source/Test/` 기능 폴더로 구분, UE 이름과 충돌할 때만 `My` 접두사 | 사용자 선택 |
| 2026-10-01 | 캐릭터는 Pirate 모델 단일 사용, 무기는 한손검/활 | 장비/애니메이션을 UE 매니퀸으로 통일 불가 |
| 2026-10-01 | 스탯/장비/스킬/이동/UI는 DataTable(.csv/.xlsx) 기반, 서버와 같은 데이터 공유 | 현업식 데이터 기반 구조 |
| 2026-10-01 | 서버 이벤트는 `IGameBackend` 뒤에서 로컬로 흉내 (Seam) | 나중에 패킷 구현체로 교체 |
| 2026-10-01 | 버전 관리/마이그레이션은 포트폴리오 단계라 생략 | 범위 축소 |
| 2026-10-01 | 레벨은 게임 레벨이며 지형 `.obj` + 로직 `.json`을 서버가 소비 | 서버가 소비자 역할 |
| 2026-10-01 | 신규 코드는 `Source/Test/` 기능 폴더에 작성. Variant_Platforming/SideScrolling은 삭제, Variant_Combat은 AnimNotify 판정/체력바 위젯 참고용으로 Phase 4 종료 시까지 유지 후 삭제 | 검은사막식 MMO에 불필요(점프·횡스크롤, 서버가 맡는 AI). 삭제 대상은 git 초기 커밋에서 복구 가능 |
| 2026-10-01 | 데이터 원본은 저장소 루트 `Data/` (기본값) | 서버/클라이언트 공용 |

| 2026-10-01 | Phase 7(레벨 Export)은 현재 순서 유지 | 사용자 결정 |
| 2026-10-01 | 클래스 이름: `AMyGameMode`, `AMyPlayerController`(UE와 충돌, `My` 접두사), `APlayerCharacter`(충돌 없음, 접두사 없음). Blueprint는 `Content/_Game/Blueprints/` 하위(`Core/`, `Character/` 등)에 `BP_` 접두사로 생성 | 네이밍 규칙 적용 |
| 2026-10-02 | `Test*` 템플릿 클래스(TestCharacter/GameMode/PlayerController) 삭제, `Source/Test/MMO/` 래퍼 폴더 제거하고 `Source/Test/Core` 등으로 평탄화 | 사용자 결정. 템플릿 정리 후 래퍼 폴더의 구분 이점이 사라짐 (`Test.h/.cpp`는 모듈 진입점이라 유지) |
| 2026-10-03 | 데이터 규약: 서버/UE는 `Data/*.csv`를 읽는 기준(허브)으로 사용. 편집은 xlsx 또는 UE DataTable에서 하되 항상 CSV와 일치시킴. 첫 컬럼 `Name`=정수 ID(RowName, 구조체에 Id 없음), 테이블별 독립 ID 1001부터(스킬은 앞자리=무기), 컬럼은 PascalCase로 구조체와 1:1, 애셋 참조는 소프트 경로 | 사용자 결정(동기화 방식) + 제안 승인. 상세는 DESIGN_DATA.md |
| 2026-10-03 | DataTable 경로는 `UDataTableSettings`(UDeveloperSettings, DefaultGame.ini)로 지정하고 `UDataManager`가 로드. 조회 API는 테이블별 `Find<Table>Row(int32 Id)`가 행 포인터/nullptr 반환 | 경로 하드코딩 금지 규칙 준수. 서버 연동 시 로드 방식만 교체 가능 |
| 2026-10-03 | 테이블은 한꺼번에 만들지 않고 해당 기능 작업 시마다 Row 구조체·CSV·DataTable 연결·`Find*Row` 추가. 예: 무기 습득/무기별 애니메이션 지정 시 WeaponTable, HP UI 작업 시 StatTable | 사용자 결정. 쓰이지 않는 컬럼을 미리 설계하는 낭비 방지 |
| 2026-10-03 | Pirate 스켈레톤은 **UE5 매니퀸**(`Demoscene_UE5/Mesh/SKM_Manny_Skeleton`, `Mesh_UE5`)만 사용, UE4 메시는 쓰지 않음. 이동 애니메이션은 `Demoscene_UE5/Animations`(MM_Idle/MM_Walk_Fwd/MM_Run_Fwd)에서 가져옴 | 사용자 결정. 단일 스켈레톤으로 단순화 |
| 2026-10-03 | AnimInstance는 `UMyAnimInstance` 하나만 사용(Pirate 접두 없음). 애님 그래프용 값(Speed 등)은 C++ `NativeUpdateAnimation`에서 계산 | 사용자 결정. 단일 AnimInstance라 캐릭터 이름을 붙일 필요 없음 |
| 2026-10-03 | 정정: Pirate는 무료 에셋 팩 이름일 뿐 캐릭터 이름이 아님. 캐릭터는 UE5 매니퀸 스켈레톤 기반 모듈형 RPG 캐릭터로, 기본은 의상 없는 몸이며 의상/무기를 착용·해제할 수 있어야 함. 10-01의 "Pirate 모델 단일 사용"과 10-03의 "Pirate 스켈레톤 UE5만" 행은 이 항목으로 대체됨 | 사용자 정정. 구현 방식(메시 합성 등)은 Phase 3에서 제안 후 확정 |
| 2026-10-03 | 이동 블렌드 스페이스는 템플릿 `BS_Idle_Walk_Run`(Direction/Speed 2D, 스트레이프 대비)을 재사용. 템플릿 스켈레톤(`SK_Mannequin`)과 `SKM_Manny_Skeleton`은 별개 애셋이라 재타깃/호환 설정 필요. `UMyAnimInstance`가 Speed와 Direction을 C++에서 계산 | 사용자 결정. 이전의 1D `BS_Locomotion` 계획 대체 |
| 2026-10-03 | 캐릭터는 남/여 두 성별이며 외부 에셋을 사용. 장비(모자/상의/하의/신발/장갑)와 무기는 성별 무관 착용. 무기는 한손검/두손검/활/총으로 확장하고 무기별 애니메이션 재생. 10-01의 "무기는 한손검/활"을 대체 | 사용자 지정 |
| 2026-10-03 | 스켈레톤은 `SKM_Manny_Skeleton` 하나로 통일, 남/여 몸·장비 모두 이 스켈레톤을 쓰고 애니메이션은 성별 간 공유. 다른 스켈레톤 에셋은 임포트 시 UE 내장 IK Retargeter로 한 번 변환해 `Content/_Game/`에 저장(런타임 리타겟 없음) | 제안 승인. 같은 스켈레톤이면 애님/블렌드 스페이스를 한 번만 만들면 됨 |
| 2026-10-03 | 모듈형 메시는 Leader Pose 방식(몸이 Leader, 슬롯별 컴포넌트가 Follower, 성별 전환은 Leader 메시 교체). 성별 공용 장비는 EquipmentTable Row가 `MeshMale`/`MeshFemale`를 각각 가리킴. 장비로 가려지는 몸 부위는 UE 안에서 숨김 처리 | 제안 승인. 외부 툴 없이 체형 차이를 해결하는 현실적 방식 |
| 2026-10-03 | Blender 등 외부 DCC로 에셋을 수정하지 않고 UE 툴만 사용. 메시는 에셋 팩 제공본 그대로 쓰며, 구매 시 UE5 매니퀸 기반의 파트 분리형 모듈 팩을 우선 | 사용자 지정 |
| 2026-10-03 | 무기별 애니메이션은 Animation Layer Interface로 `AL_Unarmed/AL_OneHandSword/AL_TwoHandSword/AL_Bow/AL_Gun`을 링크, WeaponTable Row가 레이어·소켓을 가리킴 | 기존 방침 유지, 무기 종류만 확장 |
| 2026-10-04 | 표준 스켈레톤은 Epic `Characters/Mannequins/Meshes/SK_Mannequin`. BS/ABP/Animation Layer는 이 스켈레톤 기준 1세트만 유지하고 모든 외부 에셋(Pirate `SKM_Manny_Skeleton` 포함)은 표준에 맞춰 공유. 10-03의 "`SKM_Manny_Skeleton` 하나로 통일" 행을 대체 | 사용자 선택. 두 스켈레톤은 본 이름·순서가 동일(diff 확인)하며 Pirate는 교체될 수 있는 무료 팩이라 프로젝트 표준을 그 안에 두지 않음 |
| 2026-10-04 | 이동 BS는 템플릿 `BS_Idle_Walk_Run`(Direction/Speed 2D) 재사용 복원(10-03 행 34 유지). 같은 BS를 쓰기 위한 스켈레톤 연결은 Compatible Skeletons를 먼저 시도하고, 안 되면 Pirate 메시의 Skeleton을 `SK_Mannequin`으로 교체. 애니메이션 복제 리타겟은 에셋이 늘수록 복제본이 늘어 지양 | 사용자 결정 복원. 임시로 "Demoscene 애니로 새 BS"를 안내한 것은 철회 |
| 2026-10-13 | 기본 바디는 Leader에 속옷 입은 FullBody(단일 메시, 남/여 각각)를 두고, 상의/하의/신발/장갑 등은 Follower 스켈레탈 메시를 겹쳐 입힘. 장비가 가리는 몸 부위는 머티리얼 슬롯 교체(1순위)로 숨기고, 슬롯이 하나뿐이면 마스크 머티리얼 파라미터 방식을 검토(`HideBoneByName`은 가중치 구멍 때문에 지양). 10-03의 "벗은 몸을 이루는 파트(Body 하나 / Head_Arms+Torso+Legs)" 미확정 항목을 이 결정으로 닫음 | 사용자 제안 승인. FullBody 에셋이 있음을 사용자가 확인 |
| 2026-10-13 | Pirate `SKM_Manny`와 Adventures_woman `SKM_Manny`의 본 이름·순서·주요 본 위치가 동일함을 확인(사용자). Leader Pose는 본 이름으로 매핑하므로 남/여 몸과 장비를 혼용 가능. Adventures_woman 스켈레톤도 Pirate와 같은 절차로 `SK_Mannequin`에 맞춤 | 사용자 확인 |
| 2026-10-13 | 실제 사용할 에셋은 팩 폴더(`Content/Pirate`, `Content/Adventures_woman`)의 메시·머티리얼·텍스처 폴더를 `Content/_Game/Character/` 아래로 **통째로 이동(Move)** 한 뒤 Fix Up Redirectors, 이후 폴더를 `Body/{Male,Female}`(몸+스켈레톤+피직스), `Equipment/{Weapon,Top,Bottom,Outfit,Head,Hands,Feet}`, 팩별 잔여 폴더(`Equipment/Pirate`, `Equipment/Adventures_woman`), 공용 `Materials/`·`Textures/`로 나누고 직접 지은 이름으로 개명. 복제 대비 참조가 유지되고 작업량이 적으며, 원본 팩은 Fab 라이브러리에서 재추가 가능. 개명은 EquipmentTable 소프트 경로 작성 전에 끝냄. 스켈레톤은 메시를 건드리지 않고 `SK_Mannequin`의 Compatible Skeletons에 여성 스켈레톤을 등록(Pirate와 동일) | 사용자 결정(복제 제안을 이동으로 변경) |
