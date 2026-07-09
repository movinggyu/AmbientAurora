#include "StaticMode.h"

namespace AmbientAurora {

StaticMode::StaticMode() : m_color(1.0f, 0.0f, 0.0f) { // 기본값: 하얀색
}

StaticMode::StaticMode(const float hue, const OKLCHColor& prevColor) : m_color(OKLCHColor(prevColor.l, prevColor.c, hue)) {
}

RGBColor StaticMode::getCurrentColor() const {
    return OKLCHToRGB(m_color);
}

RGBColor StaticMode::getAndSetNextColor() {
    return OKLCHToRGB(m_color); // Static모드는 항상 현재 색상을 반환
}

void StaticMode::setColor(const OKLCHColor& color) {
    m_color = color;
}

void StaticMode::setColor(float l, float c, float h) {
    OKLCHColor oklch(l, c, h);
    m_color = oklch;
}

} // namespace AmbientAurora
