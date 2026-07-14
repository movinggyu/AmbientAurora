# AmbientAurora

AmbientAurora는 화면 전체를 부드럽게 변하는 색과 그라데이션으로 채우는 **Qt 6 + OpenGL 데스크톱 애플리케이션 프로토타입**입니다. 차분한 주변 조명(ambient light) 같은 화면 경험을 목표로 하며, 중앙 로고와 반투명 컨트롤 패널을 결합한 미니멀한 인터페이스를 실험합니다.

> 이 저장소는 기능을 다듬는 중인 프로토타입입니다. 현재 실행 화면은 데모용 색상과 전환 값을 사용하며, 모든 UI 제어가 렌더러에 연결된 상태는 아닙니다.

## 화면 구성

![AmbientAurora logo](src/assets/full_logo.png)

- **OpenGL 배경**: 전체 화면 쿼드를 렌더링해 색상 전환과 공간 그라데이션을 표시합니다.
- **유리 질감 UI**: Qt Quick(QML) 기반의 중앙 로고와 하단 컨트롤 바를 배경 위에 겹쳐 표시합니다.
- **색상 전환**: 디졸브, 좌우 와이프, 중앙 확산 방식의 전환을 셰이더에서 처리합니다.
- **OKLCH 색 보간**: 셰이더에서 RGB와 OKLCH 색 공간을 변환해 색조 변화가 자연스럽게 보이도록 합니다.

## 현재 구현 상태

| 영역 | 상태 | 내용 |
| --- | --- | --- |
| Qt 데스크톱 창 | 구현됨 | `QMainWindow` 위에 OpenGL과 QML 레이어를 겹쳐 표시합니다. |
| OpenGL 렌더링 | 구현됨 | 60 FPS 타이머 기반으로 전체 화면 색 전환을 렌더링합니다. |
| 전환 효과 | 구현됨 | 디졸브, 와이프, 중앙 확산 전환 셰이더가 포함돼 있습니다. |
| QML 오버레이 | 구현됨 | 로고 버튼, 하단 바, 슬라이더 패널 UI가 포함돼 있습니다. |
| 조명 모드 모델 | 기반 구현 | Static, Dynamic, Aurora 모드와 `LightingManager`가 있으나 현재 렌더러와 연결 작업이 남아 있습니다. |
| 카메라·프리셋 | 미구현 | 초기 기획에는 있었지만 현재 빌드 대상에 포함되어 있지 않습니다. |

## 기술 구성

- C++17
- CMake 3.14 이상
- Qt 6: Widgets, OpenGLWidgets, QuickWidgets
- OpenGL 3.3 Core 및 GLAD
- QML / Qt Quick

## 빌드 및 실행

### 준비물

- CMake 3.14 이상
- C++17 컴파일러
- Qt 6 개발 환경 (Widgets, OpenGLWidgets, QuickWidgets 모듈)
- OpenGL 3.3 이상을 지원하는 그래픽 드라이버

### CMake로 빌드

프로젝트 루트에서 아래 명령을 실행합니다.

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

생성된 실행 파일은 일반적으로 `build/Debug/AmbientAuroraQt.exe` 또는 사용 중인 생성기 설정에 따른 `build/AmbientAuroraQt.exe`에 있습니다.

```powershell
.\build\AmbientAuroraQt.exe
```

Qt를 CMake가 찾지 못하면 Qt 설치 경로를 지정해 구성하세요.

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\msvc*_64"
```

## 사용 방법

1. 앱을 실행하면 파랑과 분홍 계열이 반복 전환되는 배경이 표시됩니다.
2. 중앙 로고를 누르면 하단 컨트롤 바가 나타납니다.
3. 하단 바에서 슬라이더 패널을 열거나 컨트롤 바를 닫을 수 있습니다.

현재 일부 버튼과 패널은 UI 동작을 확인하기 위한 형태이며, 렌더링 파라미터를 바꾸지는 않습니다.

## 프로젝트 구조

```text
src/
├── core/       색상 구조체·색 공간 변환·애니메이션 기반
├── lighting/   Static, Dynamic, Aurora 조명 모드 모델
├── render/     OpenGL 셰이더 및 전체 화면 렌더러
├── ui/         Qt Widgets 창, OpenGL 위젯, QML 오버레이
└── assets/     로고 이미지
docs/           성능 분석 및 개선 메모
```

## 다음 단계

- QML 컨트롤을 `LightingManager` 및 `ScreenRenderer`의 전환 파라미터에 연결
- 조명 모드별 색상 선택과 전환 속도 설정 지원
- QML 파일과 리소스 경로를 배포 가능한 형태로 정리
- 렌더링·QML 레이어 중첩에 대한 성능 측정과 최적화

## 라이선스

현재 라이선스 파일이 포함되어 있지 않습니다. 재사용 또는 배포 전에는 프로젝트 소유자와 라이선스 조건을 확인해 주세요.
