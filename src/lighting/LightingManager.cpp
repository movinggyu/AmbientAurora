#include "LightingManager.h"

namespace AmbientAurora {

LightingManager::LightingManager():
    m_activeMode(nullptr),
    m_defaultTotalTime(3000.0f),      // 3초
    m_defaultTransitionTime(2000.0f)  // 2초
{
}

void LightingManager::changeMode(std::unique_ptr<ILightingMode> newMode) {
    m_activeMode = std::move(newMode);

    if(m_activeMode) {
        // 새 모드의 다음 색상을 가져와 애니메이터 시작
        OKLCHColor startColor = m_activeMode->getAndSetNextColor();
        m_animator.startTransition(startColor, m_defaultHoldTime, m_defaultTransitionDuration); // 기본 전환시간으로 색 전환하며 모드 전환
    }
}

void LightingManager::update(float deltaTime) {
    if(!m_activeMode) return; // 활성 모드가 없으면 아무 작업도 하지 않음

    if(m_animator.isFinished()) {
        // 애니메이터가 끝났다면, 활성 모드에서 다음 색상을 가져와 애니메이터 시작
        OKLCHColor nextColor = m_activeMode->getAndSetNextColor();
        m_animator.startTransition(nextColor, m_defaultTotalTime, m_defaultTransitionTime);
    }

    // 애니메이터 업데이트
    m_animator.update(deltaTime);
}

OKLCHColor LightingManager::getFinalColor() const {
    return m_animator.getCurrentColor();
}

} // namespace AmbientAurora
