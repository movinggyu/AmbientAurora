// 오로라모드는 색상(hue)범위 내에서 랜덤하게 샘플링한 색상을 현재 색상으로 설정하여, 오로라처럼 부드럽게 색상이 변화하는 효과를 구현하는 모드입니다.

#ifndef AMBIENT_AURORA_AURORAMODE_H
#define AMBIENT_AURORA_AURORAMODE_H

#include "ILightingMode.h"
#include "../core/Color.h"
#include "../core/Types.h"
#include <random>

namespace AmbientAurora {
class AuroraMode : public ILightingMode {
public:
    AuroraMode();
    AuroraMode(const float startHue, const float endHue, float samplingRange,  const OKLCHColor& prevColor, const AmbientAurora::TransitionMode transitionMode = AmbientAurora::TransitionMode::DISSOLVE);


    RGBColor getCurrentColor() const override;

    bool isHueInRange(float hue) const;
    void setStartHue(float hue);
    void setEndHue(float hue);
    void setSamplingRange(float range);
    void setTransitionMode(AmbientAurora::TransitionMode transitionMode);
    float calculateNextColor(); // 다음 색상을 계산
    void updateColor(); // m_currentColor을 업데이트

private:
    float startHue; // 시작 색상(Hue, 전환은 색상범위 내에서 하기 때문에 float로 저장)
    float endHue; // 끝 색상(Hue, 전환은 색상범위 내에서 하기 때문에 float로 저장)
    float samplingRange; // 샘플링 범위 (Hue, 전환은 색상범위 내에서 하기 때문에 float로 저장)
    OKLCHColor m_currentColor; // 현재 색상(OKLCH인 이유는 Hue를 기반으로 색상을 계산하기 위해. RGB로 변환하여 반환)
    AmbientAurora::TransitionMode m_transitionMode; // 디졸브, 와이프, 센터 등 트랜지션 방법
    std::mt19937 m_gen; // 난수 생성기 초기화
};
} // namespace AmbientAurora

#endif //AMBIENT_AURORA_AURORAMODE_H