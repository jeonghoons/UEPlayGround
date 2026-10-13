# ROADMAP

진행 상황과 작업 순서를 기록합니다. 설계(목표 아키텍처)는 `DESIGN_*.md`, 확정된 결정은 `DECISIONS.md`에 둡니다.
**한 번에 하나의 체크박스만 진행하고, 완료하면 즉시 `[x]`로 갱신합니다.**

현재 위치: **Phase 2**

## 다음 할 일 (재개 시 여기부터)
1. (완료) 스켈레톤 연결·`ABP_Player`·Anim Class: Compatible Skeletons 등록(A안)으로 해결, `BS_Idle_Walk_Run` Speed 0~600(= `MaxSpeedCmSec` 600)과 일치, PIE Idle/Walk/Run 동작 확인
2. (완료) 수동 작업 2-4: Movement Id=1001 확인, CSV 재임포트로 이동 속도 반영 PIE 확인. 카메라 거리(`CameraArmLengthCm`) 반영은 미검증이며 카메라 작업 시 확인
3. 모듈형 캐릭터 구현(Phase 3): Leader = 속옷 입은 FullBody(남/여), 장비는 Follower 겹침(DECISIONS 10-13 확정). 남은 일: ① 수동 작업 3-1 에셋 이동·폴더 분할·개명(`Content/_Game/Character/`) ② 여성 스켈레톤 `SK_Mannequin` Compatible 등록 ③ 슬롯 범위(Head/Top/Bottom/Hands/Feet/Back) 확정 후 `PlayerCharacter`에 Follower 컴포넌트 추가

## Phase 0 – 정리/기반
- [x] 클래스 접두사 확정 — 충돌 시에만 `My` (DECISIONS.md 기록됨)
- [x] `Source/Test/MMO/` 하위 폴더 구조 생성 및 `Test.Build.cs` include 경로 갱신 — 빌드 성공 확인 (폴더: Core, Backend, Character, Combat, Animation, Data, UI. 빈 폴더는 `.gitkeep`)
- [x] MMO용 GameMode / PlayerController / Character C++ 껍데기 (`AMyGameMode`, `AMyPlayerController`, `APlayerCharacter`) — 빌드 성공 확인
- [x] 위 클래스의 Blueprint 생성 및 DevMap 적용 — 완료 기준: DevMap에서 PIE 진입 시 캐릭터가 PlayerStart에 나타나고 카메라가 뒤에서 비춤
- [x] 템플릿 `Test*` 클래스(TestCharacter/GameMode/PlayerController) 삭제 및 `MMO/` 폴더 평탄화(`Source/Test/Core` 등) — 빌드 성공 확인
- [x] 템플릿 variant 처리 — Platforming/SideScrolling 삭제 완료(빌드 성공), Combat은 Phase 4 종료 시 삭제 (DECISIONS.md 기록)
- [x] Phase 7(Export)은 현재 순서(Phase 7) 유지로 결정 (DECISIONS.md 기록)

## Phase 1 – 데이터 파이프라인
- [x] `Data/` 폴더와 CSV 작성 규약 확정 (`DESIGN_DATA.md` 채우기)
- [x] Row 구조체 방식 확정 및 첫 테이블(Movement) 작성 (`FMovementRow`, `Data/MovementTable.csv`, `DT_MovementTable` 임포트 확인 완료). 나머지 테이블은 해당 기능 작업 시 그 작업에서 Row 구조체·CSV·DataTable 연결 (DECISIONS.md)
- [x] CSV → DataTable 임포트/재임포트 절차 확립 — 완료 기준: CSV 수정 후 재임포트하면 값이 바뀜
- [x] `UDataManager`(GameInstanceSubsystem) ID 조회 — 완료 기준: 로그로 값 확인 (빌드·PIE 로그 확인 완료)

## Phase 2 – 조작·카메라
- [x] 플레이어 캐릭터 + Enhanced Input WASD 이동 (달리기 전용 키 없음) — 빌드·PIE 확인 완료 (기존 `Content/Input` 애셋 재사용, 이동 방향은 컨트롤러 Yaw 기준)
- [x] 카메라 기준 이동 + 검은사막식 마우스 카메라 회전 — 빌드·PIE 확인 완료
- [x] 속도 기반 블렌드 스페이스로 자동 달리기 (Idle/Walk/Run) — `UMyAnimInstance`(Speed, Direction) 작성, 템플릿 `BS_Idle_Walk_Run` 재사용, PIE Idle/Walk/Run 확인 완료
- [x] 이동 수치를 MovementTable에서 로드 (`UDataManager::FindMovementRow` 사용) — PIE에서 이동 속도 반영 확인 완료. 카메라 거리(`CameraArmLengthCm`)·피치 제한·마우스 감도 반영은 미검증이라 카메라 작업 시 확인

