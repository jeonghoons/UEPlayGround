# DESIGN_ANIMATION

## 확정 사항
- 캐릭터는 UE5 매니퀸 스켈레톤 하나만 쓰는 모듈형 구조다. 표준 스켈레톤은 Epic `SK_Mannequin`이고, 외부 에셋(Pirate `SKM_Manny_Skeleton` 등)은 본이 같으면 호환 스켈레톤으로 연결하거나 Skeleton을 교체해 BS/ABP/AnimLayer를 공유한다(DECISIONS 10-04). 기본은 의상 없는 몸이고, 의상/무기를 착용·해제한다. `Content/Pirate/`는 메시/의상을 공급하는 에셋 팩일 뿐 캐릭터 이름이 아니다.
- 캐릭터는 남/여 두 성별이며 같은 스켈레톤을 쓰므로 애니메이션(블렌드 스페이스, 공격 등)을 공유한다. 다른 스켈레톤의 외부 에셋은 UE IK Retargeter로 임포트 시 변환한다(Blender 등 외부 툴 미사용).
- 모듈형 메시는 Leader Pose 방식이다. 몸(남/여)이 Leader이고 슬롯별 메시(Head/Top/Bottom/Feet/Hands/Back)가 Follower다.
- Leader는 속옷 입은 FullBody 단일 메시(남/여 각각)다. 장비를 해제하면 Follower 메시만 비우면 되고 맨몸 상태가 항상 완결된다. 장비가 가리는 몸 부위는 몸 메시의 머티리얼 슬롯 교체로 숨긴다(슬롯이 하나뿐이면 마스크 머티리얼 파라미터 검토, DECISIONS 2026-10-13).
- 실제 사용 에셋은 팩 폴더에서 `Content/_Game/Character/`로 이동한 뒤 `Body/{Male,Female}`(몸·스켈레톤·피직스 같은 폴더)와 `Equipment/{Weapon,Top,Bottom,Outfit,Head,Hands,Feet}`로 나누고 개명해 관리한다. 남/여 스켈레톤은 본 구조가 같고 `SK_Mannequin`의 Compatible Skeletons로 등록해 공유한다.
- 무기는 한손검/두손검/활/총. 무기에 따라 애니메이션 연출이 달라진다.
- 맨손(`Unarmed`) 상태가 존재한다. 이동/대기 등 Locomotion 애니메이션은 있지만 **공격과 스킬은 불가능**하다. 공격 가능 여부는 코드 분기가 아니라 WeaponTable의 데이터(예: 공격 가능 플래그, 사용 가능 스킬 목록이 비어 있음)로 표현한다.

## 목표 구조 (Animation Layer Interface)
- Animation Layer Interface에 `Locomotion`/`Attack` 등 함수 시그니처를 정의한다.
- 무기별 AnimLayer: `AL_Unarmed`, `AL_OneHandSword`, `AL_TwoHandSword`, `AL_Bow`, `AL_Gun`.
- 메인 AnimInstance가 무기 전환 시 `LinkAnimClassLayers`로 레이어를 링크한다.
- 무기 → AnimLayer/메시/소켓 매핑은 WeaponTable에 둔다.
- 공격 히트/콤보 윈도우 같은 타이밍은 C++ 타이머가 아니라 AnimNotify가 소유 캐릭터를 콜백한다.

## 장착/해제 흐름
미정 (Phase 3에서 작성): 소켓 부착, 레이어 링크, 장착 모션 순서.

## 미정
- (확정) 스켈레톤은 UE5 매니퀸만 사용한다(UE4 메시 미사용). 이동 애니메이션은 템플릿 `BS_Idle_Walk_Run`을 재사용한다. 공격 애니메이션 출처는 미정
- 모듈형 캐릭터 구조(기본 바디 + 의상/무기 슬롯 착탈, 메시 합성 방식)는 Phase 3에서 설계한다
- 이동/공격 애니메이션 애셋 출처
