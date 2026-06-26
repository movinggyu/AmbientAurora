#ifndef AMBIENT_AURORA_COLOR_H
#define AMBIENT_AURORA_COLOR_H

namespace AmbientAurora {
    struct RGBColor { // RGB 색상 구조체 (각 채널 0 ~ 255)
        int r;
        int g;
        int b;

        RGBColor() : r(0), g(0), b(0) {}
        RGBColor(int red, int green, int blue) : r(red), g(green), b(blue) {}
    };

    struct OKLCHColor { // OKLCH 색상 구조체 (L: 0.0f ~ 1.0f, C: 0.0f ~ 1.0f, H: 0.0f ~ 360.0f)
        float l;
        float c;
        float h;

        OKLCHColor() : l(0.0f), c(0.0f), h(0.0f) {}
        OKLCHColor(float lightness, float chroma, float hue) : l(lightness), c(chroma), h(hue) {}
    };

    OKLCHColor RGBToOKLCH(const RGBColor& rgb); // RGB 색상을 OKLCH 색상으로 변환
    RGBColor OKLCHToRGB(const OKLCHColor& oklch); // OKLCH 색상을 RGB 색상으로 변환
} // namespace AmbientAurora

#endif // AMBIENT_AURORA_COLOR_H