## Phase 3 – 무기/애니메이션
- [x] 스켈레톤 방침 확정 — UE5 매니퀸 스켈레톤 하나만 사용 (DECISIONS.md). 모듈형 구조는 아래 항목에서 설계
- [ ] 모듈형 캐릭터 구조 및 기본 바디(의상 없음, 남/여) 적용 — Leader Pose + 슬롯별 컴포넌트, 성별 전환은 Leader 메시 교체 (방침 확정, 구현 대기)
- [ ] Animation Layer Interface 및 `AL_Unarmed` / `AL_OneHandSword` / `AL_Bow` (두손검/총 레이어는 해당 무기 도입 시 추가)
- [ ] 무기 장착/해제 전환 흐름 (소켓 부착 + 레이어 링크)
- [ ] WeaponTable 작성(Row 구조체, CSV, DataTable) 및 무기 습득/애니메이션 레이어 지정에 연동

## Phase 4 – 전투/스킬
- [ ] SkillTable 작성(Row 구조체, CSV, DataTable) 및 `SkillComponent` (쿨타임/시전 상태)
- [ ] 판정 모듈 Sector / Line / Projectile (AnimNotify 구동, 클라이언트 판정 후 백엔드 보고, 디버그 드로잉)
- [ ] 한손검 기본 공격(콤보 없음) + 숫자키 1~5 스킬 (무기별 스킬 목록)
- [ ] 활 기본 공격 + 조준/발사 + 숫자키 스킬
- [ ] 맨손 상태에서 공격/스킬 차단 확인
- [ ] Variant_Combat(`Source/Test/Variant_Combat`, `Content/Variant_Combat`, 관련 `__External*__`, Build.cs include 경로) 삭제

## Phase 5 – Seam + 가짜 서버 이벤트
- [ ] `IGameBackend` 인터페이스 정의 (의도/이벤트 목록)
- [ ] `ULocalGameBackend` 구현
- [ ] MonsterTable 작성 및 스폰 지점에서 `BP_Monster` 스폰, 피격/사망 애니메이션
- [ ] ItemTable 작성 및 사망 시 `BP_Item` 드랍, 줍기·장착 모션

## Phase 6 – UI
- [ ] StatTable·UITable 작성 및 HUD (HP바, 스킬바, 쿨타임)
- [ ] EquipmentTable 작성 및 인벤토리/장비창
- [ ] 몬스터 체력바

## Phase 7 – 레벨 Export
- [ ] PlayerStart / MonsterSpawn 마커 액터
- [ ] 지형 `.obj` + 로직 `.json` Export (에디터 모듈)

## Phase 8 – 서버 연동 준비 점검
- [ ] 백엔드 의도/이벤트 목록을 패킷 후보로 정리
- [ ] 서버 구조 재설계 입력 문서화

## 수동 작업 기록 (에디터에서 사용자가 직접)
상세 절차는 기록하지 않고 대화창으로 안내합니다. 여기에는 항목별 한 줄 요약과 상태만 남깁니다.
- [x] 0-1. `BP_PlayerCharacter` / `BP_PlayerController` / `BP_GameMode` 생성, DevMap의 GameMode Override 지정
- [x] 1-1. `DT_MovementTable` 임포트(`Data/MovementTable.csv`, Row 구조체 `MovementRow`), 재임포트 확인, xlsx 저장
- [x] 1-2. `UDataManager` PIE 로그 확인 (Output Log `LogTest`에서 MovementTable 로드/값 확인)
- [x] 2-1. `BP_PlayerCharacter`의 Move Action = `IA_Move`, `BP_PlayerController`의 Default Mapping Contexts에 `IMC_Default` 추가, PIE에서 WASD 이동 확인
- [x] 2-2. `BP_PlayerCharacter`의 Look Action = `IA_MouseLook`, `BP_PlayerController`의 Default Mapping Contexts에 `IMC_MouseLook` 추가, PIE에서 마우스 카메라 회전 확인
- [x] 2-3. 템플릿 `BS_Idle_Walk_Run`(Direction/Speed 2D, `SK_Mannequin` 표준)을 Pirate 메시(`SKM_Manny_Skeleton`)에서 쓰도록 스켈레톤 연결 + `ABP_Player`(부모 `MyAnimInstance`) 생성, `BP_PlayerCharacter` Mesh에 Anim Class 지정, PIE에서 Idle/Walk/Run 블렌딩 확인
- [x] 2-4. `BP_PlayerCharacter`의 Movement Id = `1001` 지정, PIE에서 `MovementTable` 값 변경이 이동 속도/카메라 거리에 반영되는지 확인
- [ ] 3-1. 팩 폴더(메시·머티리얼·텍스처)를 `Content/_Game/Character/`로 이동 → Fix Up Redirectors → 슬롯별 폴더 분할·개명, 여성 스켈레톤을 `SK_Mannequin` Compatible Skeletons에 등록
