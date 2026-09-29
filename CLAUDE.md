# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 프로젝트 개요

이 프로젝트는 Epic의 기본 Third Person 템플릿을 기반으로 한 Unreal Engine 5.6 C++ 프로젝트("Test", 프로젝트 표시 이름은 "Third Person Game Template")입니다. 기본 템플릿에 더해 Combat, Platforming, SideScrolling이라는 세 가지 독립적인 게임플레이 변형(variant)을 함께 제공하며, 각 변형은 동일한 캐릭터/입력 기반 위에서 서로 다른 장르를 보여줍니다. 소스 코드 외에 별도의 게임 기획 문서는 없으며 — 이 세 변형 자체가 참고용 구현체입니다.

현재 git 저장소는 아닙니다.

## 빌드 명령어

이 프로젝트는 표준 UE5 C++ 프로젝트이며 CMake/Make/npm 같은 빌드 시스템은 사용하지 않습니다. 빌드는 Unreal Build Tool(UBT) 또는 Visual Studio 솔루션을 통해 이루어집니다.

- **프로젝트 파일 생성** (소스 파일 추가/삭제 후): `Test.uproject`를 우클릭 → "Generate Visual Studio project files", 또는 `UnrealBuildTool.exe -projectfiles -project="Test.uproject" -game -rocket -progress` 실행.
- **Visual Studio로 빌드**: `Test.sln`을 열고 `Test` 타겟에 대해 `Development Editor`(또는 `Debug Editor`) 구성으로 빌드.
- **명령줄로 빌드** (엔진의 `Engine/Build/BatchFiles` 디렉터리에서): `Build.bat TestEditor Win64 Development -project="<path>\Test.uproject"`.
- **에디터 실행**: `Test.uproject`를 직접 열거나(더블클릭, 또는 `UnrealEditor.exe <path>\Test.uproject`), Visual Studio에서 `TestEditor` 타겟을 실행/디버그.
- 빌드 타겟은 두 개이며 `Source/Test.Target.cs`(게임, `TestTarget`)와 `Source/TestEditor.Target.cs`(에디터, `TestEditorTarget`)에 정의되어 있습니다. 두 타겟 모두 단일 `Test` 모듈을 빌드합니다.
- 이 저장소에는 유닛 테스트 스위트가 없습니다(Automation Spec 등 테스트 소스 파일 없음). 게임플레이는 자동화된 테스트 명령이 아니라 에디터/PIE 실행을 통해 검증합니다.

## 모듈 / 의존성 구조

- 단일 주 모듈 `Test`(`Source/Test/Test.Build.cs`), `Type: Runtime`, `Default` 단계에 로드됨.
- Public 의존성: `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`, `AIModule`, `StateTreeModule`, `GameplayStateTreeModule`, `UMG`, `Slate`.
- `PublicIncludePaths`에 모든 variant 하위 폴더(`Test/Variant_Combat`, `Test/Variant_Combat/AI` 등)가 명시적으로 나열되어 있어, 변형(variant) 간 헤더를 상대 경로 없이 파일명만으로 `#include`할 수 있습니다 — 새 variant 하위 폴더를 추가할 때는 이 목록도 함께 갱신해야 합니다.
- `Test.uproject`에서 활성화된 플러그인: `ModelingToolsEditorMode`(에디터 전용), `StateTree`, `GameplayStateTree`.
- 입력 시스템은 레거시 입력이 아닌 Enhanced Input(`UInputAction` / `FInputActionValue`)을 사용합니다 — 모든 캐릭터 클래스는 `SetupPlayerInputComponent`에서 액션을 바인딩합니다.

## 코드 아키텍처

