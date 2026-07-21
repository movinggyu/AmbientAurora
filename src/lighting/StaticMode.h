#ifndef AMBIENT_AURORA_STATICMODE_H
#define AMBIENT_AURORA_STATICMODE_H

#include "ILightingMode.h"
#include "../core/Color.h"

namespace AmbientAurora {

class StaticMode : public ILightingMode {
public:
    // 생성자 (기본 색상은 흰색)
    StaticMode();
    // 생성자 (색상 지정)
    explicit StaticMode(const float hue, const OKLCHColor& prevColor);

    // ILightingMode 인터페이스 구현
    OKLCHColor getCurrentColor() const override;
    OKLCHColor getAndSetNextColor() override;

    // 색상 설정 (슬라이더는 색상, 밝기, 채도를 사용하기 때문에 OKLCHColor를 사용, 구현 시 내부적으로 RGB로 변환하여 저장)
    void setLightnessAndChroma(float lightness, float chroma) override {m_currentColor.l = lightness; m_currentColor.c = chroma;}
    void setHue(float h) {m_currentColor.h = h;};
    void onActivate(const OKLCHColor& prevColor) override {m_currentColor = prevColor;}

private:
    // 현재 색상 저장
    OKLCHColor m_currentColor;
};

} // namespace AmbientAurora

#endif // AMBIENT_AURORA_STATICMODE_H
