#include "StaticMode.h"

namespace AmbientAurora {

StaticMode::StaticMode() : m_color(255, 255, 255) { // Default to white
}

StaticMode::StaticMode(const float hue, const OKLCHColor& prevColor) : m_color(OKLCHColor(prevColor.l, prevColor.c, hue)) {
}

RGBColor StaticMode::getCurrentColor() const {
    return OKLCHToRGB(m_color);
}

void StaticMode::setColor(const OKLCHColor& color) {
    m_color = color;
}

void StaticMode::setColor(float l, float c, float h) {
    OKLCHColor oklch(l, c, h);
    m_color = oklch;
}

} // namespace AmbientAurora