### 기본 템플릿 레이어 (`Source/Test/*.h/.cpp`, `Variant_` 접두사 없음)
`ATestGameMode`, `ATestCharacter`, `ATestPlayerController` — 기본 Third Person 템플릿 클래스들입니다. `ATestCharacter`는 카메라 붐/추적 카메라를 소유하고, `DoMove`/`DoLook`/`DoJumpStart`/`DoJumpEnd`를 `BlueprintCallable` 래퍼로 노출하여 Enhanced Input 핸들러를 감쌉니다. 이를 통해 UI(예: 터치 컨트롤)와 실제 입력이 동일한 진입점을 통과하도록 합니다. 이 프로젝트의 모든 게임플레이 클래스는 "물리적 입력 → private 핸들러 → public `Do*` BlueprintCallable"이라는 동일한 패턴을 따르므로, 새로운 Blueprint 노출 입력 경로를 추가하기보다는 이 패턴을 그대로 재사용해야 합니다.

각 variant는 자체 GameMode/PlayerController/Character를 가진 독립적인 수직 슬라이스이며, 다른 variant에 의존하지 **않습니다**. 디렉터리(`Variant_Combat`, `Variant_Platforming`, `Variant_SideScrolling`)와 클래스 접두사(`Combat*`, `Platforming*`, `SideScrolling*`)로 네임스페이스가 구분됩니다. 대응하는 레벨은 `Content/<ThirdPerson|Variant_*>/...Lvl_*`에 있으며, `Config/DefaultEngine.ini`의 `GameDefaultMap`/`EditorStartupMap`은 현재 기본 `Lvl_ThirdPerson` 맵을 가리키고 있습니다.

### Variant_Combat — 근접 전투 + StateTree AI
- `ACombatCharacter`(플레이어)와 `ACombatEnemy`(AI) 모두 `ICombatAttacker`, `ICombatDamageable` 인터페이스(`Variant_Combat/Interfaces/`)를 구현하여, 콤보/차지 공격 실행과 데미지/사망/치유 처리가 플레이어 조작인지 AI 조작인지와 무관하게 분리되어 있습니다.
- AI 행동은 Behavior Tree가 아니라 **StateTree**로 구동됩니다: `CombatAIController`가 StateTree 컴포넌트를 구동하고, `Variant_Combat/AI/CombatStateTreeUtility.h/.cpp`에 커스텀 StateTree 조건/태스크(`FStateTreeCharacterGroundedCondition`, `FStateTreeComboAttackTask`, `FStateTreeChargedAttackTask`, `FStateTreeFaceActorTask`, `FStateTreeGetPlayerInfoTask` 등)가 정의되어 있습니다. 이들은 `USTRUCT(meta=(DisplayName=...))`와 인스턴스 데이터 구조체를 통해 StateTree 에디터에 노출됩니다 — 새로운 AI 행동을 추가할 때는 컨트롤러에 직접 로직을 넣지 말고, 기존의 `EnterState`/`Tick`/`ExitState` 패턴을 따르는 새 조건/태스크 구조체를 여기에 추가하세요.
- `EnvQueryContext_Player`는 AI 쿼리를 위한 EQS 컨텍스트(플레이어 위치)를 제공하고, `CombatEnemySpawner`는 웨이브/스폰 로직을 담당합니다.
- 공격 충돌/애니메이션 타이밍은 AnimNotify(`Variant_Combat/Animation/AnimNotify_DoAttackTrace`, `AnimNotify_CheckCombo`, `AnimNotify_CheckChargedAttack`)가 소유 캐릭터의 `ICombatAttacker` 인터페이스를 콜백 호출하는 방식으로 구동됩니다 — 전투 히트 타이밍은 C++ 타이머가 아니라 애니메이션 애셋에 있습니다.
- 게임플레이 볼륨(`CombatActivationVolume`, `CombatCheckpointVolume`)과 위험 요소/파괴 가능 오브젝트(`CombatLavaFloor`, `CombatDamageableBox`, `CombatDummy`)는 구체적인 캐릭터 클래스를 알지 못한 채 위 인터페이스들과 상호작용하는 일반 Actor입니다.
- UI: `CombatLifeBar`(적/플레이어 위에 부착된 UMG 위젯 컴포넌트)는 `ICombatDamageable`의 현재/최대 HP로 구동됩니다.

