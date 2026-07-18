#include "LightingManager.h"
#include "StaticMode.h"

namespace AmbientAurora {

LightingManager::LightingManager():
    m_activeMode(nullptr),
    m_defaultTotalTime(3000.0f),      // 3초
    m_defaultTransitionTime(2000.0f)  // 2초
{
    m_staticMode = std::make_unique<StaticMode>();
    m_dynamicMode = std::make_unique<DynamicMode>(); 
    m_auroraMode = std::make_unique<AuroraMode>();

    m_activeMode = m_staticMode.get(); // 초기모드: StaticMode
}

void LightingManager::changeMode(LightingModeType modeType) {
    OKLCHColor prevColor = m_activeMode->getCurrentColor();
    
    switch (modeType) {
        case LightingModeType::Static:
            m_activeMode = m_staticMode.get();
            break;
        case LightingModeType::Dynamic:
            m_activeMode = m_dynamicMode.get();
            break;
        case LightingModeType::Aurora:
            m_activeMode = m_auroraMode.get();
            break;
    }

    if(m_activeMode) {
        // 이전 모드의 색상으로 시작
        m_activeMode->onActivate(prevColor);

        // 새 모드의 다음 색상을 가져와 애니메이터 시작
        // TODO: 여기 로직 이상함
        OKLCHColor startColor = m_activeMode->getAndSetNextColor();
        m_animator.startTransition(m_defaultHoldTime, m_defaultTransitionDuration); // 기본 전환시간으로 색 전환하며 모드 전환
    }
}

void LightingManager::update(float deltaTime) {
    if(!m_activeMode) return; // 활성 모드가 없으면 아무 작업도 하지 않음

    if(m_animator.isFinished()) {
        // 애니메이터가 끝났다면, 활성 모드에서 다음 색상을 가져와 애니메이터 시작
        OKLCHColor nextColor = m_activeMode->getAndSetNextColor();
        m_animator.startTransition(m_defaultTotalTime, m_defaultTransitionTime);
    }

    // 애니메이터 업데이트
    m_animator.update(deltaTime);
}

void LightingManager::setColor(float l, float c, float h) {
    if(!m_activeMode) return;

    auto staticMode = dynamic_cast<StaticMode*>(m_activeMode);
    if(staticMode) staticMode->setColor(l, c, h);
}

} // namespace AmbientAurora
