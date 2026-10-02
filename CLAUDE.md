# CLAUDE.md

이 파일은 Claude Code가 이 저장소에서 작업할 때 따르는 최상위 가이드입니다. 변하지 않는 규칙만 적고, 상세 설계는 `Docs/`를 참고하세요.

## 프로젝트 개요

IOCP 게임 서버와 연동할 **3D MMORPG의 UE 5.6 클라이언트 기초 베이스**입니다(모듈명 `Test`). 현재는 **서버 연동 전 단계**이며, 서버가 해줄 일(몬스터 스폰, 아이템 드랍 등)은 클라이언트 내부에서 흉내냅니다. 검은사막/붉은사막의 이동 조작, 카메라, 판정 방식, UI를 참고하되 모작은 하지 않습니다.

- 캐릭터: **Pirate** 모델 단일 사용(`Content/Pirate/`). 무기는 **한손검/활**. 맨손 상태는 있으나 공격/스킬은 불가.
- 이동은 WASD(자동 달리기), 스킬은 숫자키 1~5(무기별 스킬 목록), **콤보 공격 없음**.
- 기존 Epic Third Person 템플릿 클래스(`Test*`)는 삭제됐습니다. `Variant_Combat`은 **참고용**으로만 두고 수정하지 않으며, 신규 코드는 `Source/Test/` 아래 기능 폴더(Core, Character 등)에 작성합니다. `Variant_Combat`은 Phase 4 종료 시 삭제합니다(Platforming/SideScrolling은 삭제됨).

## 문서 (작업 전에 읽는 순서)

1. [Docs/ROADMAP.md](Docs/ROADMAP.md) — **현재 Phase와 다음 작업**, 완료 기준
2. 해당 작업의 설계 문서:
   - [Docs/DESIGN_ARCHITECTURE.md](Docs/DESIGN_ARCHITECTURE.md) — 백엔드 Seam, 폴더/네이밍
   - [Docs/DESIGN_DATA.md](Docs/DESIGN_DATA.md) — DataTable 규약
   - [Docs/DESIGN_ANIMATION.md](Docs/DESIGN_ANIMATION.md) — 무기별 애니메이션
   - [Docs/DESIGN_COMBAT.md](Docs/DESIGN_COMBAT.md) — 스킬/판정
   - [Docs/DESIGN_CONTROL_CAMERA.md](Docs/DESIGN_CONTROL_CAMERA.md) — 조작/카메라
   - [Docs/DESIGN_LEVEL_EXPORT.md](Docs/DESIGN_LEVEL_EXPORT.md) — 레벨 `.obj`/`.json` Export
   - [Docs/DESIGN_UI.md](Docs/DESIGN_UI.md) — UI
3. [Docs/DECISIONS.md](Docs/DECISIONS.md) — 확정된 결정 로그 (번복하지 말 것)

## 작업 절차 규칙

1. 작업 시작 전 ROADMAP에서 현재 Phase와 다음 미완료 항목을 확인합니다.
2. **한 번에 하나의 체크박스 단위**만 진행합니다. 범위가 크면 먼저 계획을 제시합니다.
3. 문서에 없는 결정이 필요하면 구현 전에 사용자에게 묻거나, 검은사막/붉은사막과 유사한 방식으로 제안합니다. 확정되면 `DECISIONS.md`에 기록합니다.
4. 완료 기준(빌드 성공 + ROADMAP에 적힌 PIE 확인)을 충족하면 체크박스를 갱신합니다.
5. 프로젝트는 C++ 중심이지만 Blueprint/AnimBP/머티리얼/위젯 등 `Content/` 애셋이 필요한 작업은, 사용자가 에디터에서 그대로 따라 할 수 있도록 **모든 단계**를 안내합니다. 안내는 ROADMAP의 "수동 작업"에도 남깁니다. 안내에 포함할 것:
   - 애셋 종류와 **부모 클래스**(어떤 C++ 클래스를 상속하는지), 저장 경로와 이름
   - 에디터 조작 순서(우클릭 메뉴 → 생성 → 열기 등)
   - 설정할 프로퍼티와 값(Class Defaults, 컴포넌트, 메시/애님 지정, DataTable 연결 등)
   - 다른 곳에 연결하는 방법(GameMode의 Default Pawn 지정, 레벨 배치, Project Settings 등)
   - 완료 확인 방법(PIE에서 무엇이 보여야 하는지)
   - 이 C++ 코드가 어떤 프로퍼티/함수를 Blueprint에 노출하는지(`EditAnywhere`, `BlueprintCallable` 등)
6. 사용자가 요청하지 않은 파일 수정/삭제, 템플릿 variant 코드 수정은 하지 않습니다.

## 핵심 아키텍처 규칙

- **서버가 붙을 자리(Seam)**: 게임플레이는 의도(Intent) → 이벤트(Event)로 분리합니다. 클라이언트 게임플레이 코드는 `IGameBackend`만 알고, 지금은 `ULocalGameBackend`가 서버 응답을 흉내냅니다. 몬스터 AI/드랍 계산은 클라이언트에 만들지 않습니다. (`DESIGN_ARCHITECTURE.md`)
- **데이터 기반**: 스탯, 장비/무기, 스킬(쿨타임/시전시간/판정 Shape), 이동, UI 정의는 DataTable(`Data/*.csv`)에서 읽습니다. 수치·ID·경로를 코드에 하드코딩하지 않고, 필요하면 컬럼을 추가합니다. 조회는 `UDataManager`를 통해서만 합니다. (`DESIGN_DATA.md`)
- **타이밍은 AnimNotify**: 공격 판정, 장착 시점 등은 C++ 타이머가 아니라 AnimNotify가 소유 캐릭터를 콜백합니다. 공격 판정은 클라이언트가 수행하고 결과를 백엔드에 보고합니다.
- **무기 애니메이션**: Animation Layer Interface로 무기별 레이어를 링크합니다. 맨손의 공격 불가는 코드 분기가 아니라 데이터로 표현합니다.
- 입력은 Enhanced Input을 사용합니다.
- 새 actor/character 베이스는 `UCLASS(abstract)` C++ 클래스로 만들고, 구체적인 인스턴스는 `Content/`의 Blueprint에서 설정합니다.

## 네이밍

신규 클래스는 접두사 없이 `Source/Test/` 아래 기능 폴더로 구분합니다. UE에 이미 있는 이름과 **충돌할 때만 `My` 접두사**를 붙입니다(예: `AMyGameMode`, `AMyPlayerController`. 충돌이 없는 `APlayerCharacter`는 접두사 없음).

## 빌드

표준 UE5 C++ 프로젝트이며 UBT/Visual Studio로 빌드합니다. 빌드 타겟은 `Source/Test.Target.cs`(`TestTarget`), `Source/TestEditor.Target.cs`(`TestEditorTarget`)이고 모듈은 단일 `Test`입니다.

- 프로젝트 파일 생성(소스 추가/삭제 후): `Test.uproject` 우클릭 → "Generate Visual Studio project files"
- 명령줄 빌드: `"C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" TestEditor Win64 Development -Project="C:\Users\kjhkjh\UEProjects\Test\Test.uproject" -WaitMutex`
- 에디터 실행: `Test.uproject`를 열거나 Visual Studio에서 `TestEditor` 실행
- 모듈 의존성: `Test.Build.cs`. **새 소스 하위 폴더를 만들면 `PublicIncludePaths`도 함께 갱신**합니다.
- 유닛 테스트 스위트는 없으며 에디터/PIE 실행으로 검증합니다.

## 언어

문서와 주석은 한국어로 작성합니다.
