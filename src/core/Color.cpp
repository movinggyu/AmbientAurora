#include "Color.h"
#include <algorithm>
#include <cmath>

namespace AmbientAurora {

    // RGB (r, g, b: 0.0 ~ 1.0) -> HSV (h: 0.0 ~ 360.0, s: 0.0 ~ 1.0, v: 0.0 ~ 1.0)
    HSVColor RGBToHSV(const RGBColor& rgb) {
        float r = std::clamp(rgb.r, 0.0f, 1.0f);
        float g = std::clamp(rgb.g, 0.0f, 1.0f);
        float b = std::clamp(rgb.b, 0.0f, 1.0f);

        float maxVal = std::max({r, g, b});
        float minVal = std::min({r, g, b});
        float delta = maxVal - minVal;

        float v = maxVal;
        float s = (maxVal > 0.0f) ? (delta / maxVal) : 0.0f;
        float h = 0.0f;

        if (delta > 0.0f) {
            if (maxVal == r) {
                h = 60.0f * std::fmod(((g - b) / delta) + 6.0f, 6.0f);
            } else if (maxVal == g) {
                h = 60.0f * (((b - r) / delta) + 2.0f);
            } else {
                h = 60.0f * (((r - g) / delta) + 4.0f);
            }
        }

        return HSVColor(h, s, v);
    }

    // HSV (h: 0.0 ~ 360.0, s: 0.0 ~ 1.0, v: 0.0 ~ 1.0) -> RGB (r, g, b: 0.0 ~ 1.0)
    RGBColor HSVToRGB(const HSVColor& hsv) {
        float h = std::fmod(hsv.h, 360.0f);
        if (h < 0.0f) h += 360.0f;

        float s = std::clamp(hsv.s, 0.0f, 1.0f);
        float v = std::clamp(hsv.v, 0.0f, 1.0f);

        // 삼각파 공식을 활용한 분기 없는 수학식 변환
        auto f = [h, s, v](float n) {
            float k = std::fmod(n + h / 60.0f, 6.0f);
            return v - v * s * std::max(0.0f, std::min({k, 4.0f - k, 1.0f}));
        };

        // 각각의 채널 값을 계산해서 변수에 담음
        float r = f(5.0f); // Red 계산 결과
        float g = f(3.0f); // Green 계산 결과
        float b = f(1.0f); // Blue 계산 결과

        // RGBColor 형태로 반환
        return RGBColor(r, g, b);
    }

} // namespace AmbientAurora