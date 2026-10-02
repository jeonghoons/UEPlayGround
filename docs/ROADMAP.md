# ROADMAP

진행 상황과 작업 순서를 기록합니다. 설계(목표 아키텍처)는 `DESIGN_*.md`, 확정된 결정은 `DECISIONS.md`에 둡니다.
**한 번에 하나의 체크박스만 진행하고, 완료하면 즉시 `[x]`로 갱신합니다.**

현재 위치: **Phase 1**

## Phase 0 – 정리/기반
- [x] 클래스 접두사 확정 — 충돌 시에만 `My` (DECISIONS.md 기록됨)
- [x] `Source/Test/MMO/` 하위 폴더 구조 생성 및 `Test.Build.cs` include 경로 갱신 — 빌드 성공 확인 (폴더: Core, Backend, Character, Combat, Animation, Data, UI. 빈 폴더는 `.gitkeep`)
- [x] MMO용 GameMode / PlayerController / Character C++ 껍데기 (`AMyGameMode`, `AMyPlayerController`, `APlayerCharacter`) — 빌드 성공 확인
- [x] 위 클래스의 Blueprint 생성 및 DevMap 적용 — 완료 기준: DevMap에서 PIE 진입 시 Pirate 캐릭터가 PlayerStart에 나타나고 카메라가 뒤에서 비춤 (아래 "수동 작업 0-1")
- [x] 템플릿 `Test*` 클래스(TestCharacter/GameMode/PlayerController) 삭제 및 `MMO/` 폴더 평탄화(`Source/Test/Core` 등) — 빌드 성공 확인
- [x] 템플릿 variant 처리 — Platforming/SideScrolling 삭제 완료(빌드 성공), Combat은 Phase 4 종료 시 삭제 (DECISIONS.md 기록)
- [x] Phase 7(Export)은 현재 순서(Phase 7) 유지로 결정 (DECISIONS.md 기록)

## Phase 1 – 데이터 파이프라인
- [x] `Data/` 폴더와 CSV 작성 규약 확정 (`DESIGN_DATA.md` 채우기)
- [ ] Row 구조체 작성: Stat, Weapon, Equipment, Skill, Monster, Item, Movement, UI — 진행: Movement 작성 완료(`FMovementRow`, `Data/MovementTable.csv`; 빌드·DataTable 임포트 확인 대기, "수동 작업 1-1"), 나머지 대기
- [ ] CSV → DataTable 임포트/재임포트 절차 확립 — 완료 기준: CSV 수정 후 재임포트하면 값이 바뀜
- [ ] `UDataManager`(GameInstanceSubsystem) ID 조회 — 완료 기준: 로그로 값 확인

## Phase 2 – 조작·카메라
- [ ] Pirate 캐릭터 + Enhanced Input WASD 이동 (달리기 전용 키 없음)
- [ ] 카메라 기준 이동 + 검은사막식 마우스 카메라 회전
- [ ] 속도 기반 블렌드 스페이스로 자동 달리기 (Idle/Walk/Run)
- [ ] 이동 수치를 MovementTable에서 로드

## Phase 3 – 무기/애니메이션
- [ ] Pirate 스켈레톤 확정 (UE4 / UE5 메시)
- [ ] Animation Layer Interface 및 `AL_Unarmed` / `AL_OneHandSword` / `AL_Bow`
- [ ] 무기 장착/해제 전환 흐름 (소켓 부착 + 레이어 링크)
- [ ] WeaponTable 연동

## Phase 4 – 전투/스킬
- [ ] SkillTable 및 `SkillComponent` (쿨타임/시전 상태)
- [ ] 판정 모듈 Sector / Line / Projectile (AnimNotify 구동, 클라이언트 판정 후 백엔드 보고, 디버그 드로잉)
- [ ] 한손검 기본 공격(콤보 없음) + 숫자키 1~5 스킬 (무기별 스킬 목록)
- [ ] 활 기본 공격 + 조준/발사 + 숫자키 스킬
- [ ] 맨손 상태에서 공격/스킬 차단 확인
- [ ] Variant_Combat(`Source/Test/Variant_Combat`, `Content/Variant_Combat`, 관련 `__External*__`, Build.cs include 경로) 삭제

## Phase 5 – Seam + 가짜 서버 이벤트
- [ ] `IGameBackend` 인터페이스 정의 (의도/이벤트 목록)
- [ ] `ULocalGameBackend` 구현
- [ ] 스폰 지점에서 `BP_Monster` 스폰, 피격/사망 애니메이션
- [ ] 사망 시 `BP_Item` 드랍, 줍기·장착 모션

## Phase 6 – UI
- [ ] HUD (HP바, 스킬바, 쿨타임)
- [ ] 인벤토리/장비창
- [ ] 몬스터 체력바

## Phase 7 – 레벨 Export
- [ ] PlayerStart / MonsterSpawn 마커 액터
- [ ] 지형 `.obj` + 로직 `.json` Export (에디터 모듈)

## Phase 8 – 서버 연동 준비 점검
- [ ] 백엔드 의도/이벤트 목록을 패킷 후보로 정리
- [ ] 서버 구조 재설계 입력 문서화

## 수동 작업 (에디터에서 사용자가 직접)
Claude가 만들 수 없는 `Content/` 애셋(BP, AnimBP, 머티리얼 등) 작업은 여기에 모읍니다.
각 항목에는 부모 클래스, 저장 경로, 에디터 조작 순서, 설정할 프로퍼티와 값, 연결 방법, PIE 확인 방법을 단계별로 적습니다 (`CLAUDE.md` 작업 절차 규칙 5).

