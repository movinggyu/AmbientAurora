cmake_minimum_required(VERSION 3.14)
project(AmbientAuroraTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. FetchContent 기능을 사용하여 GLFW를 자동으로 인터넷에서 다운로드 및 빌드
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8 # 검증된 안정화 버전
)

# GLFW 빌드 시 불필요한 문서나 예제 빌드는 제외하여 속도 업
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# 2. 빌드할 소스 파일들 등록 (현재 프로젝트 구조에 맞게 경로 지정)
# main.cpp와 glad.c가 src 폴더 바로 아래에 있고, ScreenRenderer가 src/render에 있다고 가정합니다.
add_executable(AmbientAuroraTest
    src/mainTest.cpp
    src/render/glad/glad.c
    src/render/ScreenRenderer.cpp
)

# 3. 헤더 파일들을 찾을 수 있도록 인클루드 디렉토리 설정
target_include_directories(AmbientAuroraTest PRIVATE
    src
    src/render
    src/core
    src/render/glad
)

# 4. GLFW 라이브러리 링크
target_link_libraries(AmbientAuroraTest PRIVATE glfw)

# 5. 운영체제(OS)별 필수 그래픽 드라이버/라이브러리 자동 링크
if(WIN32)
    target_link_libraries(AmbientAuroraTest PRIVATE opengl32)
elseif(APPLE)
    target_link_libraries(AmbientAuroraTest PRIVATE "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(AmbientAuroraTest PRIVATE OpenGL::GL)
endif()