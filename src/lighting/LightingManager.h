#ifndef AMBIENT_AURORA_LIGHTINGMANAGER_H
#define AMBIENT_AURORA_LIGHTINGMANAGER_H

#include "ILightingMode.h"
#include "../core/ColorAnimator.h"
#include <memory> // memory는 메모리누수를 방지하기 위해 스마트 포인터(unique_ptr)를 사용하기 위해 필요.

namespace AmbientAurora {
class LightingManager {
public:
    LightingManager();

    // UI 등에서 모드를 바꿀 때 호출
    void changeMode(std::unique_ptr<ILightingMode> newMode);

    // 메인 루프(Application)에서 매 프레임 호출할 핵심 루프
    void update(float deltaTime);

    // 최종적으로 화면/모니터 렌더러가 사용할 색상 반환
    RGBColor getFinalColor() const;

private:
    std::unique_ptr<ILightingMode> m_activeMode; // 현재 켜져 있는 모드
    ColorAnimator m_animator;                    // 색상을 부드럽게 전환하기 위한 애니메이터
    
    // 기획에 따른 밀리초(ms) 단위 시간 세팅 변수 (예: 3초 전체, 2초 변환)
    float m_defaultTotalTime;       
    float m_defaultTransitionTime;

    const float m_defaultHoldTime = 1000.0f; // 1초 유지
    const float m_defaultTransitionDuration = 500.0f; // 0.5초 변환
};
}
#endif