#ifndef AMBIENT_AURORA_SCREENRENDERER_H
#define AMBIENT_AURORA_SCREENRENDERER_H

#include "../core/Color.h"
#include "../core/Types.h"
#include "../core/ColorAnimator.h"

namespace AmbientAurora {

class ScreenRenderer {
public:
    ScreenRenderer();
    ~ScreenRenderer();

    // 초기화 및 해제
    bool initialize(int width, int height);

    // 프로그램 종료 시 자원 해제
    void shutdown();

    // 메인 루프에서 호출되어 화면을 그리는 핵심 메서드
    // ColorAnimator를 참조로 넘겨받아 내부에서 progress, startColor, targetColor를 사용하여 화면을 렌더링
    void renderFrame(const ColorAnimator& animator, TransitionMode mode);

private:
    // OpenGL 셰이더 프로그램을 생성하고 컴파일하는 내부 헬퍼 함수
    unsigned int compileShader(const char* vertexSource, const char* fragmentSource);

private:
    int m_screenWidth;
    int m_screenHeight;

    // Mordern OpenGL 자원 관리를 위한 ID 변수들
    unsigned int m_shaderProgram;
    unsigned int m_vao;
    unsigned int m_vbo;

    // 셰이더 내부 Uniform 변수들의 위치(Location) 캐싱
    int m_locStartColor;
    int m_locTargetColor;
    int m_locProgress;
    int m_locTransitionMode;
};

} // namespace AmbientAurora

#endif // AMBIENT_AURORA_SCREENRENDERER_H