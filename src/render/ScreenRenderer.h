#ifndef AMBIENT_AURORA_SCREENRENDERER_H
#define AMBIENT_AURORA_SCREENRENDERER_H

#include "../core/Color.h"
#include "../core/Types.h"
#include <vector>

namespace AmbientAurora {

class ScreenRenderer {
public:
    ScreenRenderer();
    ~ScreenRenderer();

    // 초기화 및 해제
    bool initialize(int width, int height);
    void shutdown();

    // 화면 렌더링
    // mode: 전환 모드 (DISSOLVE, WIPE, CENTER)
    // oldColors: 이전 모드의 현재 색상 배열 (단색이면 크기 1, 그라데이션이면 2~4)
    // newColors: 새롭게 전환될 모드의 현재 색상 배열
    void renderFrame(TransitionMode mode, float progress, 
                     const std::vector<RGBColor>& oldColors, 
                     const std::vector<RGBColor>& newColors);

private:
    // OpenGL 셰이더 컴파일 및 프로그램 링크
    unsigned int compileShader(const char* vertexSource, const char* fragmentSource);

    // 화면 전체를 덮는 사각형 VAO/VBO 설정
    // VAO란? : Vertex Array Object의 약자로, OpenGL에서 정점 데이터를 효율적으로 관리하기 위한 객체입니다.
    // VBO란? : Vertex Buffer Object의 약자로, OpenGL에서 정점 데이터를 GPU 메모리에 저장하여 렌더링 성능을 향상시키는 객체입니다.
    void setupQuad();

private:
    int m_screenWidth;
    int m_screenHeight;

    // Modern OpenGL 자원 관리 ID
    unsigned int m_shaderProgram;
    unsigned int m_vao;
    unsigned int m_vbo;

    // 다중 색상 배열 및 공간 전환 제어를 위한 Uniform 위치 캐싱
    int m_locTransitionMode;
    int m_locProgress;
    int m_locOldColors;       // 셰이더 내 vec3 u_OldColors[4] 배열의 위치
    int m_locOldColorCount;   // 이전 색상 개수
    int m_locNewColors;       // 셰이더 내 vec3 u_NewColors[4] 배열의 위치
    int m_locNewColorCount;   // 새 색상 개수
};

} // namespace AmbientAurora

#endif // AMBIENT_AURORA_SCREENRENDERER_H