### 0-1. GameMode / PlayerController / PlayerCharacter Blueprint 만들고 DevMap에 적용
사전 조건: 에디터를 닫은 상태에서 C++ 빌드가 끝난 뒤(또는 에디터 안에서 Ctrl+Alt+F11) `Test.uproject`를 연다.
Blueprint는 `Content/_Game/Blueprints/` 아래에 모은다. 폴더가 없으면 Content Browser에서 우클릭 → New Folder.

1. **BP_PlayerCharacter**
   - 저장 위치: `Content/_Game/Blueprints/Character/`
   - 생성: 폴더에서 우클릭 → Blueprint Class → 상단 "All Classes"를 펼쳐 검색창에 `PlayerCharacter` 입력 → **Player Character** 선택 → 이름 `BP_PlayerCharacter`
   - 열고 Components 패널에서 **Mesh(Inherited)** 선택 → Details의 Skeletal Mesh Asset을 `Content/Pirate/Mesh_UE5/Full/SKM_Pirate_Full_01`로 지정 (Phase 3에서 스켈레톤 최종 확정 전 임시값)
   - 같은 Mesh의 Transform: Location Z = `-96`, Rotation Z(Yaw) = `-90`. PIE에서 캐릭터가 옆을 보거나 땅에 박히면 이 값을 조정한다.
   - 필요하면 Capsule Component의 Capsule Half Height/Radius(기본 96/42)를 모델 크기에 맞춘다.
   - Compile → Save
2. **BP_PlayerController**
   - 저장 위치: `Content/_Game/Blueprints/Core/`
   - 생성: Blueprint Class → All Classes → `MyPlayerController` 검색 → **My Player Controller** 선택 → 이름 `BP_PlayerController`
   - Class Defaults의 Input → Default Mapping Contexts는 **지금은 비워둔다** (WASD/마우스 입력은 Phase 2에서 지정)
   - Compile → Save
3. **BP_GameMode**
   - 저장 위치: `Content/_Game/Blueprints/Core/`
   - 생성: Blueprint Class → All Classes → `MyGameMode` 검색 → **My Game Mode** 선택 → 이름 `BP_GameMode`
   - 열고 Class Defaults → Classes: **Default Pawn Class** = `BP_PlayerCharacter`, **Player Controller Class** = `BP_PlayerController`
   - Compile → Save
4. **DevMap에 적용**
   - `Content/_Game/Maps/DevMap`을 연다.
   - 상단 메뉴 Window → World Settings → Game Mode → **GameMode Override** = `BP_GameMode`
   - 레벨에 **Player Start**가 없으면 Place Actors 패널(Window → Place Actors)에서 `Player Start`를 검색해 바닥 위에 배치한다.
   - Ctrl+S로 맵 저장
5. **확인 (PIE)**: Alt+P 또는 Play 버튼.
   - 기대 결과: Pirate가 Player Start 위치에 서 있고 카메라가 뒤에서 비춘다.
   - 아직 입력을 연결하지 않았으므로 **WASD나 마우스로 움직이지 않는 것이 정상**이다 (Phase 2).
   - 스킨/애니메이션이 없어 T자 포즈로 보일 수 있다 (Phase 3에서 해결).

C++가 Blueprint에 노출한 항목: `APlayerCharacter`의 `CameraBoom`/`FollowCamera`(Components, ReadOnly), `AMyPlayerController::DefaultMappingContexts`(EditAnywhere). GameMode는 노출 항목 없음(BP Class Defaults의 기본 항목만 사용).

### 1-1. MovementTable DataTable 만들기 (FMovementRow 빌드 후)
사전 조건: 에디터를 닫고 빌드한 뒤 `Test.uproject`를 연다. 새 헤더(`MovementRow.h`)가 추가됐으므로 필요하면 "Generate Visual Studio project files"를 먼저 한다.

1. Content Browser에서 `Content/_Game/Data/` 폴더를 만든다(없으면 우클릭 → New Folder).
2. 탐색기에서 `Data/MovementTable.csv`를 그 폴더로 드래그하거나, 폴더에서 우클릭 → Import to ... 로 CSV를 선택한다.
3. 임포트 창에서 **Choose DataTable Row Type** = `MovementRow`를 선택하고 Apply.
4. 생성된 애셋 이름을 `DT_MovementTable`로 바꾸고 저장한다.
5. 확인: 열었을 때 Row `1001`이 보이고 값이 CSV와 같아야 한다 (MaxSpeedCmSec 600 등).
6. 재임포트 확인: CSV에서 `MaxSpeedCmSec`를 바꾼 뒤 애셋 우클릭 → Reimport → 값이 바뀌어야 한다. 확인 후 원래 값으로 되돌린다.
7. xlsx: `Data/MovementTable.csv`를 Excel로 열어 `Data/MovementTable.xlsx`로 저장한다 (CSV와 같은 내용, 동기화 규칙은 `DESIGN_DATA.md`).

C++가 Blueprint/에디터에 노출한 항목: `FMovementRow`의 모든 프로퍼티(`EditAnywhere`, `BlueprintReadOnly`). 아직 이 테이블을 읽는 코드는 없다(`UDataManager`는 Phase 1의 다음 항목, 이동 적용은 Phase 2).
