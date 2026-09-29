# External Assets (git에 포함되지 않음)

용량/라이선스 문제로 아래 에셋은 저장소에 올리지 않습니다. clone 후 직접 설치하세요.
엔진 버전: **Unreal Engine 5.6**

## Fab / Marketplace 에셋
| 폴더 | 이름 | 링크 | 크기 | 설치 방법 |
|---|---|---|---|---|
| `Content/Singapore_Canal` | (에셋 이름 기입) | (Fab 링크 기입) | ≈2.1GB | Epic Games Launcher → Fab 라이브러리 → Add to Project → `Test` |
| `Content/Polyphoria` | (에셋 이름 기입) | (Fab 링크 기입) | ≈0.9GB | 위와 동일 |
| `Content/Pirate` | (에셋 이름 기입) | (Fab 링크 기입) | ≈0.4GB | 위와 동일 |

## Third Person 템플릿 스타터 콘텐츠
| 폴더 | 복원 방법 |
|---|---|
| `Content/Characters`, `Content/ThirdPerson`, `Content/LevelPrototyping` | 새 프로젝트 생성: Games → Third Person → C++ (Starter Content 없이) 후 해당 폴더를 복사 |
| `Content/Variant_Combat`, `Variant_Platforming`, `Variant_SideScrolling` | 위 템플릿 생성 시 "Variants" 옵션 포함하여 생성 후 복사 |
| `Content/__ExternalActors__/{ThirdPerson,Variant_*}`, `__ExternalObjects__/...` | 위 맵과 함께 복사 (One-File-Per-Actor 데이터) |

> 기본 맵(`Config/DefaultEngine.ini`)은 `Lvl_ThirdPerson`이므로, 스타터 콘텐츠를 복원하기 전에는 맵 로드 경고가 표시됩니다.

## 백업 권장
받은 에셋 폴더는 저장소 밖(외장 디스크/클라우드)에 zip으로 백업해 두세요.
