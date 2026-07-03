#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render/ScreenRenderer.h"
#include "core/Color.h"
#include "core/Types.h"
#include <iostream>
#include <vector>

// 창 크기 변경 콜백
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // 1. GLFW 초기화 및 컨텍스트 설정
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2. 윈도우 창 생성
    GLFWwindow* window = glfwCreateWindow(800, 600, "Ambient Aurora - Renderer Test", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 3. GLAD 로딩 (반드시 OpenGL 컨텍스트 활성화 후 호출)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 4. ScreenRenderer 인스턴스 생성 및 초기화
    AmbientAurora::ScreenRenderer renderer;
    if (!renderer.initialize(800, 600)) {
        std::cerr << "Failed to initialize ScreenRenderer" << std::endl;
        return -1;
    }

    // 5. 테스트용 더미 색상 데이터 세팅
    // 이전 색상: 빨강
    AmbientAurora::RGBColor oldColor(1.0f, 0.0f, 0.0f); // Red

    // 새 색상: 파랑
    AmbientAurora::RGBColor newColor(0.0f, 0.0f, 1.0f); // Blue

    // 6. 상태 제어 변수들
    float progress = 0.0f;
    int currentModeIndex = 0;
    // Types.h의 순서에 맞춰 캐스팅 처리 (0: DISSOLVE, 1: WIPE, 2: CENTER)
    AmbientAurora::TransitionMode modes[] = {
        AmbientAurora::TransitionMode::DISSOLVE,
        AmbientAurora::TransitionMode::WIPE,
        AmbientAurora::TransitionMode::CENTER
    };

    float lastTime = static_cast<float>(glfwGetTime());

    // 7. 메인 루프 시작
    while (!glfwWindowShouldClose(window)) {
        // 시간 델타 계산
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // progress 속도 조절 (2초 동안 0.0에서 1.0까지 증가)
        progress += deltaTime * 0.5f; 
        if (progress > 1.0f) {
            progress = 0.0f;
            // 효과가 끝나면 다음 변형 모드로 전환하며 무한 반복
            currentModeIndex = (currentModeIndex + 1) % 3;
            
            // 시각적 재미를 위해 끝날 때마다 이전/새 색상을 스왑해줍니다.
            std::swap(oldColor, newColor);
        }

        // 화면 렌더링 프레임 호출
        renderer.renderFrame(modes[currentModeIndex], progress, oldColor, newColor, true, 1.0f);

        // 이벤트 및 더블 버퍼 교체
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 자원 정리
    renderer.shutdown();
    glfwTerminate();
    return 0;
}