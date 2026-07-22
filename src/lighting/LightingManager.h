#ifndef AMBIENT_AURORA_LIGHTINGMANAGER_H
#define AMBIENT_AURORA_LIGHTINGMANAGER_H

#include "ILightingMode.h"
#include "StaticMode.h"
#include "DynamicMode.h"
#include "AuroraMode.h"
#include "../core/TransitionManager.h"
#include "../core/Types.h"
#include <memory> // memory는 메모리누수를 방지하기 위해 스마트 포인터(unique_ptr)를 사용하기 위해 필요.

namespace AmbientAurora {
class LightingManager {
public:
    LightingManager();

    // UI 등에서 모드를 바꿀 때 호출
    void changeMode(LightingModeType modeType);

    // 메인 루프(Application)에서 매 프레임 호출할 핵심 루프
    void update(float deltaTime);

    void updateActiveModeColor(float s, float v, float h);
    void updateActiveModeColor(float s, float v);
    void updateActiveModeColorRange(float startHue, float endHue);
    void updateActiveModeSamplingRange(float range);
    void updateActiveModeDeltaHue(float deltaHue);
    void updateUserTime(float totalTime, float transitionTime);

    HSVColor getCurrentColor() const { return m_activeMode->getCurrentColor(); }
    float getProgress() const { return m_animator.getProgress(); }
    float getTotalProgress() const { return m_animator.getTotalProgress(); }
    AnimationState getState() const { return m_animator.getState(); }

private:
    std::unique_ptr<StaticMode> m_staticMode;    // 정적 모드
    std::unique_ptr<DynamicMode> m_dynamicMode;  // 동적 모드
    std::unique_ptr<AuroraMode> m_auroraMode;    // 오로라 모드

    ILightingMode* m_activeMode;                 // 현재 모드를 가르키는 포인터
    TransitionManager m_animator;                // 색상을 부드럽게 전환하기 위한 step 계산기 클래스
    
    // 기획에 따른 밀리초(ms) 단위 시간 세팅 변수 (예: 3초 전체, 2초 변환)
    float m_defaultTotalTime;       
    float m_defaultTransitionTime;

    const float m_defaultHoldTime = 1000.0f; // 1초 유지
    const float m_defaultTransitionDuration = 500.0f; // 0.5초 변환
};
}
#endif
