#include "StaticMode.h"

namespace AmbientAurora {

StaticMode::StaticMode() : m_currentColor(0.0f, 0.9f, 0.8f){ // 기본값: 빨간색
}

StaticMode::StaticMode(const float hue, const HSVColor& prevColor) : m_currentColor(HSVColor(hue, prevColor.s, prevColor.v)) {
}

HSVColor StaticMode::getCurrentColor() const {
    return m_currentColor;
}

HSVColor StaticMode::getAndSetNextColor() {
    return m_currentColor;
}

} // namespace AmbientAurora
