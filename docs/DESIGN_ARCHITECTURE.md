# DESIGN_ARCHITECTURE

목표 아키텍처 문서입니다(구현 현황은 `ROADMAP.md`).

## 핵심 원칙: 서버가 붙을 자리(Seam)
서버가 없어도 나중에 패킷으로 교체할 수 있도록 게임플레이를 **의도(Intent) → 이벤트(Event)** 로 분리합니다.

- 클라이언트 → 백엔드 (의도): `RequestMove`, `RequestAttack`, `RequestCastSkill`, `RequestPickupItem`, `RequestEquip`
- 백엔드 → 클라이언트 (이벤트): `OnSpawnMonster`, `OnDamage`, `OnMonsterDead`, `OnDropItem`, `OnEquipChanged`
- `IGameBackend`: 위 의도/이벤트를 정의하는 인터페이스. 클라이언트 게임플레이 코드는 이것만 안다.
- `ULocalGameBackend`: 현재 구현. 스폰 마커 데이터와 DataTable로 서버 응답을 흉내낸다.
- `UNetworkGameBackend`: 서버 연동 시 추가할 구현체 (지금은 만들지 않음).
- 몬스터 AI와 드랍 계산은 서버 책임이므로 클라이언트에는 만들지 않는다. 클라이언트는 애니메이션/연출만.

## 네이밍
신규 클래스는 접두사 없이 `Source/Test/MMO/` 폴더로 구분한다. UE에 이미 있는 이름과 충돌하는 경우에만 `My` 접두사를 붙인다 (예: `AMyGameMode`, `AMyPlayerController`는 접두사 사용, `APlayerCharacter`는 충돌이 없어 접두사 없음).

## 폴더 구조 (기본값, Phase 0에서 확정)
```
Source/Test/MMO/
  Core/        GameMode, PlayerController, GameInstance, DataManager
  Backend/     IGameBackend, ULocalGameBackend
  Character/   플레이어/몬스터 캐릭터, 컴포넌트
  Combat/      SkillComponent, 판정 모듈
  Animation/   AnimInstance, AnimNotify
  Data/        Row 구조체
  UI/          위젯
Source/TestEditor 또는 에디터 전용 코드: Export 도구
```

## 미정
- 에디터 전용 코드를 별도 모듈로 둘지 (Phase 7 전에 결정)
