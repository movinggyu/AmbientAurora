#ifndef AMBIENT_AURORA_COLORANIMATOR_H
#define AMBIENT_AURORA_COLORANIMATOR_H

#include "Color.h"
#include "Types.h"

namespace AmbientAurora {
class ColorAnimator {
public:
    ColorAnimator();

    // 새로운 목표 색상, 변하는 시간, 유지하는 시간을 설정하고 애니메이션 시작
    void startTransition(const OKLCHColor& targetColor, float totalTime, float transitionDuration);

    // 메인 루프에서 매 프레임 호출 (deltaTime은 밀리초(ms) 단위, 예: 20.0f)
    void update(float deltaTime);

    // 현재 계산이 완료된 프레임의 RGB 색상 반환
    RGBColor getCurrentColor() const;

    // 현재 애니레이터의 상태 확인
    AnimationState getState() const { return m_state; }
    bool isFinished() const { return m_state == AnimationState::IDLE; }

private:
    // OKLCH 색상 공간에서 두 색상을 부드럽게 이어주는 헬퍼 함수
    OKLCHColor interpolateColor(const OKLCHColor& start, const OKLCHColor& target, float progress);

private:
    OKLCHColor m_startColor;     // 전환 시작 색상
    OKLCHColor m_targetColor;    // 목표 색상
    OKLCHColor m_currentColor;   // 현재 프레임의 계산된 색상

    float m_elapsedTime;         // 현재 구간에서 흘러간 시간
    float m_totalTime;           // 현재 구간의 총 시간 (transitionDuration + holdDuration)
    float m_transitionDuration;  // 변하는 구간의 총 시간

    AnimationState m_state;      // 현재 애니메이션 상태
};

} // namespace AmbientAurora

#endif // AMBIENT_AURORA_COLORANIMATOR_H