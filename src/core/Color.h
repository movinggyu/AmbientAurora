#ifndef AMBIENT_AURORA_COLOR_H
#define AMBIENT_AURORA_COLOR_H

namespace AmbientAurora {
    struct RGBColor { // RGB 색상 구조체 (각 채널 0.0f ~ 1.0f)
        float r;
        float g;
        float b;

        RGBColor() : r(0), g(0), b(0) {}
        RGBColor(float red, float green, float blue) : r(red), g(green), b(blue) {}
    };

    struct HSVColor { // HSV 색상 구조체
        float h;
        float s;
        float v;

        HSVColor() : h(0.0f), s(0.0f), v(0.0f) {}
        HSVColor(float hue, float saturation, float value) : h(hue), s(saturation), v(value) {}
    };

    HSVColor RGBToHSV(const RGBColor& rgb); // RGB 색상을 OKLCH 색상으로 변환
    RGBColor HSVToRGB(const HSVColor& hsv); // OKLCH 색상을 RGB 색상으로 변환
} // namespace AmbientAurora

#endif // AMBIENT_AURORA_COLOR_H
