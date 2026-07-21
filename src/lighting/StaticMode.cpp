#include "StaticMode.h"

namespace AmbientAurora {

StaticMode::StaticMode() : m_currentColor(0.5f, 0.5f, 0.0f){ // 기본값: 빨간색
}

StaticMode::StaticMode(const float hue, const OKLCHColor& prevColor) : m_currentColor(OKLCHColor(prevColor.l, prevColor.c, hue)) {
}

OKLCHColor StaticMode::getCurrentColor() const {
    return m_currentColor;
}

OKLCHColor StaticMode::getAndSetNextColor() {
    return m_currentColor;
}

} // namespace AmbientAurora
