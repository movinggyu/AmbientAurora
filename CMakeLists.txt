cmake_minimum_required(VERSION 3.14)
project(AmbientAuroraQt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Qt의 메타 오브젝트 시스템을 위한 필수 자동화 옵션 활성화
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 2. Qt 패키지 찾기 (개발 환경에 맞춰 Qt6 또는 Qt5로 변경 가능)
# GUI 구성과 OpenGL 렌더링을 위해 Widgets 및 OpenGLWidgets 컴포넌트가 필요합니다.
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets QuickWidgets Qml Quick)

# 3. 빌드할 소스 파일들 등록 (이미지의 폴더 구조 반영)
add_executable(AmbientAuroraQt
    src/mainTest.cpp
    src/ui/AuroraItem.cpp
    src/ui/AuroraRenderer.cpp
    src/render/glad/glad.c
    src/render/ScreenRenderer.cpp
    src/core/Color.cpp
    src/core/TransitionManager.cpp
    src/lighting/StaticMode.cpp
    src/lighting/DynamicMode.cpp
    src/lighting/AuroraMode.cpp
    src/lighting/LightingManager.cpp
)

# 4. 헤더 파일 인클루드 디렉토리 설정
target_include_directories(AmbientAuroraQt PRIVATE
    src
    src/ui
    src/render
    src/render/glad
    src/core
)

# 5. 라이브러리 링크 (GLFW 대신 Qt 라이브러리 링크)
target_link_libraries(AmbientAuroraQt PRIVATE 
    Qt6::Widgets 
    Qt6::OpenGLWidgets
    Qt6::QuickWidgets
    Qt6::Qml
    Qt6::Quick
)

# 6. 운영체제(OS)별 필수 그래픽 드라이버/라이브러리 자동 링크
if(WIN32)
    target_link_libraries(AmbientAuroraQt PRIVATE opengl32)
elseif(APPLE)
    target_link_libraries(AmbientAuroraQt PRIVATE "-framework OpenGL" "-framework Cocoa")
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(AmbientAuroraQt PRIVATE OpenGL::GL)
endif()
