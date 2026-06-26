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
    void shutdown();

    // 메인 루프에서 호출되어 화면을 그리는 핵심 메서드
    // ColorAnimator를 참조로 넘겨받아 내부에서 progress, startColor, targetColor를 사용하여 화면을 렌더링
    void renderFrame(const ColorAnimator& animator, TransitionMode mode);

private:
    int m_screenWidth;
    int m_screenHeight;
    
    // 필요 시 여기에 그래픽스 파이프라인 관련 변수나 셰이더 객체 배치
};

} // namespace AmbientAurora

#endif // AMBIENT_AURORA_SCREENRENDERER_H