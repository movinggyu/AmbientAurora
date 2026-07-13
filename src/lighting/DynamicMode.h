#ifndef AMBIENT_AURORA_DYNAMICMODE_H
#define AMBIENT_AURORA_DYNAMICMODE_H

#include "ILightingMode.h"
#include "../core/Color.h"
#include "../core/Types.h"

namespace AmbientAurora {
class DynamicMode : public ILightingMode {
public:
    DynamicMode();
    DynamicMode(const float startHue, const float endHue, float deltaHue,  const OKLCHColor& prevColor, const AmbientAurora::TransitionMode transitionMode = AmbientAurora::TransitionMode::DISSOLVE);

    RGBColor getCurrentColor() const override;
    RGBColor getAndSetNextColor() override;

    bool isHueInRange(float hue) const;
    void setStartHue(float hue);
    void setEndHue(float hue);
    void setTransitionMode(AmbientAurora::TransitionMode transitionMode);
    void setDeltaHue(float deltaHue);
    float calculateNextHue();

private:
    float m_startHue; // 시작 색상(Hue, 전환은 색상범위 내에서 하기 때문에 float로 저장)
    float m_endHue; // 끝 색상(Hue, 전환은 색상범위 내에서 하기 때문에 float로 저장)
    float m_deltaHue; // 변화량
    OKLCHColor m_currentColor; // 현재 색상(OKLCH인 이유는 Hue를 기반으로 색상을 계산하기 위해. RGB로 변환하여 반환)
    AmbientAurora::TransitionMode m_transitionMode; // 디졸브, 와이프, 센터 등 트랜지션 방법

};
} // namespace AmbientAurora

#endif // AMBIENT_AURORA_DYNAMICMODE_H