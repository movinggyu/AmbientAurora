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

void LightingManager::updateActiveModeColor(float l, float c, float h) {
    if(!m_activeMode) return;

    m_activeMode->setLightnessAndChroma(l, c);

    auto staticMode = dynamic_cast<StaticMode*>(m_activeMode);
    if(staticMode) staticMode->setHue(h);
}

void LightingManager::updateActiveModeColor(float l, float c) {
    if(!m_activeMode) return;

    m_activeMode->setLightnessAndChroma(l, c);
}

void LightingManager::updateActiveModeColorRange(float startHue, float endHue) {
    if(!m_activeMode) return;

    if(auto dynamicMode = dynamic_cast<DynamicMode*>(m_activeMode)) {
        dynamicMode->setStartHue(startHue);
        dynamicMode->setEndHue(endHue);
    }else if(auto auroraMode = dynamic_cast<AuroraMode*>(m_activeMode)){
        auroraMode->setStartHue(startHue);
        auroraMode->setEndHue(endHue);
    }
}

void LightingManager::updateActiveModeSamplingRange(float range) {
    if(!m_activeMode) return;

    if(auto auroraMode = dynamic_cast<AuroraMode*>(m_activeMode)) {
        auroraMode->setSamplingRange(range);
    }
}

void LightingManager::updateActiveModeDeltaHue(float deltaHue) {
    if(!m_activeMode) return;

    if(auto dynamicMode = dynamic_cast<DynamicMode*>(m_activeMode)) {
        dynamicMode->setDeltaHue(deltaHue);
    }
}

void LightingManager::updateUserTime(float totalTime, float transitionTime) {
    m_defaultTotalTime = totalTime;
    m_defaultTransitionTime = transitionTime;
}

} // namespace AmbientAurora
