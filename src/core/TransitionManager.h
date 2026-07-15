#ifndef AMBIENT_AURORA_TRANSITIONMANAGER_H
#define AMBIENT_AURORA_TRANSITIONMANAGER_H

#include "Color.h"
#include "Types.h"
#include <algorithm>

namespace AmbientAurora {
class TransitionManager {
public:
    TransitionManager();

    // 새로운 목표 색상, 변하는 시간, 유지하는 시간을 설정하고 애니메이션 시작
    void startTransition(const OKLCHColor& targetColor, float totalTime, float transitionDuration);

    // 메인 루프에서 매 프레임 호출 (deltaTime은 밀리초(ms) 단위, 예: 20.0f)
    void update(float deltaTime);

    // 현재 애니레이터의 상태 확인
    AnimationState getState() const { return m_state; }
    bool isFinished() const { return m_state == AnimationState::IDLE; }

    // getter 메서드
    OKLCHColor getStartOKLCHColor() const { return m_startColor; }
    OKLCHColor getTargetOKLCHColor() const { return m_targetColor; }
    float getProgress() const { return (m_state == AnimationState::IDLE || m_transitionDuration <= 0.0f) ? 1.0f : std::min(1.0f, (m_elapsedTime / m_transitionDuration)); }

private:
    OKLCHColor m_startColor;     // 전환 시작 색상
    OKLCHColor m_targetColor;    // 목표 색상

    float m_elapsedTime;         // 현재 구간에서 흘러간 시간
    float m_totalTime;           // 현재 구간의 총 시간 (transitionDuration + holdDuration)
    float m_transitionDuration;  // 변하는 구간의 총 시간

    AnimationState m_state;      // 현재 애니메이션 상태
};

} // namespace AmbientAurora

#endif // AMBIENT_AURORA_TRANSITIONMANAGER_H