### Variant_Platforming — 고급 이동, AI 없음
- `APlatformingCharacter`는 기본 이동 세트에 프레스&홀드 점프, 더블 점프, 월 점프, 대시를 추가하며, 이는 모두 movement mode 서브클래싱이 아니라 `CharacterMovementComponent`에 대한 수동 물리 임펄스와 타이머(`WallJumpTimer`)로 구현되어 있습니다. 상태는 패킹된 `uint8` 비트필드(`bHasWallJumped`, `bHasDoubleJumped`, `bHasDashed`, `bIsDashing`)로 추적됩니다.
- 대시 종료는 `AnimNotify_EndDash`가 캐릭터를 콜백 호출하여 트리거됩니다 — Combat variant의 공격 notify와 동일한 "애니메이션이 게임플레이 타이밍을 주도한다" 규칙을 따릅니다.
- `PlatformingGameMode`/`PlatformingPlayerController` 외에 커스텀 GameMode나 AI 로직은 없습니다. 이 variant는 순수하게 싱글플레이어 이동감에 초점을 맞춥니다.

### Variant_SideScrolling — 2.5D 플랫포머 + StateTree NPC
- `ASideScrollingCharacter`는 이동을 한 축으로 제한하고, 월 점프, 더블 점프, 코요테 타임, 전용 콜리전 오브젝트 타입과 트레이스 기반 활성/비활성화를 통한 "소프트 플랫폼" 통과 낙하, 그리고 `ISideScrollingInteractable` 오브젝트(`SideScrollingPickup`, `SideScrollingJumpPad`, `SideScrollingMovingPlatform`, `SideScrollingSoftPlatform`)를 위한 인터랙트 액션을 추가합니다.
- `SideScrollingAIController` + `SideScrollingStateTreeUtility`는 Combat variant의 StateTree 패턴을 (전투가 아닌) NPC(`SideScrollingNPC`) 행동에 맞게 그대로 미러링합니다.
- `SideScrollingCameraManager`는 카메라를 스크롤 평면 안에 제한합니다.
- UI: `SideScrollingUI`(UMG)는 이 variant 전용 HUD 요소를 담당합니다.

## 이 코드베이스를 확장할 때 따라야 할 규칙

- 새로운 actor/character 클래스는 `UCLASS(abstract)` C++ 베이스 클래스로 만듭니다. 구체적인 인스턴스(머티리얼, 메시, 정확한 스탯)는 C++ leaf 클래스를 추가하는 대신 `Content/`의 Blueprint 서브클래스에서 설정합니다. 대부분의 `EditAnywhere` 프로퍼티는 하드코딩이 아니라 Blueprint별로 조정된다고 가정하세요.
- 데미지, 인터랙션, 공격 같은 횡단 관심사(cross-cutting behavior)는 공유 베이스 클래스가 아니라 `UINTERFACE`/`I*` 쌍으로 표현됩니다 — 새로운 기능을 추가하기 전에 `Variant_*/Interfaces/`에 이미 해당 기능을 다루는 인터페이스가 있는지 먼저 확인하세요.
- AI 의사결정은 프로젝트 전체에서 StateTree 기반입니다(`Variant_Combat`, `Variant_SideScrolling` 모두). StateTree로 표현할 수 없는 특별한 이유가 없다면 새 AI에 Behavior Tree를 도입하지 마세요.
- 플레이어가 수행하는 액션은 항상 두 번 노출됩니다: private Enhanced Input 핸들러와 public `BlueprintCallable DoX(...)` 메서드로, 동일한 액션을 터치/UI 입력으로도 구동할 수 있게 합니다. 입력 동작을 변경할 때는 두 쪽을 모두 동기화하세요.
- 타이밍에 민감한 게임플레이 순간(공격 히트, 콤보 윈도우, 대시 종료)은 하드코딩된 C++ 타이머가 아니라 소유 캐릭터를 콜백 호출하는 `UAnimNotify` 서브클래스로 구동됩니다 — 애니메이션과 동기화되는 새로운 동작을 추가할 때 이 패턴을 따르세요.
