#include "StaticMode.h"

namespace AmbientAurora {

StaticMode::StaticMode() : m_color(0.5f, 0.5f, 0.0f) { // 기본값: 빨간색
}

StaticMode::StaticMode(const float hue, const OKLCHColor& prevColor) : m_color(OKLCHColor(prevColor.l, prevColor.c, hue)) {
}

OKLCHColor StaticMode::getCurrentColor() const {
    return m_color;
}

OKLCHColor StaticMode::getAndSetNextColor() {
    return m_color;
}

void StaticMode::setColor(const OKLCHColor& color) {
    m_color = color;
}

void StaticMode::setColor(float l, float c, float h) {
    OKLCHColor oklch(l, c, h);
    m_color = oklch;
}

} // namespace AmbientAurora
